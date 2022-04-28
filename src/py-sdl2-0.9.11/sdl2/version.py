from ctypes import Structure, POINTER, c_char_p, c_int
from .dll import _bind
from .stdinc import Uint8

__all__ = [
    # Structs
    "SDL_version",

    # Defines
    "SDL_MAJOR_VERSION", "SDL_MINOR_VERSION", "SDL_PATCHLEVEL",

    # Macro Functions
    "SDL_VERSION", "SDL_VERSIONNUM", "SDL_COMPILEDVERSION",
    "SDL_VERSION_ATLEAST",

    # Functions
    "SDL_GetVersion", "SDL_GetRevision", "SDL_GetRevisionNumber"
]


class SDL_version(Structure):
    _fields_ = [("major", Uint8),
                ("minor", Uint8),
                ("patch", Uint8),
                ]

SDL_MAJOR_VERSION = 2
SDL_MINOR_VERSION = 0
SDL_PATCHLEVEL = 20

def SDL_VERSION(x):
    x.major = SDL_MAJOR_VERSION
    x.minor = SDL_MINOR_VERSION
    x.patch = SDL_PATCHLEVEL

SDL_VERSIONNUM = lambda x, y, z: (x * 1000 + y * 100 + z)
SDL_COMPILEDVERSION = SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL)
SDL_VERSION_ATLEAST = lambda x, y, z: (SDL_COMPILEDVERSION >= SDL_VERSIONNUM(x, y, z))
SDL_GetVersion = _bind("SDL_GetVersion", [POINTER(SDL_version)])
SDL_GetRevision = _bind("SDL_GetRevision", None, c_char_p)
SDL_GetRevisionNumber = _bind("SDL_GetRevisionNumber", None, c_int) # Deprecated as of 2.0.16, add warning?
