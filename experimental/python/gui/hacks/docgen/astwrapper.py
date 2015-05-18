"""
AST (Abstract Syntax Tree) wrapper focused on function definitions.

This module provides a universal pythonic wrappers
for an AST (Abstract Syntax Tree).

It is currently focused on function/method definitions only.
"""


# TODO list:
# 1. Use caching/threading to optimize lookup speed.
# 2. Implement the sparse classes.
# 3. Documentation...
# 4. Optimizations.
# 5. PEP8, PEP257


import os.path
from itertools import chain
from collections import namedtuple

from ctypes import c_int, c_uint
from clang.cindex import conf as clang_conf
from clang.cindex import register_function as clang_register_function
from clang.cindex import Index, AccessSpecifier, Type, TypeKind, CursorKind

from utilities import glob_recursive

__all__ = [
    'Signature',
    'Argument',
    'Function',
    'BaseASTWrapper',
    'XmlASTWrapper',
    'ClangASTWrapper',
]

Signature = namedtuple('Signature', 'return_type, arguments')
Argument = namedtuple('Argument', 'name, type')


class Function(object):

    """ Function object. """

    # Performance optimization.
    __slots__ = ('_name', '_flags', '_arguments', '_rtype', '_class', '_file')

    # Flags enumeration
    NONE = 0  # this should evaluate to False
    PUBLIC = 1
    PRIVATE = 2
    PROTECTED = 4
    STATIC = 8
    VIRTUAL = 16
    PUREVIRTUAL = 32
    SIGNAL = 64
    SLOT = 128
    METHOD = 256
    CONSTRUCTOR = 512
    DESTRUCTOR = 1024

    def __init__(self,
                 name, flags=0, class_=None, filepath=None,
                 return_type=None, arguments=()):
        """
        Function default constructor.

        :param name: Function name.
        :type name: str

        :param flags: Function flags.
        :type flags: int

        :param class_: Parent class.
        :type class_: str

        :param filepath: Location source file.
        :type filepath: str

        :param return_type: Funtion return type.
        :type return_type: str

        :param arguments: Funtion arguments.
        :type arguments: Sequence[Argument]
        """
        self._name = name
        self._flags = flags
        self._class = class_
        self._file = filepath
        self._rtype = return_type
        self._arguments = arguments

    @property
    def name(self):
        """
        Function name.

        :rtype: str
        """
        return self._name

    @property
    def return_type(self):
        """
        Function return type.

        :rtype: str
        """
        return self._rtype

    @property
    def arguments(self):
        """
        Function arguments.

        :rtype: str
        """
        return self._arguments

    @property
    def class_(self):
        """
        Parent Class name.

        :rtype: str
        """
        return self._class

    @property
    def file(self):
        """
        Location source file.

        :rtype: str
        """
        return self._file

    @property
    def flags(self):
        """
        Function flags.

        :rtype: int
        """
        return self._flags

    @property
    def signature(self):
        """
        Function signature.

        :rtype: Signature[str, tuple[Argument]]
        """
        return Signature(self._rtype, tuple(self._arguments))

    def __getitem__(self, key):
        """ Implement mapping like protocol. """
        if key == 'name':
            return self.name
        elif key == 'return_type':
            return self.return_type
        elif key == 'arguments':
            return self.arguments
        elif key == 'class':
            return self.class_
        elif key == 'file':
            return self.file
        elif key == 'flags':
            return self.flags
        elif key == 'signature':
            return self.signature
        else:
            raise KeyError(
                'Invalid key {}.\n\nValid keys are:\n  '
                'name, return_type, arguments, class, file, flags, signature'
                    .format(key)
                )

    @property
    def is_method(self):
        """
        Check if function is a a method.

        :rtype: bool
        """
        return self.METHOD & self._flags

    @property
    def is_public(self):
        """
        Check if function is a public method.

        :rtype: bool
        """
        return self.PUBLIC & self._flags

    @property
    def is_private(self):
        """
        Check if function is a private method.

        :rtype: bool
        """
        return self.PRIVATE & self._flags

    @property
    def is_protected(self):
        """
        Check if function is a protected method.

        :rtype: bool
        """
        return self.PROTECTED & self._flags

    @property
    def is_static(self):
        """
        Check if function is a static method.

        :rtype: bool
        """
        return self.STATIC & self._flags

    @property
    def is_virtual(self):
        """
        Check if function is a virtual method.

        :rtype: bool
        """
        return self.VIRTUAL & self._flags

    @property
    def is_purevirtual(self):
        """
        Check if function is a pure virtual method.

        :rtype: bool
        """
        return self.PUREVIRTUAL & self._flags

    @property
    def is_slot(self):
        """
        Check if function is a Qt slot.

        :rtype: bool
        """
        return self.SLOT & self._flags

    @property
    def is_signal(self):
        """
        Check if function is a Qt signal.

        :rtype: bool
        """
        return self.SIGNAL & self._flags


