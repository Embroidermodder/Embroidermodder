from ctypes import c_int, c_void_p, c_char_p, c_float, c_size_t, \
    py_object, Structure, POINTER, CFUNCTYPE
from .dll import _bind
from .stdinc import Uint16, Uint32, SDL_bool
from .rect import SDL_Point, SDL_Rect
from .surface import SDL_Surface

__all__ = [
    # Structs & Opaque Types
    "SDL_DisplayMode", "SDL_Window", "SDL_GLContext",

    # Enums
    "SDL_WindowFlags",
    "SDL_WINDOW_FULLSCREEN", "SDL_WINDOW_OPENGL", "SDL_WINDOW_SHOWN",
    "SDL_WINDOW_HIDDEN", "SDL_WINDOW_BORDERLESS",
    "SDL_WINDOW_RESIZABLE", "SDL_WINDOW_MINIMIZED",
    "SDL_WINDOW_MAXIMIZED",
    "SDL_WINDOW_INPUT_GRABBED", "SDL_WINDOW_MOUSE_GRABBED",
    "SDL_WINDOW_INPUT_FOCUS", "SDL_WINDOW_MOUSE_FOCUS",
    "SDL_WINDOW_FULLSCREEN_DESKTOP", "SDL_WINDOW_FOREIGN",
    "SDL_WINDOW_ALLOW_HIGHDPI", "SDL_WINDOW_MOUSE_CAPTURE",
    "SDL_WINDOW_ALWAYS_ON_TOP", "SDL_WINDOW_SKIP_TASKBAR",
    "SDL_WINDOW_UTILITY", "SDL_WINDOW_TOOLTIP",
    "SDL_WINDOW_POPUP_MENU", "SDL_WINDOW_KEYBOARD_GRABBED",
    "SDL_WINDOW_VULKAN", "SDL_WINDOW_METAL",
    "SDL_WINDOW_INPUT_FOCUS",

    "SDL_WindowEventID",
    "SDL_WINDOWEVENT_NONE",
    "SDL_WINDOWEVENT_SHOWN", "SDL_WINDOWEVENT_HIDDEN",
    "SDL_WINDOWEVENT_EXPOSED", "SDL_WINDOWEVENT_MOVED",
    "SDL_WINDOWEVENT_RESIZED", "SDL_WINDOWEVENT_SIZE_CHANGED",
    "SDL_WINDOWEVENT_MINIMIZED", "SDL_WINDOWEVENT_MAXIMIZED",
    "SDL_WINDOWEVENT_RESTORED", "SDL_WINDOWEVENT_ENTER",
    "SDL_WINDOWEVENT_LEAVE", "SDL_WINDOWEVENT_FOCUS_GAINED",
    "SDL_WINDOWEVENT_FOCUS_LOST", "SDL_WINDOWEVENT_CLOSE",
    "SDL_WINDOWEVENT_TAKE_FOCUS", "SDL_WINDOWEVENT_HIT_TEST",
    "SDL_WINDOWEVENT_ICCPROF_CHANGED", "SDL_WINDOWEVENT_DISPLAY_CHANGED",

    "SDL_DisplayEventID",
    "SDL_DISPLAYEVENT_NONE", "SDL_DISPLAYEVENT_ORIENTATION",
    "SDL_DISPLAYEVENT_CONNECTED", "SDL_DISPLAYEVENT_DISCONNECTED",
    
    "SDL_DisplayOrientation",
    "SDL_ORIENTATION_UNKNOWN", "SDL_ORIENTATION_LANDSCAPE",
    "SDL_ORIENTATION_LANDSCAPE_FLIPPED", "SDL_ORIENTATION_PORTRAIT",
    "SDL_ORIENTATION_PORTRAIT_FLIPPED",

    "SDL_FlashOperation",
    "SDL_FLASH_CANCEL", "SDL_FLASH_BRIEFLY", "SDL_FLASH_UNTIL_FOCUSED",

    "SDL_GLattr",
    "SDL_GL_RED_SIZE",
    "SDL_GL_GREEN_SIZE", "SDL_GL_BLUE_SIZE", "SDL_GL_ALPHA_SIZE",
    "SDL_GL_BUFFER_SIZE", "SDL_GL_DOUBLEBUFFER", "SDL_GL_DEPTH_SIZE",
    "SDL_GL_STENCIL_SIZE", "SDL_GL_ACCUM_RED_SIZE",
    "SDL_GL_ACCUM_GREEN_SIZE", "SDL_GL_ACCUM_BLUE_SIZE",
    "SDL_GL_ACCUM_ALPHA_SIZE", "SDL_GL_STEREO",
    "SDL_GL_MULTISAMPLEBUFFERS", "SDL_GL_MULTISAMPLESAMPLES",
    "SDL_GL_ACCELERATED_VISUAL", "SDL_GL_RETAINED_BACKING",
    "SDL_GL_CONTEXT_MAJOR_VERSION", "SDL_GL_CONTEXT_MINOR_VERSION",
    "SDL_GL_CONTEXT_EGL", "SDL_GL_CONTEXT_FLAGS",
    "SDL_GL_CONTEXT_PROFILE_MASK", "SDL_GL_SHARE_WITH_CURRENT_CONTEXT",
    "SDL_GL_FRAMEBUFFER_SRGB_CAPABLE", "SDL_GL_CONTEXT_RELEASE_BEHAVIOR",
    "SDL_GL_CONTEXT_RESET_NOTIFICATION", "SDL_GL_CONTEXT_NO_ERROR",

    "SDL_GLprofile",
    "SDL_GL_CONTEXT_PROFILE_CORE",
    "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY",
    "SDL_GL_CONTEXT_PROFILE_ES",
           
    "SDL_GLcontextFlag",
    "SDL_GL_CONTEXT_DEBUG_FLAG",
    "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG",
    "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG",
    "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG",

    "SDL_GLcontextReleaseFlag",
    "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE",
    "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH",

    "SDL_GLContextResetNotification",
    "SDL_GL_CONTEXT_RESET_NO_NOTIFICATION",
    "SDL_GL_CONTEXT_RESET_LOSE_CONTEXT",

    "SDL_HitTestResult",
    "SDL_HITTEST_NORMAL", "SDL_HITTEST_DRAGGABLE", 
    "SDL_HITTEST_RESIZE_TOPLEFT", "SDL_HITTEST_RESIZE_TOP",
    "SDL_HITTEST_RESIZE_TOPRIGHT", "SDL_HITTEST_RESIZE_RIGHT",
    "SDL_HITTEST_RESIZE_BOTTOMRIGHT", "SDL_HITTEST_RESIZE_BOTTOM",
    "SDL_HITTEST_RESIZE_BOTTOMLEFT", "SDL_HITTEST_RESIZE_LEFT",

    # Macro Functions
    "SDL_WINDOWPOS_UNDEFINED_MASK", "SDL_WINDOWPOS_UNDEFINED_DISPLAY",
    "SDL_WINDOWPOS_UNDEFINED", "SDL_WINDOWPOS_ISUNDEFINED",
    "SDL_WINDOWPOS_CENTERED_MASK", "SDL_WINDOWPOS_CENTERED_DISPLAY",
    "SDL_WINDOWPOS_CENTERED", "SDL_WINDOWPOS_ISCENTERED",

    # Functions
    "SDL_GetNumVideoDrivers", "SDL_GetVideoDriver",
    "SDL_VideoInit", "SDL_VideoQuit",
    "SDL_GetCurrentVideoDriver", "SDL_GetNumVideoDisplays",
    "SDL_GetDisplayName", "SDL_GetDisplayBounds",
    "SDL_GetDisplayOrientation", "SDL_GetNumDisplayModes",
    "SDL_GetDisplayMode",
    "SDL_GetDesktopDisplayMode", "SDL_GetCurrentDisplayMode",
    "SDL_GetClosestDisplayMode", "SDL_GetWindowDisplayIndex",
    "SDL_SetWindowDisplayMode", "SDL_GetWindowDisplayMode",
    "SDL_GetWindowICCProfile",
    "SDL_GetWindowPixelFormat", "SDL_CreateWindow",
    "SDL_CreateWindowFrom", "SDL_GetWindowID", "SDL_GetWindowFromID",
    "SDL_GetWindowFlags", "SDL_SetWindowTitle", "SDL_GetWindowTitle",
    "SDL_SetWindowIcon", "SDL_SetWindowData", "SDL_GetWindowData",
    "SDL_SetWindowPosition", "SDL_GetWindowPosition",
    "SDL_SetWindowSize", "SDL_GetWindowSize",
    "SDL_SetWindowMinimumSize", "SDL_GetWindowMinimumSize",
    "SDL_SetWindowMaximumSize", "SDL_GetWindowMaximumSize",
    "SDL_SetWindowBordered", "SDL_ShowWindow", "SDL_HideWindow",
    "SDL_RaiseWindow", "SDL_MaximizeWindow", "SDL_MinimizeWindow",
    "SDL_RestoreWindow", "SDL_SetWindowFullscreen",
    "SDL_GetWindowSurface", "SDL_UpdateWindowSurface",
    "SDL_UpdateWindowSurfaceRects", "SDL_SetWindowGrab",
    "SDL_SetWindowKeyboardGrab", "SDL_SetWindowMouseGrab",
    "SDL_GetWindowGrab", "SDL_GetWindowKeyboardGrab",
    "SDL_GetWindowMouseGrab", "SDL_GetGrabbedWindow",
    "SDL_SetWindowMouseRect", "SDL_GetWindowMouseRect",
    "SDL_SetWindowBrightness", "SDL_GetWindowBrightness",
    "SDL_SetWindowGammaRamp", "SDL_GetWindowGammaRamp", 
    "SDL_FlashWindow", "SDL_DestroyWindow",
    "SDL_DisableScreenSaver", "SDL_IsScreenSaverEnabled",
    "SDL_EnableScreenSaver",
    "SDL_SetWindowHitTest", "SDL_GL_LoadLibrary",
    "SDL_GL_GetProcAddress", "SDL_GL_UnloadLibrary",
    "SDL_GL_ExtensionSupported", "SDL_GL_SetAttribute",
    "SDL_GL_GetAttribute", "SDL_GL_CreateContext",
    "SDL_GL_MakeCurrent", "SDL_GL_SetSwapInterval",
    "SDL_GL_GetSwapInterval", "SDL_GL_SwapWindow",
    "SDL_GL_GetDrawableSize", "SDL_GL_GetCurrentWindow",
    "SDL_GL_GetCurrentContext", "SDL_GL_DeleteContext",
    "SDL_GL_ResetAttributes",
    "SDL_GetDisplayDPI", "SDL_GetDisplayUsableBounds",
    "SDL_GetWindowBordersSize", "SDL_GetWindowOpacity",
    "SDL_SetWindowOpacity", "SDL_SetWindowInputFocus",
    "SDL_SetWindowModalFor", "SDL_SetWindowResizable",
    "SDL_SetWindowAlwaysOnTop",

    # Callback Functions
    "SDL_HitTest"
]


