from ctypes import Structure, c_int, c_char_p, POINTER
from .dll import _bind
from .stdinc import Uint8, Uint32
from .video import SDL_Window

__all__ = [
    # Structs
    "SDL_MessageBoxButtonData", "SDL_MessageBoxColor",
    "SDL_MessageBoxColorScheme", "SDL_MessageBoxData",

    # Enums
    "SDL_MessageBoxFlags",
    "SDL_MESSAGEBOX_ERROR", "SDL_MESSAGEBOX_WARNING",
    "SDL_MESSAGEBOX_INFORMATION", "SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT",
    "SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT",

    "SDL_MessageBoxButtonFlags",
    "SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT",
    "SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT",

    "SDL_MessageBoxColorType",
    "SDL_MESSAGEBOX_COLOR_BACKGROUND", "SDL_MESSAGEBOX_COLOR_TEXT",
    "SDL_MESSAGEBOX_COLOR_BUTTON_BORDER",
    "SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND",
    "SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED", "SDL_MESSAGEBOX_COLOR_MAX",

    # Functions
    "SDL_ShowMessageBox", "SDL_ShowSimpleMessageBox"
]


SDL_MessageBoxFlags = c_int

SDL_MESSAGEBOX_ERROR = 0x00000010
SDL_MESSAGEBOX_WARNING = 0x00000020
SDL_MESSAGEBOX_INFORMATION = 0x00000040
SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT = 0x00000080
SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT = 0x00000100


SDL_MessageBoxButtonFlags = c_int

SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001
SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002


class SDL_MessageBoxButtonData(Structure):
    _fields_ = [("flags", Uint32), ("buttonid", c_int), ("text", c_char_p)]

class SDL_MessageBoxColor(Structure):
    _fields_ = [("r", Uint8), ("g", Uint8), ("b", Uint8)]


SDL_MessageBoxColorType = c_int

SDL_MESSAGEBOX_COLOR_BACKGROUND = 0
SDL_MESSAGEBOX_COLOR_TEXT = 1
SDL_MESSAGEBOX_COLOR_BUTTON_BORDER = 2
SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND = 3
SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED = 4
SDL_MESSAGEBOX_COLOR_MAX = 5


class SDL_MessageBoxColorScheme(Structure):
    _fields_ = [("colors", (SDL_MessageBoxColor * SDL_MESSAGEBOX_COLOR_MAX))]

class SDL_MessageBoxData(Structure):
    _fields_ = [("flags", Uint32),
                ("window", POINTER(SDL_Window)),
                ("title", c_char_p),
                ("message", c_char_p),
                ("numbuttons", c_int),
                ("buttons", POINTER(SDL_MessageBoxButtonData)),
                ("colorScheme", POINTER(SDL_MessageBoxColorScheme))
                ]


SDL_ShowMessageBox = _bind("SDL_ShowMessageBox", [POINTER(SDL_MessageBoxData), POINTER(c_int)], c_int)
SDL_ShowSimpleMessageBox = _bind("SDL_ShowSimpleMessageBox", [Uint32, c_char_p, c_char_p, POINTER(SDL_Window)], c_int)
