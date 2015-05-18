#!/usr/bin/env python2

import os
import sys
import logging
import argparse
from lib2to3.main import StdoutRefactoringTool

from astwrapper import ClangASTWrapper


def main(argv):
    parser = argparse.ArgumentParser(
        description='Add missind docstrings to functions missing them,'
                    ' or fill sparse docstrings.')
    pygroup = parser.add_mutually_exclusive_group()
    pygroup.add_argument('-pydir', '--python-source-dir',
                         help='directory containing python source files') 
    pygroup.add_argument('-pyfiles', '--python-source-files', nargs='*',
                         help='python files to process')
    parser.add_argument('-pyfpatt', '--python-source-pattern', default='*.py',
                        help='python files unix shell like pattern. ex: *.py')
    parser.add_argument('-W', '--wrapper', choices=['xml', 'clang'],
                        default='clang', help='AST wrapper to use.')
    # xml wrapper
    xmlgroup = parser.add_argument_group('xml')
    xmlgroup0 = xmlgroup.add_mutually_exclusive_group()
    xmlgroup0.add_argument('-xdir', '--xml-files-dir',
                           help='directory containing xml files')
    xmlgroup0.add_argument('-xfiles', '--xml-files', nargs='*',
                           help='xml files that contain AST')
    # clang wrapper
    clanggroup = parser.add_argument_group('clang')
    cppgroup = clanggroup.add_mutually_exclusive_group()
    cppgroup.add_argument('-cppdir', '--cpp-source-dir',
                          help='directory containing c++ source files')
    cppgroup.add_argument('-cppfiles', '--cpp-source-files', nargs='*',
                          help='C++ files to parse')

    clanggroup.add_argument('-cppfpatt', '--cpp-source-pattern', default='*.h',
                            help='C++ files unix shell like pattern. ex: *.h')
    clanggroup.add_argument('-C', '--clang-args', nargs='*', default=('-xc++',),
                            help='arguments passed to clang')
    # refactoring
    refactorgroup = parser.add_argument_group('refactoring')
    refactorgroup.add_argument('-w', '--write', action='store_true',
                               help='write modifications to files')
    refactorgroup.add_argument('-o', '--output-dir', default='',
                               help='save output files in this directory')
    refactorgroup.add_argument('--add-suffix', default='',
                               help='append this string to all output filenames')
    refactorgroup.add_argument('-m', '--mode', default='missing',
                               choices=['missing', 'sparse', 'all'],
                               help='docstring refactoring mode. '
                                    'default is  missing. '
                                    'missing: only refactor when missing'
                                    ' docstring. '
                                    'sparse: replace sparse docstrings. '
                                    'all: replace all docstrings.')
    # verbosity
    parser.add_argument('-v', '--verbosity', action='count', default=0,
                        help='increase output verbosity. ex: -v, -vv')
    # parse
    args = parser.parse_args(argv)

    if args.verbosity >= 2:
        level = logging.DEBUG
    else:
        level = logging.INFO

    logging.basicConfig(format='%(name)s: %(message)s', level=level)
    logger = logging.getLogger('docgen.main')

    using_defaults = False
    if not args.python_source_dir:
        args.python_source_dir = os.path.abspath('../../../gui')
        using_defaults = True
    if args.wrapper == 'clang':
        if not args.cpp_source_dir:
            args.cpp_source_dir = os.path.abspath('../../../../../embroidermodder2')
            using_defaults = True

        if using_defaults:
            if args.clang_args == ('-xc++',):
                args.clang_args = [
                    "-x", "c++",

                    # qt includes
                    "-I/usr/lib/qt/mkspecs/linux-clang",
                    "-isystem", "/usr/include/qt",
                    "-isystem", "/usr/include/qt/QtOpenGL",
                    "-isystem", "/usr/include/qt/QtPrintSupport",
                    "-isystem", "/usr/include/qt/QtWidgets",
                    "-isystem", "/usr/include/qt/QtGui",
                    "-isystem", "/usr/include/qt/QtScript",
                    "-isystem", "/usr/include/qt/QtScriptTools",
                    "-isystem", "/usr/include/qt/QtCore",

                    # local includes
                    "-I" + args.cpp_source_dir,
                    "-I%s/javascript" % args.cpp_source_dir,
                    "-I%s/../libembroidery" % args.cpp_source_dir,
                ]

        wrapper = ClangASTWrapper(
            args.cpp_source_dir, args.cpp_source_files, args.cpp_source_pattern,
            clangargs=args.clang_args, pythonify_types=True)
    else:
        raise NotImplementedError('XmlASTWrapper is not implemented yet.')

    options = {
        'funcmeta_provider': wrapper,
        'srcdir': args.python_source_dir,
        'mode': args.mode,
        }
    rt = StdoutRefactoringTool(
        ["fix_defdocs"], options, [], True, True, args.python_source_dir,
        args.output_dir, args.add_suffix)

    if args.python_source_files:
        logger.info('Starting refactoring %d files', len(args.python_source_files))
        logger.debug('files: %s', args.python_source_files)
        rt.refactor(args.python_source_files, args.write)
    else:
        logger.info('Starting refactoring directory: %s', args.python_source_dir)
        rt.refactor([args.python_source_dir], args.write)

    rt.summarize()

    return int(bool(rt.errors))


if __name__ == '__main__':
    # sys.exit(main(['-pyfiles', '../../../gui/object_base.py', '-cppfiles', '../../../../../embroidermodder2/object-base.h']))
    sys.exit(main(sys.argv[1:]))
