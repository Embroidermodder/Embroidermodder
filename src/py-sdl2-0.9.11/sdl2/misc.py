from ctypes import c_char_p, c_int
from .dll import _bind

__all__ = [
    # Functions
    "SDL_OpenURL"
]


SDL_OpenURL = _bind("SDL_OpenURL", [c_char_p], c_int, added='2.0.14')
