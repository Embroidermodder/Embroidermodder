from ctypes import Structure, POINTER, c_float, c_int
from .dll import _bind
from .stdinc import Sint64

__all__ = [
    # Structs
    "SDL_Finger",

    # Defines
    "SDL_TouchID", "SDL_FingerID", 
    "SDL_TOUCH_MOUSEID", "SDL_MOUSE_TOUCHID",

    # Enums
    "SDL_TouchDeviceType",
    "SDL_TOUCH_DEVICE_INVALID", "SDL_TOUCH_DEVICE_DIRECT",
    "SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE",
    "SDL_TOUCH_DEVICE_INDIRECT_RELATIVE",

    # Functions
    "SDL_GetNumTouchDevices", "SDL_GetTouchDevice",
    "SDL_GetTouchDeviceType", "SDL_GetNumTouchFingers",
    "SDL_GetTouchFinger"
]


SDL_TouchID = Sint64
SDL_FingerID = Sint64

SDL_TouchDeviceType = c_int
SDL_TOUCH_DEVICE_INVALID = -1,
SDL_TOUCH_DEVICE_DIRECT = 0
SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE = 1
SDL_TOUCH_DEVICE_INDIRECT_RELATIVE = 2

class SDL_Finger(Structure):
    _fields_ = [("id", SDL_FingerID),
                ("x", c_float),
                ("y", c_float),
                ("pressure", c_float)
                ]

SDL_TOUCH_MOUSEID = 2**32 - 1 # defined as ((Uint32)-1), hope this is right
SDL_MOUSE_TOUCHID = 2**63 - 1 # defined as ((Sint64)-1), hope this is right

SDL_GetNumTouchDevices = _bind("SDL_GetNumTouchDevices", None, c_int)
SDL_GetTouchDevice = _bind("SDL_GetTouchDevice", [c_int], SDL_TouchID)
SDL_GetTouchDeviceType = _bind("SDL_GetTouchDeviceType", [SDL_TouchID], SDL_TouchDeviceType, added='2.0.10')
SDL_GetNumTouchFingers = _bind("SDL_GetNumTouchFingers", [SDL_TouchID], c_int)
SDL_GetTouchFinger = _bind("SDL_GetTouchFinger", [SDL_TouchID, c_int], POINTER(SDL_Finger))