class SDL_DisplayMode(Structure):
    _fields_ = [("format", Uint32),
                ("w", c_int),
                ("h", c_int),
                ("refresh_rate", c_int),
                ("driverdata", c_void_p)
               ]

    def __init__(self, format_=0, w=0, h=0, refresh_rate=0):
        super(SDL_DisplayMode, self).__init__()
        self.format = format_
        self.w = w
        self.h = h
        self.refresh_rate = refresh_rate

    def __repr__(self):
        return "SDL_DisplayMode(format=%d, w=%d, h=%d, refresh_rate=%d)" % \
            (self.format, self.w, self.h, self.refresh_rate)

    def __eq__(self, mode):
        return self.format == mode.format and self.w == mode.w and \
            self.h == mode.h and self.refresh_rate == mode.refresh_rate

    def __ne__(self, mode):
        return self.format != mode.format or self.w != mode.w or \
            self.h != mode.h or self.refresh_rate != mode.refresh_rate


class SDL_Window(c_void_p):
    pass


SDL_WindowFlags = c_int

SDL_WINDOW_FULLSCREEN = 0x00000001
SDL_WINDOW_OPENGL = 0x00000002
SDL_WINDOW_SHOWN = 0x00000004
SDL_WINDOW_HIDDEN = 0x00000008
SDL_WINDOW_BORDERLESS = 0x00000010
SDL_WINDOW_RESIZABLE = 0x00000020
SDL_WINDOW_MINIMIZED = 0x00000040
SDL_WINDOW_MAXIMIZED = 0x00000080
SDL_WINDOW_MOUSE_GRABBED = 0x00000100
SDL_WINDOW_INPUT_GRABBED = SDL_WINDOW_MOUSE_GRABBED  # for < 2.0.16
SDL_WINDOW_INPUT_FOCUS = 0x00000200
SDL_WINDOW_MOUSE_FOCUS = 0x00000400
SDL_WINDOW_FULLSCREEN_DESKTOP = (SDL_WINDOW_FULLSCREEN | 0x00001000)
SDL_WINDOW_FOREIGN = 0x00000800
SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000
SDL_WINDOW_MOUSE_CAPTURE = 0x00004000
SDL_WINDOW_ALWAYS_ON_TOP = 0x00008000
SDL_WINDOW_SKIP_TASKBAR  = 0x00010000
SDL_WINDOW_UTILITY = 0x00020000
SDL_WINDOW_TOOLTIP = 0x00040000
SDL_WINDOW_POPUP_MENU = 0x00080000
SDL_WINDOW_KEYBOARD_GRABBED = 0x00100000
SDL_WINDOW_VULKAN = 0x10000000
SDL_WINDOW_METAL = 0x20000000

