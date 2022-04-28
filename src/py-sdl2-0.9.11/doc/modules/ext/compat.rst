`sdl2.ext.compat` - Python Version Compatibility Helpers
========================================================

The :mod:`sdl2.ext.compat` module provides various helper functions for writing
code that works seamlessly on both Python 2.7 and Python 3.x.

.. data:: ISPYTHON2

   ``True``, if executed in a Python 2.x compatible interpreter, ``False``
   otherwise.

.. data:: ISPYTHON3

   ``True``, if executed in a Python 3.x compatible interpreter, ``False``
   otherwise.

.. automodule:: sdl2.ext.compat
   :exclude-members: long, unichr, unicode, UnsupportedError, ExperimentalWarning
   :members:
