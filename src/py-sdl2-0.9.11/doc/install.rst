Installing PySDL2
=================
This section provides an overview and guidance for installing PySDL2 on
various target platforms.

Using pip
---------
The easiest way to get up and running with PySDL2 is to install it and the
SDL2 binaries it depends on using the Pip package manager ::

  python -m pip install pysdl2 pysdl2-dll 

Note that the pysdl2-dll binaries may not be available yet for your platform.
If not, please read through the Prerequisites section below.

Installing from source
----------------------
You can download the official releases of PySDL2 from
https://github.com/marcusva/py-sdl2/releases. Download the most
recent release, unpack it and make sure that you installed the relevant
prerequisites before continuing with the installation.

Then, you can install PySDL2 directly with Python ::

   python setup.py install

Prerequisites
-------------
PySDL2 relies on some additional libraries to be fully usable and to
provide you full access to all of its features.

You must have at least one of the following Python versions installed:

* Python 2.7, 3.5+     (http://www.python.org)
* PyPy 1.8.0+          (http://www.pypy.org)

Other Python versions or Python implementations might work, but are
(currently) not officially tested or supported by the PySDL2
distribution.

You need to have a working SDL2 library on your target system. On Windows
(32-bit and 64-bit x86), macOS (64-bit x86), and Linux (32-bit/64-bit x86,
and 64-bit ARM), you can install the latest official binaries with pip via the
pysdl2-dll package. Alternatively, you can install it using your system's
package manager, download the official binaries from http://www.libsdl.org, or
compile it from source yourself.

PySDL2 also offers optional support for the following SDL-related libraries:

* SDL2_image             (http://www.libsdl.org/projects/SDL_image/)
* SDL2_mixer             (http://www.libsdl.org/projects/SDL_mixer/)
* SDL2_ttf               (http://www.libsdl.org/projects/SDL_ttf/)
* SDL2_gfx               (http://www.ferzkopp.net/Software/SDL_gfx-2.0/)

However, these are only necessary if you want to use the :mod:`sdl2.sdlimage`,
:mod:`sdl2.sdlmixer`, :mod:`sdl2.sdlttf` or :mod:`sdl2.sdlgfx` modules. Binaries
for all these modules are provided by the pysdl2-dll package.

Trying out
^^^^^^^^^^
You also can test out PySDL2 without actually installing it. You just
need to set up your ``PYTHONPATH`` to point to the location of the
source distribution package. On Windows-based platforms, you might use
something like ::

   set PYTHONPATH=C:\path\to\pysdl2\:%PYTHONPATH%

to define the ``PYTHONPATH`` on a command shell. On Linux/Unix, use ::

   export PYTHONPATH=/path/to/pysdl2:$PYTHONPATH

for bourne shell compatibles or ::

   setenv PYTHONPATH /path/to/pysdl2:$PYTHONPATH

for C shell compatibles. You can omit the `:$PYTHONPATH`, if you did not use
it so far and if your environment settings do not define it.

.. note::

   If you did not install SDL2 using the preferred way for your operation
   system, please read the information about :ref:`importing-pysdl2` in the
   section :doc:`integration`.