class BaseASTWrapper(object):

    """Optional base class for Provider."""

    def iter_functions(self, name=None, class_=None, srcfile=None, flags=None, signature=None):
        """
        Return Iterable of functions matching given crieteria.

        If no matched criteria, return None.
        If no criteria given, return Iterable of all functions.

        :param name: Function name.
        :type name: str

        :param class_: Declaring Class.
        :type class_: str

        :param srcfile: Source file path.
        :type srcfile: str

        :param flags: Function FLAGS.
        :type flags: int

        :param signature: Function signature.
        :type signature: Signature[str, tuple[Argument]]

        :rtype: Iterable[Function]
        """
        raise NotImplementedError


class XmlASTWrapper(BaseASTWrapper):

    """
    Provider using AST from xml file(s).

    :TODO: Not yet implemented.
    """

    # TODO
    def __new__(cls):
        raise NotImplementedError


class ClangASTWrapper(BaseASTWrapper):

    """ Provider using clang python library. """

    # Map of Typekinds to their printable form.
    _typekinds_repr_map = {
        TypeKind.VOID:                'void',
        TypeKind.BOOL:                'bool',
        TypeKind.CHAR_U:              'char',
        TypeKind.UCHAR:               'unsigned char',
        TypeKind.CHAR16:              'char16_t',
        TypeKind.CHAR32:              'char32_t',
        TypeKind.USHORT:              'unsigned short',
        TypeKind.UINT:                'unsigned int',
        TypeKind.ULONG:               'unsigned long',
        TypeKind.ULONGLONG:           'unsigned long long',
        TypeKind.UINT128:             '__uint128_t',
        TypeKind.CHAR_S:              'char',
        TypeKind.SCHAR:               'signed char',
        TypeKind.WCHAR:               'wchar_t',
        TypeKind.SHORT:               'short',
        TypeKind.INT:                 'int',
        TypeKind.LONG:                'long',
        TypeKind.LONGLONG:            'long long',
        TypeKind.INT128:              '__int128_t',
        TypeKind.FLOAT:               'float',
        TypeKind.DOUBLE:              'double',
        TypeKind.LONGDOUBLE:          'long double',
        TypeKind.COMPLEX:             'complex',
        TypeKind.CONSTANTARRAY:       '[]',
        TypeKind.VECTOR:              'std.vector',
        TypeKind.INCOMPLETEARRAY:     '[]',
        TypeKind.VARIABLEARRAY:       '[]',
        TypeKind.DEPENDENTSIZEDARRAY: '[]',
    }

    # Map of types to their corresponding alternative python types.
    # NOTE: This mapping contains suggestions to what can be used alternatively.
    types_conv_map = {
        # standard types
        'void':                       None,  # this should evaluate to False
        'bool':                       'bool',
        'char':                       'bytes',
        'unsigned char':              'bytes',
        'char16_t':                   'bytes',
        'char32_t':                   'bytes',
        'unsigned short':             'int',
        'unsigned int':               'int',
        'unsigned long':              'int',
        'unsigned long long':         'int',
        '__uint128_t':                'int',
        'signed char':                'bytes',
        'wchar_t':                    'str',
        'short':                      'int',
        'int':                        'int',
        'long':                       'int',
        'long long':                  'int',
        '__int128_t':                 'int',
        'float':                      'float',
        'double':                     'float',
        'long double':                'int',
        'complex':                    'complex',
        '[]':                         'list',

        # C++
        'std.string':                 'str',
        'std.wstring':                'str',
        'std.complex':                'complex',
        'std.tuple':                  'tuple',
        'std.pair':                   'tuple',
        'std.map':                    'dict',  # XXX: replace with OrderedDict?
        'std.multimap':               'dict',  # there's no multimaps in python
        'std.array':                  'list',
        'std.deque':                  'deque',  # dynamic array deque
        'std.forward_list':           'list',   # linked list
        'std.list':                   'deque',  # doubly linked list
        'std.queue':                  'deque',
        'std.priority_queue':         'heapq',
        'std.set':                    'set',
        'std.multiset':               'set',    # there's no multisets in python
        'std.stack':                  'list',   # there's no multimaps in python
        'std.unordered_map':          'dict',
        'std.unordered_multimap':     'dict',   # there's no multimaps in python
        'std.unordered_set':          'set',
        'std.unordered_multiset':     'set',    # there's no multisets in python
        'std.vector':                 'list',
        'std.bitset':                 'list[bool]',

        # Qt types
        'qint8':                      'int',
        'qint16':                     'int',
        'qint32':                     'int',
        'quint8':                     'int',
        'quint16':                    'int',
        'quint32':                    'int',
        'quint64':                    'int',
        'qreal':                      'float',
        'qint64':                     'int',
        'qlonglong':                  'int',
        'qulonglong':                 'int',
        'QBool':                      'bool',
        'QString':                    'str',
        'QStringRef':                 'str',
        'QChar':                      'str',
        'QVariant':                   'object',
        'QVariant.Type':              'type',
        'QVariantMap':                'dict',
        'QStringList':                'list',
        'QModelIndexList':            'list',
        'QSet':                       'set',
        'QList':                      'list',
        'QVector':                    'list',
        'QStack':                     'list',
        'QQueue':                     'deque',
        'QLinkedList':                'deque',
        'QHash':                      'dict',
        'QMap':                       'dict',
        'QMultiMap':                  'dict',
        'QPair':                      'tuple',
        'QRgb':                       'int',
    }

    def __init__(self,
                 srcdir, files=None, filespattern="*.h",
                 clangargs=('-x', 'c++'), pythonify_types=True):
        """
        Default class constructor.

        :param `srcdir`: C/C++/ObjC source files directory path.
        :type `srcdir`: str

        :param `files`: C/C++/ObjC source files paths.
        :type `files`: str

        :param `clangargs`: Agruments passed to clang.
        :type `clangargs`: Sequence[str]

        :param `filespattern`: Unix shell like pattern for soure files
        :type `filespattern`: str

        :param `pythonify_types`: If set to True replace clang types
            with their python alternatives.
        :type `pythonify_types`: bool
        """
        self.srcdir = srcdir
        self.clangargs = clangargs
        self.pythonify_types = pythonify_types

        if files is not None:
            self.fileslist = list(files)
        else:
            self.fileslist = glob_recursive(srcdir, filespattern)

        self._index = Index.create()
        self._pool = {}
        self._searchindex = {}

    def _get_functions_byfilepath(self, filepath):
        srcdir, pool = self.srcdir, self._pool

        if filepath in pool:
            funcs = pool[filepath]
        else:
            funcs = pool[filepath] = \
                self._parse_sourcefile(filepath)

        return funcs

    def _parse_sourcefile(self, filepath):
        clangargs, index = self.clangargs, self._index
        pythonify_types = self.pythonify_types

        tu = index.parse(filepath, clangargs)

        if pythonify_types:
            type_repr = self.clang_Type_pythonify
        else:
            def type_repr(type_):
                return type_.spelling

        funcs_pool = []
        for funcdecl in self.clang_Cursor_iter_funcdecls(tu.cursor):
            funcs_pool.append(
                self.function_from_clang_cusrsor(funcdecl, type_repr))

        return funcs_pool

    @staticmethod
    def clang_Cursor_is_funcdecl(cursor):
        """
        Check if clang cursor node is a function declaration.

        :type cursor: clang.cindex.Cursor
        :rtype: bool
        """
        return cursor.kind in (
            CursorKind.FUNCTION_DECL,
            CursorKind.CXX_METHOD,
            CursorKind.CONSTRUCTOR,
            CursorKind.DESTRUCTOR,
            CursorKind.OBJC_INSTANCE_METHOD_DECL,
            CursorKind.OBJC_CLASS_METHOD_DECL,
        )

    @staticmethod
    def clang_Cursor_is_scopedecl(cursor):
        """
        Check if clang cursor node is a "scope declaration".

        A "scope declaration" is one the following:
            - C++ Class declaration.
            - C++ Namespace declaration.
            - C/C++ Struct declaration.
            - C/C++ Union declaration.
            - Enum declaration.
            - C++ Class Template declaration.
            - ObjC @interface declaration.
            - ObjC @protocol declaration.

        :type cursor: clang.cindex.Cursor
        :rtype: bool
        """
        return cursor.kind in (
            CursorKind.STRUCT_DECL,
            CursorKind.UNION_DECL,
            CursorKind.CLASS_DECL,
            CursorKind.ENUM_DECL,
            CursorKind.OBJC_INTERFACE_DECL,
            CursorKind.OBJC_CATEGORY_DECL,
            CursorKind.OBJC_PROTOCOL_DECL,
            CursorKind.NAMESPACE,
            CursorKind.CLASS_TEMPLATE,
            CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION,
        )

    @staticmethod
    def clang_Cursor_is_template_class(cursor):
        """
        Check if clang cursor node is a template class declaration.

        :type cursor: clang.cindex.Cursor
        :rtype: bool
        """
        return bool(
            clang_conf.lib.clang_getSpecializedCursorTemplate(cursor)
        )

    @staticmethod
    def clang_Cursor_iter_funcdecls(cursor):
        """
        Yield all cursor nodes that represent a function declaration.

        :param cursor: Root node cursor.
        :type cursor: clang.cindex.Cursor

        :rtype: Iterable[clang.cindex.Cursor]
        """
        if ClangASTWrapper.clang_Cursor_is_funcdecl(cursor):
            if cursor.location.file is not None and \
                    cursor.location.file.name == cursor.translation_unit.spelling:
                yield cursor

        # Recurse all children of this node
        for child in cursor.get_children():
            for cnode in ClangASTWrapper.clang_Cursor_iter_funcdecls(child):
                yield cnode

    @staticmethod
    def clang_Cursor_iter_semantic_parents(cursor, order='post'):
        """
        Yield all semantic parents of cursor node in given order.

        :param cursor: The Cursor node to process.
        :type cursor: clang.cindex.Cursor

        :param order: The order of iteration; Either 'pre' or 'post'.
        :type order: str

        :rtype: Iterable[clang.cindex.Cursor]
        """
        def _iter_post_order(cursor):
            s_parent = cursor.semantic_parent
            while s_parent:
                yield s_parent
                s_parent = s_parent.semantic_parent

        if order == 'post':
            return _iter_post_order(cursor)
        elif order == 'pre':
            return reversed(list(_iter_post_order(cursor)))
        else:
            raise ValueError("Argument 'order' must either be 'pre' or 'post'")

    @staticmethod
    def clang_Type_pythonify(type_, convertion_map=types_conv_map):
        """
        Return a pythonic name of the given Type.

        If type is not a valid Type or has no representaion, return None.
        Name convertion is based on 'convertion_map'.

        :param type_: The Type to process.
        :type type_: clang.cindex.Type

        :param convertion_map: The mapping used to convert types names
            to their python equivalents.
        :type convertion_map: Mapping[str, str]

        :rtype: str
        """
        # TODO: refactor code.

        if type_.kind == TypeKind.INVALID:
            return

        pointer_level = 0
        pointee = type_.get_pointee()
        while pointee.kind != TypeKind.INVALID:
            if type_.kind == TypeKind.POINTER:
                pointer_level += 1

            type_ = pointee
            pointee = type_.get_pointee()

        type_decl = type_.get_declaration()

        # Check for builtin types
        if type_decl.kind == CursorKind.NO_DECL_FOUND:
            if type_.kind in ClangASTWrapper._typekinds_repr_map:
                type_name = ClangASTWrapper._typekinds_repr_map[type_.kind]

                # Check if it is a pointer
                if pointer_level > 0:
                    # check if a char type, i.e. a char pointer
                    # which is often a string or array of strings.
                    if 'char' in type_name:
                        if pointer_level > 1:
                            type_name = 'tuple'  # array of strings
                        elif type_name in convertion_map:
                            # str or bytes
                            type_name = convertion_map[type_name]
                        else:
                            type_name = 'str'  # default to str
                    elif type_name == 'void':  # check for void pointer
                        type_name = 'object'
            else:
                type_name = None
        # Check for bugs
        elif not type_decl.displayname:
            raise TypeError(
                'Type declaration Cursor {} has no displayname.'
                    .format(type_decl))
        # Check for templates
        elif ClangASTWrapper.clang_Cursor_is_template_class(type_decl):
            return ClangASTWrapper.clang_Type_template_pythonify(type_)
        # ...
        else:
            _name_components = [type_decl.displayname]
            s_parents = \
                ClangASTWrapper.clang_Cursor_iter_semantic_parents(type_decl, 'post')
            for parent in s_parents:
                if not ClangASTWrapper.clang_Cursor_is_scopedecl(parent):
                    break

                _name_components.append(parent.spelling)

            type_name = '.'.join(reversed(_name_components))

        # Get python equivalent
        type_name = convertion_map.get(type_name, type_name)

        return type_name

    # expose unexposed clang functions
    _clang_functions_data_ = (

        ("clang_Type_getNumTemplateArguments",
         [Type],
         c_int),

        ("clang_Type_getTemplateArgumentAsType",
         [Type, c_uint],
         Type,
         Type.from_result),

    )

    for _func_data_ in _clang_functions_data_:
        clang_register_function(clang_conf.lib, _func_data_, False)

    del _func_data_, _clang_functions_data_

    @staticmethod
    def clang_Type_template_pythonify(template_type, convertion_map=types_conv_map):
        """
        Return a pythonic name of the given Type.

        If type is not a valid Type or has no representaion, return None.

        :param type_: The template Type to process.
        :type type_: clang.cindex.Type

        :param convertion_map: The mapping used to convert types names
            to their python equivalents.
        :type convertion_map: Mapping[str, str]

        :rtype: str
        """
        type_decl = template_type.get_declaration()

        # Check for bugs
        if type_decl.kind == CursorKind.NO_DECL_FOUND:
            raise TypeError(
                'No declaration found for Type {}; Check the source code.'
                    .format(template_type))

        # Check if this is actually a template
        if not ClangASTWrapper.clang_Cursor_is_template_class(type_decl):
            return ClangASTWrapper.clang_Type_pythonify(template_type)

        _name_components = [type_decl.spelling]
        s_parents = \
            ClangASTWrapper.clang_Cursor_iter_semantic_parents(type_decl, 'post')
        for parent in s_parents:
            if not ClangASTWrapper.clang_Cursor_is_scopedecl(parent):
                break

            _name_components.append(parent.spelling)

        _name = '.'.join(reversed(_name_components))

        # Get python equivalent
        _name = convertion_map.get(_name, _name)

        num_args = clang_conf.lib.clang_Type_getNumTemplateArguments(
            template_type)

        args_as_types = []
        for i in range(num_args):
            arg_type = clang_conf.lib.clang_Type_getTemplateArgumentAsType(
                template_type, i)
            if arg_type.kind != TypeKind.INVALID:
                args_as_types.append(arg_type)

        args_names = []
        for arg_type in args_as_types:
            arg_type_name = ClangASTWrapper.clang_Type_pythonify(arg_type)
            if not arg_type_name:
                return

            args_names.append(arg_type_name)

        if args_names:
            return '{}[{}]'.format(_name, ', '.join(args_names))
        else:
            return _name

    @staticmethod
    def function_from_clang_cusrsor(cursor, type_repr=clang_Type_pythonify):
        """
        Build Function object from clang cursor.

        :param cursor: Clang cursor representing the function declaration.
        :type cursor: `Cursor`

        :param type_repr: Function that takes a Type an returns it's representation.
        :type type_repr: Callable(Type) -> str

        :rtype: `Function`
        """
        if not ClangASTWrapper.clang_Cursor_is_funcdecl(cursor):
            raise TypeError('Cursor %s is not a function declaration' % cursor.kind)

        name = cursor.spelling
        flags = Function.NONE
        class_ = None
        return_type = None
        arguments = []
        filepath = cursor.location.file.name if cursor.location.file else None

        if cursor.kind in (
                CursorKind.CXX_METHOD,
                CursorKind.OBJC_INSTANCE_METHOD_DECL,
                CursorKind.OBJC_CLASS_METHOD_DECL,
                ):
            flags += Function.METHOD
            class_ = cursor.semantic_parent.spelling
        elif cursor.kind == CursorKind.CONSTRUCTOR:
            flags += Function.METHOD
            flags += Function.CONSTRUCTOR
            class_ = cursor.semantic_parent.spelling
        elif cursor.kind == CursorKind.DESTRUCTOR:
            flags += Function.METHOD
            flags += Function.DESTRUCTOR
            class_ = cursor.semantic_parent.spelling

        accspecif = cursor.access_specifier
        if accspecif == AccessSpecifier.PUBLIC:
            flags += Function.PUBLIC
        elif accspecif == AccessSpecifier.PRIVATE:
            flags += Function.PRIVATE
        elif accspecif == AccessSpecifier.PROTECTED:
            flags += Function.PROTECTED

        if cursor.is_static_method():
            flags += Function.STATIC

        # TODO: check for the left flags

        return_type = type_repr(cursor.result_type)

        for arg in cursor.get_arguments():
            argname = arg.spelling
            argtype = type_repr(arg.type)
            arguments.append(Argument(argname, argtype))

        return Function(name, flags, class_, filepath, return_type, arguments)

    @staticmethod
    def filter_functions(functions, key, value):
        """
        Filter by key.

        :NOTE: If `key` is 'signature', fields in the given
            signature `value` that are None are ignored,
            thus partial match is performed.

        :param functions: Functions to filter.
        :type functions: Iterable[Function]

        :param key: Key to filter by.
            Valid keys are:
                "name", "class", "file", "flags", "signature".
        :type key: str

        :param value: Value to be matched.
        :type value: str or int or Signature[str, tuple[Argument]]

        :rtype: Iterable[Function]
        """
        # TODO: refactor code.

        if key == 'signature':
            def comp(a, b):
                """ Compare for equality and ignore None values. """
                if None in (a, b) or a == b:
                    return True
                return False

            # value type is Signature[str, tuple[Argument]]
            v_rtype = value[0]
            v_args = value[1]
            for func in functions:
                rtype, args = func[key]
                # compare number of arguments
                if len(v_rtype) != len(args):
                    continue

                # compare return types
                if not comp(v_rtype, rtype):
                    continue

                # compare argument names and types
                if not all(map(comp, zip(chain(*v_args), chain(*args)))):
                    continue

                # found a match
                yield func
        elif key == 'flags':
            v_flags = value
            for func in functions:
                flags = func[key]
                if flags & v_flags == v_flags:
                    yield func
        else:
            for func in functions:
                if func[key] == value:
                    yield func

    def iter_functions(self,
                       name=None, class_=None, srcfile=None,
                       flags=None, signature=None):
        # TODO: use SearchIndex for faster lookup.
        fileslist = self.fileslist
        get_funcs = self._get_functions_byfilepath

        if srcfile:
            functions = get_funcs(srcfile)
        else:
            functions = chain.from_iterable(
                get_funcs(file_) for file_ in fileslist)

        if name:
            functions = self.filter_functions(functions, 'name', name)

        if class_:
            functions = self.filter_functions(functions, 'class', class_)

        if flags:
            functions = self.filter_functions(functions, 'flags', flags)

        if signature:
            functions = self.filter_functions(functions, 'signature', signature)

        return functions

    iter_functions.__doc__ = BaseASTWrapper.iter_functions.__doc__


class SearchIndex(object):

    """
    SearchIndex.

    :TODO: Not yet implemented.
    """

    # TODO
    def __new__(cls):
        raise NotImplementedError
