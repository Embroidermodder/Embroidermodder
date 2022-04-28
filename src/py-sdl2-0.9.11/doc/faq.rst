.. _faq:

PySDL2 FAQ
==========
This is a list of Frequently Asked Questions about PySDL2. If there are any
questions and answers you would like added here, please let us know on GitHub!

On importing...
---------------
... my script fails and complains that a SDL2 library could not be found!
    
   Do you have the libraries properly installed? On most platforms, you can
   install them easily using ``pip install pysdl2-dll``. On ARM64 macOS, you
   can install native SDL2 binaries using the Homebrew package manager. For
   BSD, Linux on exotic architectures (not x86 or ARM64), and other Unix-like
   platforms, install the SDL2 binaries using your system's package manager. On
   all platforms, you can manually set the path of your SDL2 binaries using
   the ``PYSDL2_DLL_PATH`` environment variable.
   
... my script fails complaining that the *found* SDL2 library can't be used!

   Do you use a 64-bit operating system? Please make sure that the Python
   interpreter *and* that the SDL2 libraries are either 64-bit ones *or*
   32-bit ones. A 32-bit Python interpreter can't deal with a 64-bit library
   and vice versa.

Using...
--------

... the sdl2 API is weird. Why do you use the SDL\_ prefix all the time?

   PySDL2 offers 1-to-1 bindings for most of the SDL2 API, keeping the same
   function names and arguments as the original SDL2, SDL2\_mixer, SDL2\_ttf,
   SDL2\_image, and SDL2\_gfx libraries. This makes the package much easier to
   maintain, but also makes it easy to reference the SDL2 documentation and
   adapt C SDL2 examples into Python code. For a friendlier, more Pythonic
   wrapper around the SDL2 API, take a look at PySDL2's :mod:`sdl2.ext`
   module.

... the sdl2 API does not comply to PEP-8. Please make it PEP-8 compatible.

   Most of the API is PEP-8 compatible. However, the low-level bindings to SDL2
   and its addon libraries use the exact same function/structure/constant naming
   as their C counterparts, meaning that PySDL2 will necessarily diverge from
   PEP-8 in some places. The :mod:`sdl2.ext` module should theoretically be
   fully PEP-8 compliant.

How do I...
-----------

... save my surfaces as image files?

   You can use :func:`sdl2.ext.save_bmp()` to save them as bitmap files, or the
   low-level :func:`sdl2.sdlimage.IMG_SavePNG()` function to save them to PNG on
   systems with SDL\_image installed. For a wider range of formats, you can use
   the :func:`~sdl2.ext.surface_to_ndarray` function to convert an SDL surface
   into a Numpy array, open the array with the Pillow library using
   ``Image.from_array()``, and then save it to any export format supported by
   Pillow.
   
   
Font handling...
----------------

... is too hard. Why can't it work the same way as pygame does?

   The :mod:`sdl2.sdlttf` API does not know about platform-specific font
   locations and is unable to resolve font paths based on e.g. the font name
   or typeface. Additionally, it's usually a bad idea for a projects to rely
   on system fonts that may not be available on every computer: finding a
   free-use font you like and bundling it with your project is almost always
   the better option.
