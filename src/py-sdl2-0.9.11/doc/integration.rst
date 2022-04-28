Integrating PySDL2
==================
PySDL2 consists of two packages, :mod:`sdl2`, which is a plain 1:1 API
wrapper around the SDL2 API, and :mod:`sdl2.ext`, which offers enhanced
functionality for :mod:`sdl2`.

The :mod:`sdl2` package is implemented in a way that shall make it easy for
you to integrate and deploy it with your own software projects. You can rely
on PySDL2 as third-party package, so that the user needs to install it
before he can use your software. Alternatively, you can just copy the
whole package into your project to ship it within your own project
bundle.

.. _importing-pysdl2:

Bundling SDL2 with pysdl2-dll
-----------------------------
The :mod:`sdl2` package relies on an external SDL2 library for creating the
wrapper functions. This means that the user needs to have SDL2 installed or
that you ship a SDL2 library with your project.

For macOS, Windows, and most distributions of x86 and ARM64 Linux, the easiest
and most flexible way to bundle and install the SDL2 binaries with your project
is via the ``pysdl2-dll`` package on PyPI,
which pysdl2 will load automatically if available. This approach allows you to
add the SDL2 binaries as a dependency for your project in a requirements.txt
file, a setup.py file, a Pipfile, or any other form of Python dependency
tracking. You can also specify a minimum version of the SDL2 binaries for your
project using this mechanism if your project depends on a function not
available in earlier versions of SDL2.

For platforms without any available pysdl2-dll binaries (e.g. Alpine Linux, 
ARM32 Linux, BSD), PySDL2 will still work as long as a recent version of SDL2
is installed using the system's package manager. Additionally, pysdl2-dll will
still install successfully on unsupported platforms as an empty pacakge with
no binaries, making it safe to add as a dependency for cross-platform projects.

Bundling SDL2 without pysdl2-dll
--------------------------------
If the user has a SDL2 library installed on the target system, the
:mod:`ctypes` hooks of :mod:`sdl2` try to find it in the OS-specific standard
locations via :func:`ctypes.util.find_library`. If you are going to ship your
own SDL2 library with the project or cannot rely on the standard mechanism of
:mod:`ctypes`, it is also possible to set the environment variable
:envvar:`PYSDL2_DLL_PATH`, which shall point to the directory of the SDL2
library or consist of a list of directories, in which the SDL2 libraries can
be found.

.. note::

   :envvar:`PYSDL2_DLL_PATH` is preferred over the standard
   mechanism. If the module finds a SDL2 library in :envvar:`PYSDL2_DLL_PATH`,
   it will try to use that one in the first place, before using any SDL2
   library installed on the target system.

Let's assume you ship your own library *SDL2.dll* within your project
location *fancy_project/third_party*. You can set the environment
variable :envvar:`PYSDL2_DLL_PATH` before starting Python. ::

  # Win32 platforms
  set PYSDL2_DLL_PATH=C:\path\to\fancy_project\third_party

  # Unix/Posix-alike environments - bourne shells
  export PYSDL2_DLL_PATH=/path/to/fancy_project/third_party

  # Unix/Posix-alike environments - C shells
  setenv PYSDL2_DLL_PATH /path/to/fancy_project/third_party

  # Define multiple paths to search for the libraries - Win32
  set PYSDL2_DLL_PATH=C:\first\path;C:\second\path

You also can set the environment variable within Python using
:data:`os.environ`. ::

  dllpath = os.path.join('path', 'to', 'fancy_project', 'third_party')
  os.environ["PYSDL2_DLL_PATH"] = dllpath

.. note::

   If you aim to integrate :mod:`sdl` directly into your software and do
   not want or are not allowed to change the environment variables, you
   can also change the ``os.getenv("PYSDL2_DLL_PATH")`` query within the
   *sdl2/dll.py* (or *sdl2/sdlimage.py*, *sdl2/sdlttf.py*, *sdl2/sdlgfx.py*)
   file to point to the directory, in which you keep the DLL.

Using different SDL2 versions
-----------------------------
PySDL2 tries to provide interfaces to the most recent versions of the
SDL2 libraries. Sometimes this means that PySDL2 tries to test for
functions that might not be available for your very own project or that
are not available on the target system due to a version of the specific
library.

If a PySDL2 function is called that requires a newer version
of a binary than the one currently being used, it will raise a
RuntimeWarning indicating the minimum version of SDL2 (or SDL2_mixer,
or SDL2_ttf, etc.) required to use the called function. Additionally,
if you already know what minimum versions your project needs, you can
check the linked binary versions at runtime: ::

  if not (sdl2.dll.version >= 2008 and sdl2.sdlttf.dll.version >= 2015):
    err = ("This project requires SDL2 >= 2.0.8 and SDL2_ttf >= 2.0.15. "
     "Please update your SDL2 binaries and relaunch.")
    raise RuntimeError(err)

Binary version numbers are stored as 4-digit integers, with the first digit
being the major release, the second digit being the minor release, and the
last two digits indicating the patch level. Thus, SDL2 2.0.10 would be
version 2010 and SDL2 2.0.6 would be 2006.

Known problems with SDL2 binaries
---------------------------------
The SDL2 libraries are designed to be highly consistent across platforms, but
there are some occasional bugs/issues with certain common binaries that can
result in unexpected problems. Here are the ones we are currently aware of:

**Support for XCF images broken in SDL2_image**
  Affects: official 32-bit Windows SDL2_image 2.0.5 binaries, all official
  macOS SDL2_image binaries

**Support for FLAC and MP3 missing by default in SDL2_mixer**
  Affects: SDL2_mixer installed with Homebrew on macOS

**Support for FLAC/MP3/OGG/MID/MOD broken in SDL2_mixer**
  Affects: official SDL2_mixer 2.0.2 binaries for macOS and Windows (fixed
  in 2.0.4)
