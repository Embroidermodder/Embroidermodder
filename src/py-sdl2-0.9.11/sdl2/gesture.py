from ctypes import c_int, POINTER
from .dll import _bind
from .stdinc import Sint64
from .touch import SDL_TouchID
from .rwops import SDL_RWops

__all__ = [
    # Defines
    "SDL_GestureID",
    
    # Functions
    "SDL_RecordGesture", "SDL_SaveAllDollarTemplates",
    "SDL_SaveDollarTemplate", "SDL_LoadDollarTemplates"
]


SDL_GestureID = Sint64
SDL_RecordGesture = _bind("SDL_RecordGesture", [SDL_TouchID], c_int)
SDL_SaveAllDollarTemplates = _bind("SDL_SaveAllDollarTemplates", [POINTER(SDL_RWops)], c_int)
SDL_SaveDollarTemplate = _bind("SDL_SaveDollarTemplate", [SDL_GestureID, POINTER(SDL_RWops)], c_int)
SDL_LoadDollarTemplates = _bind("SDL_LoadDollarTemplates", [SDL_TouchID, POINTER(SDL_RWops)], c_int)
