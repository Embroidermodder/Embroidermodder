#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
==================================
|module_summary| hacks.__init__.py
==================================

TOWRITE

Functions summary:
==================

============================== ====================================
Widget hacks
============================== ====================================
:py:func:`~ForceRepaint`       Force an immediate full paint
                               refresh/update.
============================== ====================================

============================== ====================================
Overload hacks
============================== ====================================
:py:func:`~signature`          Decorator function to annotate 
                               functions with types signature.
:py:func:`~match_signature`    Dynamically match arguments to the
                               corresponding types signature.
:py:func:`~overloaded`         Decorator function to simplify
                               function overloading in python.
============================== ====================================


"""




from functools import wraps
from inspect import getargspec
from collections import Mapping


def ForceRepaint(widget):
    """
    Force an immediate full paint refresh/update.

    :note: This is rather HACK-ish. Qt should have a built-in way to do this...?
    :TODO: Figure out which ForceRefresh method is the least expensive performance-wise.
    """
    # Method1
    widget.hide()
    widget.show()

    # ...or this works also...
    # ... Not so well on wx when the frame is fullscreen/maximized. May need to be (-1, -1)

    # Method2
    # HACK: Take that QMdiArea!
    ## hack = widget.size()
    ## widget.resize(hack + QSize(1, 1))
    ## widget.resize(hack)


######################################################
# Helper utilities for function overloading in python.
######################################################


def signature(*types):
    """
    Decorator function to annotate functions with types signature.

    :param types: types of function arguments in order.
    :type types: tuple[type]
    """
    def wrapper(function):
        function._signature_ = types
        return function

    return wrapper


def match_signature(args, signatures, strict=False):
    """
    Dynamically match arguments to the corresponding types signature.

    Return matching types signature from `signatures` to the
    given types of arguments from `args` by inspecting their types
    dynamically, or None if no match.

    :param `args`: Sequence of function arguments.
    :type `args`: Sequence[object]

    :param `signatures`: Sequence of type signatures.
    :type `signatures`: Sequence[Sequence[type]]

    :param `strict`: If set to False, try to keep duck-typing support
        as much as possible.
    :type `strict`: bool

    :return: matched signature
    :rtype: type
    """
    # check for duplicate signatures
    if len(set(signatures)) != len(signatures):
        raise TypeError("Duplicate signatures")

    matched_signatures = []
    # check number of arguments
    for sig in signatures:
        if len(sig) == len(args):
            matched_signatures.append(sig)

    if not matched_signatures:
        return

    if not strict and len(matched_signatures) == 1:
        return matched_signatures[0]

    signatures = matched_signatures
    # check types of arguments
    for sig in signatures:
        for arg, type_ in zip(args, sig):
            if not isinstance(arg, type_):
                break
        else:
            return sig

    if not strict:
        for sig in signatures:
            for arg, type_ in zip(args, sig):
                try:
                    type_(arg)
                except (ValueError, TypeError):
                    break
            else:
                return sig


invalid_sig_msg_template = """\
{func.__name__!r} called with wrong argument types:
  {func.__name__}{callsig}
Supported signatures:
  {validsig}
"""


def overloaded(*args, **kwargs):
    """
    Decorator function to simplify function overloading in python.

    :param `args`: Mapping of {function: type signature}.
        or functions decorated with :py:func:`signature` decorator.
    :type `args`: Mapping or functions

    :param `strict`: keyword only argument; If set to False,
        try to keep duck-typing support as much as possible.
    :type `strict`: bool

    A simple use case would be:

    .. code-block:: python

        def func0(x, y):
            return 0

        def func1(s):
            return 1

        @overloaded({func0: (float, float), func1: (str,)})
        def func(*args):
            pass

        assert func(1.0, 2,0) == 0
        assert func("") == 1

    Or:

    .. code-block:: python

        @signature(float, float)
        def func0(x, y):
            return 0

        @signature(str)
        def func1(s):
            return 1

        @overloaded(func0, func1)
        def func(*args):
            pass

        assert func(1.0, 2,0) == 0
        assert func("") == 1

    :NOTE: It currently does not check keyword arguments.

    .. WARNING:: Function overloading is not needed in python. This should
        only be used for compatibilty and as a temporary solution.
    """

    strict = kwargs.get('strict', False)
    arg_error_msg = ("Arguments to 'overloaded' must be either a Maping of "
                     "{function: type_signature}, or functions.")
    if not args:
        raise TypeError(arg_error_msg)

    is_mapping = False
    if len(args) == 1:
        func_or_map = args[0]
        if isinstance(func_or_map, Mapping):
            if not func_or_map:
                raise TypeError(arg_error_msg)

            is_mapping = True
            func_sig_map = func_or_map
        elif not callable(func_or_map):
            raise TypeError(arg_error_msg)

    # Implementation detail.
    if is_mapping:
        sig_func_map = {sig: func for func, sig in func_sig_map.items()}
    else:
        sig_func_map = {}
        for func in args:
            if not callable(func):
                raise TypeError(arg_error_msg)

            try:
                sig_func_map[func._signature_] = func
            except AttributeError:
                raise TypeError("function passed to 'overloaded' must be "
                                "decorated with 'signature' decorator "
                                "or have _signature_ attribute.")

    def wrapper(function):

        @wraps(function)
        def wrapped_func(*args, **kwargs):
            _args = args  # backup

            #TODO/FIXME# lousy hack to detect methods; needs improvement.
            argspec = getargspec(function)
            if argspec.args and argspec.args[0] == 'self':
                args = args[1:]

            matched_sig = match_signature(args, sig_func_map.keys(), strict=strict)

            if matched_sig is None:
                func = wrapped_func
                callsig = '(%s)' % ', '.join(type(arg).__name__ for arg in args)

                valid_signatures = []
                for sig in sig_func_map.keys():
                    typesig = '(%s)' % ', '.join(type_.__name__ for type_ in sig)
                    valid_signatures.append("%s%s" % (func.__name__, typesig))

                validsig = "\n  ".join(valid_signatures)

                error_msg = invalid_sig_msg_template.format(**locals())
                raise TypeError(error_msg)

            matched_func = sig_func_map[matched_sig]
            return matched_func(*_args, **kwargs)

        return wrapped_func

    return wrapper
