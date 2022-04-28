import sys
import ctypes
import pytest
from sdl2 import version, dll, __version__, version_info


def test_SDL_version():
    v = version.SDL_version(0, 0, 0)
    assert v.major == 0
    assert v.minor == 0
    assert v.patch == 0

def test_SDL_GetVersion():
    v = version.SDL_version()
    version.SDL_GetVersion(ctypes.byref(v))
    assert type(v) == version.SDL_version
    assert v.major == 2
    assert v.minor == 0
    assert v.patch >= 5

def test_SDL_VERSIONNUM():
    assert version.SDL_VERSIONNUM(1, 2, 3) == 1203
    assert version.SDL_VERSIONNUM(4, 5, 6) == 4506
    assert version.SDL_VERSIONNUM(2, 0, 0) == 2000
    assert version.SDL_VERSIONNUM(17, 42, 3) == 21203

def test_SDL_VERSION_ATLEAST():
    assert version.SDL_VERSION_ATLEAST(1, 2, 3)
    assert version.SDL_VERSION_ATLEAST(2, 0, 0)
    assert version.SDL_VERSION_ATLEAST(2, 0, 1)
    assert not version.SDL_VERSION_ATLEAST(2, 0, 100)

def test_SDL_GetRevision():
    rev = version.SDL_GetRevision()
    # If revision not empty string (e.g. Conda), test the prefix
    if len(rev):
        if dll.version >= 2016:
            assert rev[0:4] == b"http"
        else:
            assert rev[0:3] == b"hg-"

def test_SDL_GetRevisionNumber():
    if sys.platform in ("win32",) or dll.version >= 2016:
        # HG tip on Win32 does not set any revision number
        assert version.SDL_GetRevisionNumber() >= 0
    else:
        assert version.SDL_GetRevisionNumber() >= 7000
