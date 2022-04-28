.. currentmodule:: sdl2.ext

`sdl2.ext.image` - Importing and Exporting Image Files
======================================================

The :mod:`sdl2.ext.image` module provides some simple functions for importing
images as SDL surfaces and exporting SDL surfaces to image files.

The basic functions :func:`load_bmp` and :func:`save_bmp` load and save BMP
files, and are guaranteed to be available on all systems supported by PySDL2.
The :func:`load_img` function can be used to import additional image formats
(e.g. JPEG, PNG), but requires that the **SDL_image** library is installed on
the target system (can be installed as a Python dependency with ``pysdl2-dll``
on platforms that support it).

In addition to importing images from files, this module also provides the
:func:`pillow_to_surface` function for converting :obj:`Image` objects from the
`Pillow <https://pillow.readthedocs.io/en/stable/>`_ Python library to SDL
surfaces.

.. automodule:: sdl2.ext.image
   :members:
