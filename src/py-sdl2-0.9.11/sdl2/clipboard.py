from ctypes import c_char_p, c_int
from .dll import _bind
from .stdinc import SDL_bool

__all__ = [
    "SDL_SetClipboardText", "SDL_GetClipboardText", "SDL_HasClipboardText"
]


SDL_SetClipboardText = _bind("SDL_SetClipboardText", [c_char_p], c_int)
SDL_GetClipboardText = _bind("SDL_GetClipboardText", None, c_char_p)
SDL_HasClipboardText = _bind("SDL_HasClipboardText", None, SDL_bool)
