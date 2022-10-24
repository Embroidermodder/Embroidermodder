import sys
import pytest
from sdl2 import platform


def test_SDL_GetPlatform():
    retval = platform.SDL_GetPlatform()
    if sys.platform in ("win32", "cygwin"):
        assert retval == b"Windows"
    elif sys.platform.startswith("linux"):
        assert retval == b"Linux"
    elif sys.platform.startswith("freebsd"):
        assert retval == b"FreeBSD"
    elif sys.platform.startswith("darwin"):
        assert retval == b"Mac OS X"
    # Do not check others atm, since we are unsure about what Python will
    # return here.
