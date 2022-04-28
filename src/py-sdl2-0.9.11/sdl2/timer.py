from ctypes import CFUNCTYPE, c_void_p, c_int
from .dll import _bind
from .stdinc import Uint32, Uint64, SDL_bool

__all__ = [
    # Defines
    "SDL_TimerID",

    # Macro Functions
    "SDL_TICKS_PASSED",

    # Functions
    "SDL_GetTicks", "SDL_GetTicks64", "SDL_GetPerformanceCounter",
    "SDL_GetPerformanceFrequency", "SDL_Delay",
    "SDL_AddTimer", "SDL_RemoveTimer",

    # Callback Functions
    "SDL_TimerCallback"
]


SDL_GetTicks = _bind("SDL_GetTicks", None, Uint32)
SDL_GetTicks64 = _bind("SDL_GetTicks64", None, Uint64, added='2.0.18')
SDL_GetPerformanceCounter = _bind("SDL_GetPerformanceCounter", None, Uint64)
SDL_GetPerformanceFrequency = _bind("SDL_GetPerformanceFrequency", None, Uint64)
SDL_Delay = _bind("SDL_Delay", [Uint32])

SDL_TimerCallback = CFUNCTYPE(Uint32, Uint32, c_void_p)
SDL_TimerID = c_int

SDL_AddTimer = _bind("SDL_AddTimer", [Uint32, SDL_TimerCallback, c_void_p], SDL_TimerID)
SDL_RemoveTimer = _bind("SDL_RemoveTimer", [SDL_TimerID], SDL_bool)

def SDL_TICKS_PASSED(A, B):
    return (B - A) <= 0

