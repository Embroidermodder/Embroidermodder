from ctypes import Structure, c_int, c_char_p, POINTER
from .dll import _bind
from .stdinc import Uint8, Uint16, Uint32, SDL_bool
from .keycode import SDL_Keycode, SDL_Keymod
from .scancode import SDL_Scancode
from .rect import SDL_Rect
from .video import SDL_Window

__all__ = [
    # Structs
    "SDL_Keysym",
    
    # Functions
    "SDL_GetKeyboardFocus", "SDL_GetKeyboardState",
    "SDL_GetModState", "SDL_SetModState", "SDL_GetKeyFromScancode",
    "SDL_GetScancodeFromKey", "SDL_GetScancodeName",
    "SDL_GetScancodeFromName", "SDL_GetKeyName", "SDL_GetKeyFromName",
    "SDL_StartTextInput", "SDL_IsTextInputActive", "SDL_StopTextInput",
    "SDL_SetTextInputRect", "SDL_HasScreenKeyboardSupport",
    "SDL_IsScreenKeyboardShown"
]


class SDL_Keysym(Structure):
    _fields_ = [("scancode", SDL_Scancode),
                ("sym", SDL_Keycode),
                ("mod", Uint16),
                ("unused", Uint32)
                ]

SDL_GetKeyboardFocus = _bind("SDL_GetKeyboardFocus", None, POINTER(SDL_Window))
SDL_GetKeyboardState = _bind("SDL_GetKeyboardState", [POINTER(c_int)], POINTER(Uint8))
SDL_GetModState = _bind("SDL_GetModState", None, SDL_Keymod)
SDL_SetModState = _bind("SDL_SetModState", [SDL_Keymod])
SDL_GetKeyFromScancode = _bind("SDL_GetKeyFromScancode", [SDL_Scancode], SDL_Keycode)
SDL_GetScancodeFromKey = _bind("SDL_GetScancodeFromKey", [SDL_Keycode], SDL_Scancode)
SDL_GetScancodeName = _bind("SDL_GetScancodeName", [SDL_Scancode], c_char_p)
SDL_GetScancodeFromName = _bind("SDL_GetScancodeFromName", [c_char_p], SDL_Scancode)
SDL_GetKeyName = _bind("SDL_GetKeyName", [SDL_Keycode], c_char_p)
SDL_GetKeyFromName = _bind("SDL_GetKeyFromName", [c_char_p], SDL_Keycode)
SDL_StartTextInput = _bind("SDL_StartTextInput")
SDL_IsTextInputActive = _bind("SDL_IsTextInputActive", None, SDL_bool)
SDL_StopTextInput = _bind("SDL_StopTextInput")
SDL_SetTextInputRect = _bind("SDL_SetTextInputRect", [POINTER(SDL_Rect)])
SDL_HasScreenKeyboardSupport = _bind("SDL_HasScreenKeyboardSupport", None, SDL_bool)
SDL_IsScreenKeyboardShown = _bind("SDL_IsScreenKeyboardShown", [POINTER(SDL_Window)], SDL_bool)
