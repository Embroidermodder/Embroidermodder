"""
Fixer for functions missing docstring.

This adds sphinx compatible docstrings to functions missing them,
taking metadata from :py:data:`astwrapper.Function` objects provided
by an instance of a subclass of :py:class:`astwrapper.BaseASTWrapper`,
which is passed as an option named 'funcmeta_provider' to the fixer.

If 'funcmeta_provider' option is not given a default docstring is used.

    
    def func(x):  ->  def func():
        pass              '''
                          TOWRITE
                          
                          :param x: TOWRITE
                          :type x: int
                          '''
                          pass
"""

# Python imports
from __future__ import print_function
from lib2to3 import pytree
from lib2to3 import fixer_base
from lib2to3.pgen2 import token
from lib2to3.pygram import python_symbols as syms
import re

# Local import
from astwrapper import Function
from utilities import Template


# TODO list:
# 1. Handle overloads
# 2. Format local references
# 3. Documentation

builtin_types = {
    'bool',
    'complex',
    'dict',
    'float',
    'generator',
    'int',
    'list',
    'object',
    'slice',
    'str',
    'tuple',
    'type',
}

docstring_template = Template('''\
{% if func.arguments or func.return_type: %}
"""
{indent}TOWRITE

{% for arg in func.arguments: %}
{indent}:param `{arg.name}`: TOWRITE
{indent}:type `{arg.name}`: {postprocess_typename(arg.type)}
{% end %}  # endfor
{% if func.return_type: %}
{indent}:return: TOWRITE
{indent}:rtype: {postprocess_typename(func.return_type)}
{% end %}  # endif
{indent}"""
{%+ else: +%}
""" TOWRITE """
{% end %}  # endif
''')


def get_doctsring(node):
    if (isinstance(node, pytree.Node) and
        # stmt is simplified to simple_stmt
        node.type == syms.simple_stmt and
        node.children[0].type == token.STRING):
        return node.children[0].value


def postprocess_typename(typename):
    def _process_name(typename):
        if typename in builtin_types or typename.startswith('Qt.'):
            return typename
        else:
            return "`{}`_".format(typename)

    if '[' in typename:  # template/generic
        tokens = re.split('([][, ])', typename)
        for i, tok in enumerate(tokens):
            if tok not in ' ,[]':
                tokens[i] = _process_name(tok)

        return ''.join(tokens)

    return _process_name(typename)


class FixDefdocs(fixer_base.BaseFix):
    BM_compatible = True

    order = "pre"

    # NOTE: this pattern only matches fucdedfs with suites of type:
    # NEWLINE INDENT stmt+ DEDENT
    # where stmt is simplified to simple_stmt.
    # The other type of suites is not matched because suite is then
    # simlified to simple_stmt.
    PATTERN = """
    funcdef< 'def'
             name=NAME
             args=parameters ['->' test] ':'
             body=suite >
    """

    def transform(self, node, results):
        syms = self.syms
        options = self.options

        mode= options.get('mode', False)
        func_provider = options.get('funcmeta_provider', None)

        body = results['body']
        name = results['name']
        args = results['args']

        # Skip Leafs (indents and linebreaks)
        newline, indent = '\n', ' ' * 4
        for i, child in enumerate(body.children):
            if child.type == token.NEWLINE:
                newline = child.value
            elif child.type == token.INDENT:
                indent = child.value

            if isinstance(child, pytree.Node):
                break

        docstring = get_doctsring(child)
        do_process = False

        # if no docstring
        if not docstring:
            do_process = True
        elif mode == 'sparse':
            # if docstring is sparse
            if eval(docstring).strip() in ('TOWRITE', ''):
                do_process = True
        elif mode == 'all':
            do_process = True

        if do_process:
            funcname = name.value
            classdef = None
            classname = None
            flags = Function.NONE
            srcfile = None

            # check whether this is a method
            _node = node
            while True:
                if _node.parent.type in (syms.decorated, syms.suite):
                    _node = _node.parent
                    continue

                if _node.parent.type == syms.classdef:
                    classdef = _node.parent
                    classname = classdef.children[1].value
                    flags += Function.METHOD

                break

            new_docstring = docstring or '""" TOWRITE """'
            if func_provider:
                funcs = list(
                    func_provider.iter_functions(
                        funcname, classname, srcfile, flags)
                    )

                if not funcs:
                    self.warning(
                        node,
                        'Function {} of class {} and file {} not found'
                            .format(funcname, classname, self.filename))
                    return

                if len(funcs) > 1:
                    self.warning(
                        node,
                        'Many mathes found for function {} of class {}'
                        ' and file {}'
                            .format(funcname, classname, self.filename))
                    return

                func = funcs[0]

                context = {
                    'func': func,
                    'indent': indent,
                    'postprocess_typename': postprocess_typename,
                }

                new_docstring = docstring_template.render(context)

            # insert tokens in tree
            if not docstring:
                # insert docstring at the beginnig if no one exists
                body.insert_child(0, pytree.Leaf(token.NEWLINE, newline))
                body.insert_child(1, pytree.Leaf(token.INDENT, indent))
                body.insert_child(2, pytree.Leaf(token.STRING, new_docstring))
            else:
                # else replace the old docstring with the new one
                node = child.clone()
                node.value = new_docstring
                body.set_child(i, node)
