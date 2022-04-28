from .dll import _bind
from ctypes import c_int, c_int8, c_uint8, c_int16, c_uint16, c_int32, \
    c_uint32, c_int64, c_uint64, c_size_t, c_void_p, c_char_p

__all__ = [
    # Defines
    "Sint8", "Uint8", "Sint16", "Uint16", "Sint32", "Uint32",
    "Sint64", "Uint64",

    # Enums
    "SDL_bool",
    "SDL_FALSE", "SDL_TRUE",
    
    # Functions
    "SDL_malloc", "SDL_calloc", "SDL_realloc", "SDL_free",
    "SDL_getenv", "SDL_setenv", "SDL_abs", "SDL_min", "SDL_max", "SDL_clamp",
    "SDL_memset", "SDL_memcpy"
]


# NOTE: Lots of functions in SDL_stdinc.h are not yet added here, but they're
# mostly for math and string operations that Python can do much more easily.

SDL_bool = c_int
SDL_FALSE = 0
SDL_TRUE = 1

Sint8 = c_int8
Uint8 = c_uint8
Sint16 = c_int16
Uint16 = c_uint16
Sint32 = c_int32
Uint32 = c_uint32
Sint64 = c_int64
Uint64 = c_uint64

SDL_malloc = _bind("SDL_malloc", [c_size_t], c_void_p)
SDL_calloc = _bind("SDL_calloc", [c_size_t, c_size_t], c_void_p)
SDL_realloc = _bind("SDL_realloc", [c_void_p, c_size_t], c_void_p)
SDL_free = _bind("SDL_free", [c_void_p], None)
SDL_getenv = _bind("SDL_getenv", [c_char_p], c_char_p)
SDL_setenv = _bind("SDL_setenv", [c_char_p, c_char_p, c_int], c_int)
SDL_abs = abs
SDL_min = min
SDL_max = max
SDL_memset = _bind("SDL_memset", [c_void_p, c_int, c_size_t], c_void_p)
SDL_memcpy = _bind("SDL_memcpy", [c_void_p, c_void_p, c_size_t], c_void_p)

def SDL_clamp(x, a, b):
    if x < a:
        return a
    elif x > b:
        return b
    else:
        return x
