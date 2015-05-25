import fnmatch
import os
import sys
import logging
import re
import random
import string
from functools import partial

if sys.version_info[0] == 2:
    def exec_(code, globals_=None, locals_=None):
        # see builtin_exec_impl in bltinmodule.c in CPython source code
        if globals_ is None:
            frame = sys._getframe(1)
            globals_ = frame.f_globals
            if locals_ is None:
                locals_ = frame.f_locals

        exec("exec code in globals_, locals_")
else:
    import builtins
    exec_ = getattr(builtins, 'exec')


def glob_recursive(dirname, pattern):
    matchlist = []

    for root, dirnames, filenames in os.walk(dirname):
        for filename in fnmatch.filter(filenames, pattern):
            if root.startswith('.'):
                root = root[2:]
            matchlist.append(os.path.join(root, filename))

    return matchlist


def escape_quotes(strng):
    return strng.replace("'", "\\'").replace('"', '\\"')


def random_name(length=8):
    chars = string.ascii_letters + string.digits + '_'
    return ''.join(random.choice(chars) for _ in range(length))


class Template(object):

    """
    Simple template engine.

    Example showing the default pattern:

        {%% x = [1, 2, 3] %%}       # single line statement
        {% for i in x: %}           # start block statement
        x = {x}                     # variable substitution
        x + x = {x + x}             # variable substitution
        x * x = {x * x}

        {% end %}                   # end block statement

        {% if len(x) == 1: %}       # start block statement
        x has 1 item.
        {%+ elif len(x) > 1: +%}    # continue block statement
        x has more than 1 item.
        {%+ else: +%}               # continue block statement
        x has no items.
        {% end %}                   # end block statement

    :Note: Subclasses may override the default pattern.
    """

    # regex patterns
    # NOTE: they must all differ from each other
    pattern_var = '{(.+?)}'                # {var}
    pattern_blockstart = '{% (.+?) %}'     # {% start statement %}
    pattern_blockcont = '{%\+ (.+?) \+%}'  # {%+ suite statement +%}
    pattern_blockend = '{% end %}'         # {% end %}
    pattern_oneliner = '{%% (.+?) %%}'     # {%% statement %%}
    pattern_comments = '#.*'               # comment

    def __init__(self, text):
        """
        Default initializer.

        :param text: The template text.
        :type text: str
        """
        self.text = text
        self.logger = logging.getLogger('Template')
        self._compile_regexs()

    def _compile_regexs(self):
        self._var_re = re.compile(self.pattern_var)
        self._blockstart_re = re.compile(
            r'\s*{}\s*(?:{})*'.format(
                self.pattern_blockstart, self.pattern_comments))
        self._blockcont_re = re.compile(
            r'\s*{}\s*(?:{})*'.format(
                self.pattern_blockcont, self.pattern_comments))
        self._blockend_re = re.compile(
            r'\s*{}\s*(?:{})*'.format(
                self.pattern_blockend, self.pattern_comments))
        self._oneline_stmt_re = re.compile(
            r'\s*{}\s*(?:{})*'.format(
                self.pattern_oneliner, self.pattern_comments))

    def render(self, context):
        """
        Render the Template.

        :param `context`: Variables needed in the template.
        :type ``context`: Mapping[str, object]

        :return: The rendered form of the remplate.
        :rtype: str
        """
        self.logger.debug('started rendering of text: %s', self.text)

        result_list_name = '_result_'
        while result_list_name in context:
            result_list_name += random_name(3)

        _code = self._preprocess(self.text, result_list_name=result_list_name)
        result_lines = []
        exec_(_code, context, {result_list_name: result_lines})
        result = '\n'.join(result_lines)

        self.logger.debug('finished rendering of text: %s', self.text)
        self.logger.debug('rendered result: %s', result)

        return result

    def _preprocess(self, text, indent=' '*4, result_list_name='_result_'):
        logger = self.logger
        lines = text.splitlines()
        result_lines = []
        level = 0

        def repl(match, vars_log=[]):
            var_name = match.group(1)
            vars_log.append(var_name)
            i = var_name.find('!')
            if i > -1:
                # new style formatting !s and !r
                return '{%s}' % (var_name[i:])
            return '{}'

        self.logger.debug('started preprocessing of %d lines.', len(lines))

        for i, line in enumerate(lines, 1):
            match = self._oneline_stmt_re.match(line)
            if match:
                logger.debug('found a oneline_statement at line: %s', i)
                result_lines.append(indent*level + match.group(1))
                continue

            match = self._blockend_re.match(line)
            if match:
                logger.debug('found a block_end at line: %s', i)
                if level <= 0:
                    raise SyntaxError(
                        'block_end at line {} has no matching block_start.'
                            .format(i)
                        )
                level -= 1
                continue

            match = self._blockstart_re.match(line)
            if match:
                logger.debug('found a block_start at line: %s', i)
                result_lines.append(indent*level + match.group(1))
                level += 1
                continue

            match = self._blockcont_re.match(line)
            if match:
                logger.debug('found a block_continue at line: %s', i)
                result_lines.append(indent*(level-1) + match.group(1))
                continue

            match = self._var_re.search(line)
            if match:
                logger.debug('found variable substitutions at line: %s', i)
                _var_names = []
                _repl = partial(repl, vars_log=_var_names)
                _line = self._var_re.sub(_repl, line)
                result_lines.append(
                    '{indent}{_result_}.append("{}".format({}))'
                        .format(
                            escape_quotes(_line),
                            ', '.join(_var_names),
                            indent=indent*level,
                            _result_=result_list_name,
                        )
                    )
                continue

            # text-only line
            result_lines.append(
                '{indent}{_result_}.append("{}")'
                    .format(
                        escape_quotes(line),
                        indent=indent*level,
                        _result_=result_list_name,
                    )
                )

        result = '\n'.join(result_lines)

        self.logger.debug('finished preprocessing of %d lines.', len(lines))
        self.logger.debug('preprocessed result: %s', result)

        return result
