import os
import sys
import pytest
from ctypes import cast, c_char_p, addressof
from sdl2 import filesystem, SDL_free


def test_SDL_GetBasePath():
    path = filesystem.SDL_GetBasePath()
    path = path.decode("utf-8")
    if sys.version_info[0] < 3:
        is_python_path = False
        for s in [u"python", u"pypy", u"pyenv", u"virtualenv"]:
            if s in path.lower():
                is_python_path = True
                break
        assert is_python_path
    else:
        execprefix = sys.base_exec_prefix
        assert execprefix.lower() in path.lower()

def test_SDL_GetPrefPath():
    path = filesystem.SDL_GetPrefPath(b"OrgName", b"AppName")
    path = path.decode("utf-8")
    assert "OrgName" in path
    assert "AppName" in path
