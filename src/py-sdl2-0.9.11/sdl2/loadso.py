from ctypes import c_char_p, c_void_p
from .dll import _bind

__all__ = ["SDL_LoadObject", "SDL_LoadFunction", "SDL_UnloadObject"]

SDL_LoadObject = _bind("SDL_LoadObject", [c_char_p], c_void_p)
SDL_LoadFunction = _bind("SDL_LoadFunction", [c_void_p, c_char_p], c_void_p)
SDL_UnloadObject = _bind("SDL_UnloadObject", [c_void_p])
