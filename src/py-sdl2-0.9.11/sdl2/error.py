from ctypes import POINTER, c_char_p, c_int, c_char
from .dll import _bind

__all__ = [
    # Enums
    "SDL_errorcode",
    "SDL_ENOMEM", "SDL_EFREAD", "SDL_EFWRITE", "SDL_EFSEEK",
    "SDL_UNSUPPORTED", "SDL_LASTERROR",

    # Macro Functions
    "SDL_OutOfMemory", "SDL_Unsupported", "SDL_InvalidParamError", "SDL_Error",

    # Functions
    "SDL_SetError", "SDL_GetError", "SDL_GetErrorMsg", "SDL_ClearError"
]


SDL_SetError = _bind("SDL_SetError", [c_char_p], c_int)
SDL_GetError = _bind("SDL_GetError", None, c_char_p)
SDL_GetErrorMsg = _bind("SDL_GetErrorMsg", [POINTER(c_char), c_int], c_char_p, added='2.0.14')
SDL_ClearError = _bind("SDL_ClearError")


SDL_errorcode = c_int

SDL_ENOMEM = 0
SDL_EFREAD = 1
SDL_EFWRITE = 2
SDL_EFSEEK = 3
SDL_UNSUPPORTED = 4
SDL_LASTERROR = 5


SDL_Error = _bind("SDL_Error", [SDL_errorcode], c_int)
SDL_OutOfMemory = SDL_Error(SDL_ENOMEM)
SDL_Unsupported = SDL_Error(SDL_UNSUPPORTED)
SDL_InvalidParamError = lambda x: SDL_SetError("Parameter '%s' is invalid" % (x))
