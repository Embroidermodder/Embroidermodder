from ctypes import POINTER, c_int
from .dll import _bind

__all__ = [
    # Enums
    "SDL_PowerState",
    "SDL_POWERSTATE_UNKNOWN", "SDL_POWERSTATE_ON_BATTERY",
    "SDL_POWERSTATE_NO_BATTERY", "SDL_POWERSTATE_CHARGING",
    "SDL_POWERSTATE_CHARGED",

    # Functions
    "SDL_GetPowerInfo"
]


SDL_PowerState = c_int

SDL_POWERSTATE_UNKNOWN = 0
SDL_POWERSTATE_ON_BATTERY = 1
SDL_POWERSTATE_NO_BATTERY = 2
SDL_POWERSTATE_CHARGING = 3
SDL_POWERSTATE_CHARGED = 4


SDL_GetPowerInfo = _bind("SDL_GetPowerInfo", [POINTER(c_int), POINTER(c_int)], SDL_PowerState)
