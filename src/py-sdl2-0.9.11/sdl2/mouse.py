from ctypes import Structure, POINTER, c_int, c_void_p
from .dll import _bind
from .stdinc import Uint8, Uint32, SDL_bool
from .video import SDL_Window
from .surface import SDL_Surface

__all__ = [
    # Opaque Types
    "SDL_Cursor",
    
    # Defines
    "SDL_BUTTON_LEFT", "SDL_BUTTON_MIDDLE", "SDL_BUTTON_RIGHT",
    "SDL_BUTTON_X1", "SDL_BUTTON_X2",

    # Enums
    "SDL_SystemCursor",
    "SDL_SYSTEM_CURSOR_ARROW", "SDL_SYSTEM_CURSOR_IBEAM",
    "SDL_SYSTEM_CURSOR_WAIT", "SDL_SYSTEM_CURSOR_CROSSHAIR",
    "SDL_SYSTEM_CURSOR_WAITARROW", "SDL_SYSTEM_CURSOR_SIZENWSE",
    "SDL_SYSTEM_CURSOR_SIZENESW", "SDL_SYSTEM_CURSOR_SIZEWE",
    "SDL_SYSTEM_CURSOR_SIZENS", "SDL_SYSTEM_CURSOR_SIZEALL",
    "SDL_SYSTEM_CURSOR_NO", "SDL_SYSTEM_CURSOR_HAND", "SDL_NUM_SYSTEM_CURSORS",

    "SDL_MouseWheelDirection",
    "SDL_MOUSEWHEEL_NORMAL", "SDL_MOUSEWHEEL_FLIPPED",

    # Macro Functions
    "SDL_BUTTON", "SDL_BUTTON_LMASK", "SDL_BUTTON_MMASK",
    "SDL_BUTTON_RMASK", "SDL_BUTTON_X1MASK", "SDL_BUTTON_X2MASK",
    
    # Functions
    "SDL_GetMouseFocus", "SDL_GetMouseState", "SDL_GetGlobalMouseState",
    "SDL_GetRelativeMouseState",
    "SDL_WarpMouseInWindow", "SDL_WarpMouseGlobal", "SDL_SetRelativeMouseMode",
    "SDL_CaptureMouse", "SDL_GetRelativeMouseMode",
    "SDL_CreateCursor", "SDL_CreateColorCursor", "SDL_CreateSystemCursor",
    "SDL_SetCursor", "SDL_GetCursor", "SDL_GetDefaultCursor", "SDL_FreeCursor",
    "SDL_ShowCursor"
]


class SDL_Cursor(c_void_p):
    pass

SDL_SystemCursor = c_int
SDL_SYSTEM_CURSOR_ARROW = 0
SDL_SYSTEM_CURSOR_IBEAM = 1
SDL_SYSTEM_CURSOR_WAIT = 2
SDL_SYSTEM_CURSOR_CROSSHAIR = 3
SDL_SYSTEM_CURSOR_WAITARROW = 4
SDL_SYSTEM_CURSOR_SIZENWSE = 5
SDL_SYSTEM_CURSOR_SIZENESW = 6
SDL_SYSTEM_CURSOR_SIZEWE = 7
SDL_SYSTEM_CURSOR_SIZENS = 8
SDL_SYSTEM_CURSOR_SIZEALL = 9
SDL_SYSTEM_CURSOR_NO = 10
SDL_SYSTEM_CURSOR_HAND = 11
SDL_NUM_SYSTEM_CURSORS = 12

SDL_MouseWheelDirection = c_int
SDL_MOUSEWHEEL_NORMAL = 0
SDL_MOUSEWHEEL_FLIPPED = 1

SDL_BUTTON = lambda X: (1 << ((X) - 1))
SDL_BUTTON_LEFT = 1
SDL_BUTTON_MIDDLE = 2
SDL_BUTTON_RIGHT = 3
SDL_BUTTON_X1 = 4
SDL_BUTTON_X2 = 5
SDL_BUTTON_LMASK = SDL_BUTTON(SDL_BUTTON_LEFT)
SDL_BUTTON_MMASK = SDL_BUTTON(SDL_BUTTON_MIDDLE)
SDL_BUTTON_RMASK = SDL_BUTTON(SDL_BUTTON_RIGHT)
SDL_BUTTON_X1MASK = SDL_BUTTON(SDL_BUTTON_X1)
SDL_BUTTON_X2MASK = SDL_BUTTON(SDL_BUTTON_X2)

SDL_GetMouseFocus = _bind("SDL_GetMouseFocus", None, POINTER(SDL_Window))
SDL_GetMouseState = _bind("SDL_GetMouseState", [POINTER(c_int), POINTER(c_int)], Uint32)
SDL_GetRelativeMouseState = _bind("SDL_GetRelativeMouseState", [POINTER(c_int), POINTER(c_int)], Uint32)
SDL_WarpMouseInWindow = _bind("SDL_WarpMouseInWindow", [POINTER(SDL_Window), c_int, c_int])
SDL_SetRelativeMouseMode = _bind("SDL_SetRelativeMouseMode", [SDL_bool], c_int)
SDL_GetRelativeMouseMode = _bind("SDL_GetRelativeMouseMode", None, SDL_bool)
SDL_CreateCursor = _bind("SDL_CreateCursor", [POINTER(Uint8), POINTER(Uint8), c_int, c_int, c_int, c_int], POINTER(SDL_Cursor))
SDL_CreateColorCursor = _bind("SDL_CreateColorCursor", [POINTER(SDL_Surface), c_int, c_int], POINTER(SDL_Cursor))
SDL_CreateSystemCursor = _bind("SDL_CreateSystemCursor", [SDL_SystemCursor], POINTER(SDL_Cursor))
SDL_SetCursor = _bind("SDL_SetCursor", [POINTER(SDL_Cursor)])
SDL_GetCursor = _bind("SDL_GetCursor", None, POINTER(SDL_Cursor))
SDL_GetDefaultCursor = _bind("SDL_GetDefaultCursor", None, POINTER(SDL_Cursor))
SDL_FreeCursor = _bind("SDL_FreeCursor", [POINTER(SDL_Cursor)])
SDL_ShowCursor = _bind("SDL_ShowCursor", [c_int], c_int)
SDL_WarpMouseGlobal = _bind("SDL_WarpMouseGlobal", [c_int, c_int], c_int, added='2.0.4')
SDL_CaptureMouse = _bind("SDL_CaptureMouse", [SDL_bool], c_int, added='2.0.4')
SDL_GetGlobalMouseState = _bind("SDL_GetGlobalMouseState", [POINTER(c_int), POINTER(c_int)], Uint32, added='2.0.4')
