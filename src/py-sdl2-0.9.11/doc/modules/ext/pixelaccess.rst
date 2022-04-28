`sdl2.ext.pixelaccess` - Array-like Access to SDL Surface Contents
==================================================================

The :mod:`sdl2.ext.pixelaccess` module offers a number of methods for reading,
modifying, and copying the contents SDL surface objects.

In most cases, the Numpy-based :func:`~sdl2.ext.pixels2d`,
:func:`~sdl2.ext.pixels3d`, and :func:`~sdl2.ext.surface_to_ndarray` functions
are the fastest and most flexible way of directly accessing the pixels of an
SDL surface. However, the pure-Python :class:`~sdl2.ext.PixelView` class can
be used instead to avoid adding Numpy as a dependency for your project.

.. automodule:: sdl2.ext.pixelaccess
   :members:
