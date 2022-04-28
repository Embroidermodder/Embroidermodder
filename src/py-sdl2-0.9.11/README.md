# PySDL2

[![Tests](https://github.com/marcusva/py-sdl2/actions/workflows/run_tests.yml/badge.svg)](https://github.com/marcusva/py-sdl2/actions/workflows/run_tests.yml)
![PyPI - Python Version](https://img.shields.io/pypi/pyversions/pysdl2)
[![PyPI Version](https://img.shields.io/pypi/v/PySDL2.svg)](https://pypi.python.org/pypi/PySDL2)

PySDL2 is a pure Python wrapper around the SDL2, SDL2\_mixer, SDL2\_image,
SDL2\_ttf, and SDL2\_gfx libraries.
Instead of relying on C code, it uses the built-in ctypes module to interface
with SDL2, and provides simple Python classes and wrappers for common
SDL2 functionality.

## Installation

PySDL2 is easy to install and integrate within your own projects.
To install or update to the latest version, simply run one of the
following commands in a terminal:

```bash
# Install latest stable version from PyPI
pip install -U pysdl2

# Install latest development verion from GitHub
pip install -U git+https://github.com/marcusva/py-sdl2.git
```

**Note**: If installing on Python 3 on a computer where both Python 2 and 3
are installed, replace `pip` with `pip3` in the above commands.

## Requirements

In order for PySDL2 to work, the binaries for SDL2 (and any SDL2 addon modules
you wish to use, e.g. SDL2\_mixer) need to be installed on your system. On
macOS and Windows, the recommended way to install the SDL2 binaries is via the `pysdl2-dll` package using pip:

```bash
pip install pysdl2-dll
```

This will install pre-built binaries for all supported SDL2 libraries as
a Python package, which PySDL2 will automatically load if available.
On Linux and other Unix-like OSes, you can install the SDL2 binaries using
your system's package manager (which may be out of date), or alternatively
build and install the latest versions yourself from source.

The current minimum supported versions for each library are listed below:

* **SDL2** >= 2.0.5
* **SDL2_mixer** >= 2.0.1 (for the `sdl2.sdlmixer` module)
* **SDL2_ttf** >= 2.0.14 (for the `sdl2.sdlttf` module)
* **SDL2_image** >= 2.0.1 (for the `sdl2.sdlimage` module)
* **SDL2_gfx** >= 1.0.3 (for the `sdl2.sdlgfx` module)

## Documentation

If you just started with SDL and PySDL2, it is strongly recommended
that you read through the tutorial of the documentation to learn the
basics. You can find the documentation at `doc/html` or online at
<http://pysdl2.readthedocs.org>.

## License

This library is given to the public domain. There are no licensing
restrictions. Please see `doc/copying.rst` for further details.