SDL_WINDOWPOS_UNDEFINED_MASK = 0x1FFF0000
SDL_WINDOWPOS_UNDEFINED_DISPLAY = lambda x: (SDL_WINDOWPOS_UNDEFINED_MASK | x)
SDL_WINDOWPOS_UNDEFINED = SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)
SDL_WINDOWPOS_ISUNDEFINED = lambda x: ((x & 0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

SDL_WINDOWPOS_CENTERED_MASK = 0x2FFF0000
SDL_WINDOWPOS_CENTERED_DISPLAY = lambda x: (SDL_WINDOWPOS_CENTERED_MASK | x)
SDL_WINDOWPOS_CENTERED = SDL_WINDOWPOS_CENTERED_DISPLAY(0)
SDL_WINDOWPOS_ISCENTERED = lambda x: ((x & 0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)


SDL_WindowEventID = c_int

SDL_WINDOWEVENT_NONE = 0
SDL_WINDOWEVENT_SHOWN = 1
SDL_WINDOWEVENT_HIDDEN = 2
SDL_WINDOWEVENT_EXPOSED = 3
SDL_WINDOWEVENT_MOVED = 4
SDL_WINDOWEVENT_RESIZED = 5
SDL_WINDOWEVENT_SIZE_CHANGED = 6
SDL_WINDOWEVENT_MINIMIZED = 7
SDL_WINDOWEVENT_MAXIMIZED = 8
SDL_WINDOWEVENT_RESTORED = 9
SDL_WINDOWEVENT_ENTER = 10
SDL_WINDOWEVENT_LEAVE = 11
SDL_WINDOWEVENT_FOCUS_GAINED = 12
SDL_WINDOWEVENT_FOCUS_LOST = 13
SDL_WINDOWEVENT_CLOSE = 14
SDL_WINDOWEVENT_TAKE_FOCUS = 15
SDL_WINDOWEVENT_HIT_TEST = 16
SDL_WINDOWEVENT_ICCPROF_CHANGED = 17
SDL_WINDOWEVENT_DISPLAY_CHANGED = 18


SDL_DisplayEventID = c_int

SDL_DISPLAYEVENT_NONE = 0
SDL_DISPLAYEVENT_ORIENTATION = 1
SDL_DISPLAYEVENT_CONNECTED = 2
SDL_DISPLAYEVENT_DISCONNECTED = 3


SDL_DisplayOrientation = c_int

SDL_ORIENTATION_UNKNOWN = 0
SDL_ORIENTATION_LANDSCAPE = 1
SDL_ORIENTATION_LANDSCAPE_FLIPPED = 2
SDL_ORIENTATION_PORTRAIT = 3
SDL_ORIENTATION_PORTRAIT_FLIPPED = 4


SDL_FlashOperation = c_int

SDL_FLASH_CANCEL = 0
SDL_FLASH_BRIEFLY = 1
SDL_FLASH_UNTIL_FOCUSED = 2


SDL_GLContext = c_void_p


SDL_GLattr = c_int

SDL_GL_RED_SIZE = 0
SDL_GL_GREEN_SIZE = 1
SDL_GL_BLUE_SIZE = 2
SDL_GL_ALPHA_SIZE = 3
SDL_GL_BUFFER_SIZE = 4
SDL_GL_DOUBLEBUFFER = 5
SDL_GL_DEPTH_SIZE = 6
SDL_GL_STENCIL_SIZE = 7
SDL_GL_ACCUM_RED_SIZE = 8
SDL_GL_ACCUM_GREEN_SIZE = 9
SDL_GL_ACCUM_BLUE_SIZE = 10
SDL_GL_ACCUM_ALPHA_SIZE = 11
SDL_GL_STEREO = 12
SDL_GL_MULTISAMPLEBUFFERS = 13
SDL_GL_MULTISAMPLESAMPLES = 14
SDL_GL_ACCELERATED_VISUAL = 15
SDL_GL_RETAINED_BACKING = 16
SDL_GL_CONTEXT_MAJOR_VERSION = 17
SDL_GL_CONTEXT_MINOR_VERSION = 18
SDL_GL_CONTEXT_EGL = 19
SDL_GL_CONTEXT_FLAGS = 20
SDL_GL_CONTEXT_PROFILE_MASK = 21
SDL_GL_SHARE_WITH_CURRENT_CONTEXT = 22
SDL_GL_FRAMEBUFFER_SRGB_CAPABLE = 23
SDL_GL_CONTEXT_RELEASE_BEHAVIOR = 24
SDL_GL_CONTEXT_RESET_NOTIFICATION = 25
SDL_GL_CONTEXT_NO_ERROR = 26


SDL_GLprofile = c_int

SDL_GL_CONTEXT_PROFILE_CORE = 0x0001
SDL_GL_CONTEXT_PROFILE_COMPATIBILITY = 0x0002
SDL_GL_CONTEXT_PROFILE_ES = 0x0004


SDL_GLcontextFlag = c_int

SDL_GL_CONTEXT_DEBUG_FLAG = 0x0001
SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG = 0x0002
SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG = 0x0004
SDL_GL_CONTEXT_RESET_ISOLATION_FLAG = 0x0008


SDL_GLcontextReleaseFlag = c_int

SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE = 0x0000
SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH = 0x0001


SDL_GLContextResetNotification = c_int

SDL_GL_CONTEXT_RESET_NO_NOTIFICATION = 0x0000
SDL_GL_CONTEXT_RESET_LOSE_CONTEXT = 0x0001


SDL_HitTestResult = c_int

SDL_HITTEST_NORMAL = 0
SDL_HITTEST_DRAGGABLE = 1
SDL_HITTEST_RESIZE_TOPLEFT = 2
SDL_HITTEST_RESIZE_TOP = 3
SDL_HITTEST_RESIZE_TOPRIGHT = 4
SDL_HITTEST_RESIZE_RIGHT = 5
SDL_HITTEST_RESIZE_BOTTOMRIGHT = 6
SDL_HITTEST_RESIZE_BOTTOM = 7
SDL_HITTEST_RESIZE_BOTTOMLEFT = 8
SDL_HITTEST_RESIZE_LEFT = 9


SDL_HitTest = CFUNCTYPE(SDL_HitTestResult, POINTER(SDL_Window), POINTER(SDL_Point), c_void_p)

SDL_GetNumVideoDrivers = _bind("SDL_GetNumVideoDrivers", None, c_int)
SDL_GetVideoDriver = _bind("SDL_GetVideoDriver", [c_int], c_char_p)
SDL_VideoInit = _bind("SDL_VideoInit", [c_char_p], c_int)
SDL_VideoQuit = _bind("SDL_VideoQuit")
SDL_GetCurrentVideoDriver = _bind("SDL_GetCurrentVideoDriver", None, c_char_p)
SDL_GetNumVideoDisplays = _bind("SDL_GetNumVideoDisplays", None, c_int)
SDL_GetDisplayName = _bind("SDL_GetDisplayName", [c_int], c_char_p)
SDL_GetDisplayBounds = _bind("SDL_GetDisplayBounds", [c_int, POINTER(SDL_Rect)], c_int)
SDL_GetDisplayOrientation = _bind("SDL_GetDisplayOrientation", [c_int], SDL_DisplayOrientation, added='2.0.9')
SDL_GetNumDisplayModes = _bind("SDL_GetNumDisplayModes", [c_int], c_int)
SDL_GetDisplayMode = _bind("SDL_GetDisplayMode", [c_int, c_int, POINTER(SDL_DisplayMode)], c_int)
SDL_GetDesktopDisplayMode = _bind("SDL_GetDesktopDisplayMode", [c_int, POINTER(SDL_DisplayMode)], c_int)
SDL_GetCurrentDisplayMode = _bind("SDL_GetCurrentDisplayMode", [c_int, POINTER(SDL_DisplayMode)], c_int)
SDL_GetClosestDisplayMode = _bind("SDL_GetClosestDisplayMode", [c_int, POINTER(SDL_DisplayMode), POINTER(SDL_DisplayMode)], POINTER(SDL_DisplayMode))
SDL_GetWindowDisplayIndex = _bind("SDL_GetWindowDisplayIndex", [POINTER(SDL_Window)], c_int)
SDL_SetWindowDisplayMode = _bind("SDL_SetWindowDisplayMode", [POINTER(SDL_Window), POINTER(SDL_DisplayMode)], c_int)
SDL_GetWindowDisplayMode = _bind("SDL_GetWindowDisplayMode", [POINTER(SDL_Window), POINTER(SDL_DisplayMode)], c_int)
SDL_GetWindowICCProfile = _bind("SDL_GetWindowICCProfile", [POINTER(SDL_Window), POINTER(c_size_t)], c_void_p, added='2.0.18')
SDL_GetWindowPixelFormat = _bind("SDL_GetWindowPixelFormat", [POINTER(SDL_Window)], Uint32)
SDL_CreateWindow = _bind("SDL_CreateWindow", [c_char_p, c_int, c_int, c_int, c_int, Uint32], POINTER(SDL_Window))
SDL_CreateWindowFrom = _bind("SDL_CreateWindowFrom", [c_void_p], POINTER(SDL_Window))
SDL_GetWindowID = _bind("SDL_GetWindowID", [POINTER(SDL_Window)], Uint32)
SDL_GetWindowFromID = _bind("SDL_GetWindowFromID", [Uint32], POINTER(SDL_Window))
SDL_GetWindowFlags = _bind("SDL_GetWindowFlags", [POINTER(SDL_Window)], Uint32)
SDL_SetWindowTitle = _bind("SDL_SetWindowTitle", [POINTER(SDL_Window), c_char_p])
SDL_GetWindowTitle = _bind("SDL_GetWindowTitle", [POINTER(SDL_Window)], c_char_p)
SDL_SetWindowIcon = _bind("SDL_SetWindowIcon", [POINTER(SDL_Window), POINTER(SDL_Surface)])
SDL_SetWindowData = _bind("SDL_SetWindowData", [POINTER(SDL_Window), c_char_p, POINTER(py_object)], POINTER(py_object))
SDL_GetWindowData = _bind("SDL_GetWindowData", [POINTER(SDL_Window), c_char_p], POINTER(py_object))
SDL_SetWindowPosition = _bind("SDL_SetWindowPosition", [POINTER(SDL_Window), c_int, c_int])
SDL_GetWindowPosition = _bind("SDL_GetWindowPosition", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)])
SDL_SetWindowSize = _bind("SDL_SetWindowSize", [POINTER(SDL_Window), c_int, c_int])
SDL_GetWindowSize = _bind("SDL_GetWindowSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)])
SDL_SetWindowMinimumSize = _bind("SDL_SetWindowMinimumSize", [POINTER(SDL_Window), c_int, c_int])
SDL_GetWindowMinimumSize = _bind("SDL_GetWindowMinimumSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)])
SDL_SetWindowMaximumSize = _bind("SDL_SetWindowMaximumSize", [POINTER(SDL_Window), c_int, c_int])
SDL_GetWindowMaximumSize = _bind("SDL_GetWindowMaximumSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)])
SDL_SetWindowBordered = _bind("SDL_SetWindowBordered", [POINTER(SDL_Window), SDL_bool])
SDL_ShowWindow = _bind("SDL_ShowWindow", [POINTER(SDL_Window)])
SDL_HideWindow = _bind("SDL_HideWindow", [POINTER(SDL_Window)])
SDL_RaiseWindow = _bind("SDL_RaiseWindow", [POINTER(SDL_Window)])
SDL_MaximizeWindow = _bind("SDL_MaximizeWindow", [POINTER(SDL_Window)])
SDL_MinimizeWindow = _bind("SDL_MinimizeWindow", [POINTER(SDL_Window)])
SDL_RestoreWindow = _bind("SDL_RestoreWindow", [POINTER(SDL_Window)])
SDL_SetWindowFullscreen = _bind("SDL_SetWindowFullscreen", [POINTER(SDL_Window), Uint32], c_int)
SDL_GetWindowSurface = _bind("SDL_GetWindowSurface", [POINTER(SDL_Window)], POINTER(SDL_Surface))
SDL_UpdateWindowSurface = _bind("SDL_UpdateWindowSurface", [POINTER(SDL_Window)], c_int)
SDL_UpdateWindowSurfaceRects = _bind("SDL_UpdateWindowSurfaceRects", [POINTER(SDL_Window), POINTER(SDL_Rect), c_int], c_int)
SDL_SetWindowGrab = _bind("SDL_SetWindowGrab", [POINTER(SDL_Window), SDL_bool])
SDL_SetWindowKeyboardGrab = _bind("SDL_SetWindowKeyboardGrab", [POINTER(SDL_Window), SDL_bool], added='2.0.16')
SDL_SetWindowMouseGrab = _bind("SDL_SetWindowMouseGrab", [POINTER(SDL_Window), SDL_bool], added='2.0.16')
SDL_GetWindowGrab = _bind("SDL_GetWindowGrab", [POINTER(SDL_Window)], SDL_bool)
SDL_GetWindowKeyboardGrab = _bind("SDL_GetWindowKeyboardGrab", [POINTER(SDL_Window)], SDL_bool, added='2.0.16')
SDL_GetWindowMouseGrab = _bind("SDL_GetWindowMouseGrab", [POINTER(SDL_Window)], SDL_bool, added='2.0.16')
SDL_GetGrabbedWindow = _bind("SDL_GetGrabbedWindow", None, POINTER(SDL_Window), added='2.0.4')
SDL_SetWindowMouseRect = _bind("SDL_SetWindowMouseRect", [POINTER(SDL_Window), POINTER(SDL_Rect)], c_int, added='2.0.18')
SDL_GetWindowMouseRect = _bind("SDL_GetWindowMouseRect", [POINTER(SDL_Window)], POINTER(SDL_Rect), added='2.0.18')
SDL_SetWindowBrightness = _bind("SDL_SetWindowBrightness", [POINTER(SDL_Window), c_float], c_int)
SDL_GetWindowBrightness = _bind("SDL_GetWindowBrightness", [POINTER(SDL_Window)], c_float)
SDL_SetWindowGammaRamp = _bind("SDL_SetWindowGammaRamp", [POINTER(SDL_Window), POINTER(Uint16), POINTER(Uint16), POINTER(Uint16)], c_int)
SDL_GetWindowGammaRamp = _bind("SDL_GetWindowGammaRamp", [POINTER(SDL_Window), POINTER(Uint16), POINTER(Uint16), POINTER(Uint16)], c_int)
SDL_FlashWindow = _bind("SDL_FlashWindow", [POINTER(SDL_Window), SDL_FlashOperation], c_int, added='2.0.16')
SDL_DestroyWindow = _bind("SDL_DestroyWindow", [POINTER(SDL_Window)])
SDL_IsScreenSaverEnabled = _bind("SDL_IsScreenSaverEnabled", None, SDL_bool)
SDL_EnableScreenSaver = _bind("SDL_EnableScreenSaver")
SDL_DisableScreenSaver = _bind("SDL_DisableScreenSaver")
SDL_SetWindowHitTest = _bind("SDL_SetWindowHitTest", [POINTER(SDL_Window), SDL_HitTest, c_void_p], c_int, added='2.0.4')
SDL_GetDisplayDPI = _bind("SDL_GetDisplayDPI", [c_int, POINTER(c_float), POINTER(c_float), POINTER(c_float)], c_int, added='2.0.4')
SDL_GetDisplayUsableBounds = _bind("SDL_GetDisplayUsableBounds", [c_int, POINTER(SDL_Rect)], c_int, added='2.0.5')
SDL_GetWindowBordersSize = _bind("SDL_GetWindowBordersSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)], c_int, added='2.0.5')
SDL_GetWindowOpacity = _bind("SDL_GetWindowOpacity", [POINTER(SDL_Window), POINTER(c_float)], c_int, added='2.0.5')
SDL_SetWindowOpacity = _bind("SDL_SetWindowOpacity", [POINTER(SDL_Window), c_float], c_int, added='2.0.5')
SDL_SetWindowInputFocus = _bind("SDL_SetWindowInputFocus", [POINTER(SDL_Window)], c_int, added='2.0.5')
SDL_SetWindowModalFor = _bind("SDL_SetWindowModalFor", [POINTER(SDL_Window), POINTER(SDL_Window)], c_int, added='2.0.5')
SDL_SetWindowResizable = _bind("SDL_SetWindowResizable", [POINTER(SDL_Window), SDL_bool], added='2.0.5')
SDL_SetWindowAlwaysOnTop = _bind("SDL_SetWindowAlwaysOnTop", [POINTER(SDL_Window), SDL_bool], added='2.0.16')

SDL_GL_LoadLibrary = _bind("SDL_GL_LoadLibrary", [c_char_p], c_int)
SDL_GL_GetProcAddress = _bind("SDL_GL_GetProcAddress", [c_char_p], c_void_p)
SDL_GL_UnloadLibrary = _bind("SDL_GL_UnloadLibrary")
SDL_GL_ExtensionSupported = _bind("SDL_GL_ExtensionSupported", [c_char_p], SDL_bool)
SDL_GL_SetAttribute = _bind("SDL_GL_SetAttribute", [SDL_GLattr, c_int], c_int)
SDL_GL_GetAttribute = _bind("SDL_GL_GetAttribute", [SDL_GLattr, POINTER(c_int)], c_int)
SDL_GL_CreateContext = _bind("SDL_GL_CreateContext", [POINTER(SDL_Window)], SDL_GLContext)
SDL_GL_GetCurrentWindow = _bind("SDL_GL_GetCurrentWindow", None, POINTER(SDL_Window))
SDL_GL_MakeCurrent = _bind("SDL_GL_MakeCurrent", [POINTER(SDL_Window), SDL_GLContext], c_int)
SDL_GL_GetCurrentContext = _bind("SDL_GL_GetCurrentContext", None, SDL_GLContext)
SDL_GL_SetSwapInterval = _bind("SDL_GL_SetSwapInterval", [c_int], c_int)
SDL_GL_GetSwapInterval = _bind("SDL_GL_GetSwapInterval", None, c_int)
SDL_GL_SwapWindow = _bind("SDL_GL_SwapWindow", [POINTER(SDL_Window)])
SDL_GL_DeleteContext = _bind("SDL_GL_DeleteContext", [SDL_GLContext])
SDL_GL_GetDrawableSize = _bind("SDL_GL_GetDrawableSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)])
SDL_GL_ResetAttributes = _bind("SDL_GL_ResetAttributes")
