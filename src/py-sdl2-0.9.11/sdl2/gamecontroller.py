import sys
from ctypes import (Structure, Union, c_int, c_char_p, c_void_p, c_float,
    POINTER, create_string_buffer)
from .dll import _bind
from .dll import version as sdl_version
from .stdinc import SDL_bool, Sint16, Uint32, Uint16, Uint8
from .joystick import (SDL_JoystickGUID, SDL_Joystick, SDL_JoystickID,
    SDL_JoystickGetGUIDString)
from .rwops import SDL_RWops, SDL_RWFromFile
from .sensor import SDL_SensorType

__all__ = [
    # Structs & Opaque Types
    "SDL_GameController", "SDL_GameControllerButtonBind",

    # Enums
    "SDL_GameControllerType",
    "SDL_CONTROLLER_TYPE_UNKNOWN", "SDL_CONTROLLER_TYPE_XBOX360",
    "SDL_CONTROLLER_TYPE_XBOXONE", "SDL_CONTROLLER_TYPE_PS3",
    "SDL_CONTROLLER_TYPE_PS4", "SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO",
    "SDL_CONTROLLER_TYPE_VIRTUAL", "SDL_CONTROLLER_TYPE_PS5",
    "SDL_CONTROLLER_TYPE_AMAZON_LUNA", "SDL_CONTROLLER_TYPE_GOOGLE_STADIA",

    "SDL_GameControllerBindType",
    "SDL_CONTROLLER_BINDTYPE_NONE", "SDL_CONTROLLER_BINDTYPE_BUTTON",
    "SDL_CONTROLLER_BINDTYPE_AXIS", "SDL_CONTROLLER_BINDTYPE_HAT", 

    "SDL_GameControllerAxis",
    "SDL_CONTROLLER_AXIS_INVALID", "SDL_CONTROLLER_AXIS_LEFTX",
    "SDL_CONTROLLER_AXIS_LEFTY", "SDL_CONTROLLER_AXIS_RIGHTX",
    "SDL_CONTROLLER_AXIS_RIGHTY", "SDL_CONTROLLER_AXIS_TRIGGERLEFT",
    "SDL_CONTROLLER_AXIS_TRIGGERRIGHT", "SDL_CONTROLLER_AXIS_MAX",  

    "SDL_GameControllerButton",
    "SDL_CONTROLLER_BUTTON_INVALID", "SDL_CONTROLLER_BUTTON_A",
    "SDL_CONTROLLER_BUTTON_B", "SDL_CONTROLLER_BUTTON_X",
    "SDL_CONTROLLER_BUTTON_Y", "SDL_CONTROLLER_BUTTON_BACK",
    "SDL_CONTROLLER_BUTTON_GUIDE", "SDL_CONTROLLER_BUTTON_START",
    "SDL_CONTROLLER_BUTTON_LEFTSTICK", "SDL_CONTROLLER_BUTTON_RIGHTSTICK",
    "SDL_CONTROLLER_BUTTON_LEFTSHOULDER",
    "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER",
    "SDL_CONTROLLER_BUTTON_DPAD_UP", "SDL_CONTROLLER_BUTTON_DPAD_DOWN",
    "SDL_CONTROLLER_BUTTON_DPAD_LEFT", "SDL_CONTROLLER_BUTTON_DPAD_RIGHT",
    "SDL_CONTROLLER_BUTTON_MISC1", "SDL_CONTROLLER_BUTTON_PADDLE1",
    "SDL_CONTROLLER_BUTTON_PADDLE2", "SDL_CONTROLLER_BUTTON_PADDLE3",
    "SDL_CONTROLLER_BUTTON_PADDLE4", "SDL_CONTROLLER_BUTTON_TOUCHPAD",
    "SDL_CONTROLLER_BUTTON_MAX",

    # Macro Functions
    "SDL_GameControllerAddMappingsFromFile",
    
    # Functions
    "SDL_GameControllerAddMappingsFromRW", "SDL_GameControllerAddMapping",
    "SDL_GameControllerNumMappings", "SDL_GameControllerMappingForIndex",
    "SDL_GameControllerMappingForGUID", "SDL_GameControllerMapping",
    "SDL_IsGameController", "SDL_GameControllerNameForIndex",
    "SDL_GameControllerTypeForIndex", "SDL_GameControllerMappingForDeviceIndex",
    "SDL_GameControllerOpen", "SDL_GameControllerFromInstanceID",
    "SDL_GameControllerFromPlayerIndex", "SDL_GameControllerName",
    "SDL_GameControllerGetType", 
    "SDL_GameControllerGetPlayerIndex", "SDL_GameControllerSetPlayerIndex",
    "SDL_GameControllerGetVendor", "SDL_GameControllerGetProduct",
    "SDL_GameControllerGetProductVersion", "SDL_GameControllerGetSerial",
    "SDL_GameControllerGetAttached", "SDL_GameControllerGetJoystick",
    "SDL_GameControllerEventState", "SDL_GameControllerUpdate",
    "SDL_GameControllerGetAxisFromString", "SDL_GameControllerGetStringForAxis",
    "SDL_GameControllerGetBindForAxis", "SDL_GameControllerHasAxis",
    "SDL_GameControllerGetAxis",
    "SDL_GameControllerGetButtonFromString",
    "SDL_GameControllerGetStringForButton",
    "SDL_GameControllerGetBindForButton", "SDL_GameControllerHasButton",
    "SDL_GameControllerGetButton",
    "SDL_GameControllerGetNumTouchpads",
    "SDL_GameControllerGetNumTouchpadFingers",
    "SDL_GameControllerGetTouchpadFinger",
    "SDL_GameControllerHasSensor", "SDL_GameControllerSetSensorEnabled",
    "SDL_GameControllerIsSensorEnabled", 
    "SDL_GameControllerGetSensorDataRate", "SDL_GameControllerGetSensorData",
    "SDL_GameControllerRumble", "SDL_GameControllerRumbleTriggers",
    "SDL_GameControllerHasLED", "SDL_GameControllerHasRumble",
    "SDL_GameControllerHasRumbleTriggers",
    "SDL_GameControllerSetLED",
    "SDL_GameControllerSendEffect",
    "SDL_GameControllerClose",
    "SDL_GameControllerGetAppleSFSymbolsNameForButton",
    "SDL_GameControllerGetAppleSFSymbolsNameForAxis",
]


class SDL_GameController(c_void_p):
    pass


SDL_GameControllerBindType = c_int

SDL_CONTROLLER_BINDTYPE_NONE = 0
SDL_CONTROLLER_BINDTYPE_BUTTON = 1
SDL_CONTROLLER_BINDTYPE_AXIS = 2
SDL_CONTROLLER_BINDTYPE_HAT = 3


SDL_GameControllerType = c_int

SDL_CONTROLLER_TYPE_UNKNOWN = 0
SDL_CONTROLLER_TYPE_XBOX360 = 1
SDL_CONTROLLER_TYPE_XBOXONE = 2
SDL_CONTROLLER_TYPE_PS3 = 3
SDL_CONTROLLER_TYPE_PS4 = 4
SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO = 5
SDL_CONTROLLER_TYPE_VIRTUAL = 6
SDL_CONTROLLER_TYPE_PS5 = 7
SDL_CONTROLLER_TYPE_AMAZON_LUNA = 8
SDL_CONTROLLER_TYPE_GOOGLE_STADIA = 9


class _gchat(Structure):
    _fields_ = [("hat", c_int), ("hat_mask", c_int)]

class _gcvalue(Union):
    _fields_ = [("button", c_int), ("axis", c_int), ("hat", _gchat)]

class SDL_GameControllerButtonBind(Structure):
    _fields_ = [("bindType", SDL_GameControllerBindType), ("value", _gcvalue)]


SDL_GameControllerAddMapping = _bind("SDL_GameControllerAddMapping", [c_char_p], c_int)
SDL_GameControllerMapping = _bind("SDL_GameControllerMapping", [POINTER(SDL_GameController)], c_char_p)
SDL_IsGameController = _bind("SDL_IsGameController", [c_int], SDL_bool)
SDL_GameControllerNameForIndex = _bind("SDL_GameControllerNameForIndex", [c_int], c_char_p)
SDL_GameControllerTypeForIndex = _bind("SDL_GameControllerTypeForIndex", [c_int], SDL_GameControllerType, added='2.0.12')
SDL_GameControllerOpen = _bind("SDL_GameControllerOpen", [c_int], POINTER(SDL_GameController))
SDL_GameControllerName = _bind("SDL_GameControllerName", [POINTER(SDL_GameController)], c_char_p)
SDL_GameControllerGetType = _bind("SDL_GameControllerGetType", [POINTER(SDL_GameController)], SDL_GameControllerType, added='2.0.12')
SDL_GameControllerGetAttached = _bind("SDL_GameControllerGetAttached", [POINTER(SDL_GameController)], SDL_bool)
SDL_GameControllerGetJoystick = _bind("SDL_GameControllerGetJoystick", [POINTER(SDL_GameController)], POINTER(SDL_Joystick))
SDL_GameControllerEventState = _bind("SDL_GameControllerEventState", [c_int], c_int)
SDL_GameControllerUpdate = _bind("SDL_GameControllerUpdate")


SDL_GameControllerAxis = c_int

SDL_CONTROLLER_AXIS_INVALID = -1
SDL_CONTROLLER_AXIS_LEFTX = 0
SDL_CONTROLLER_AXIS_LEFTY = 1
SDL_CONTROLLER_AXIS_RIGHTX = 2
SDL_CONTROLLER_AXIS_RIGHTY = 3
SDL_CONTROLLER_AXIS_TRIGGERLEFT = 4
SDL_CONTROLLER_AXIS_TRIGGERRIGHT = 5
SDL_CONTROLLER_AXIS_MAX = 6


SDL_GameControllerGetAxisFromString = _bind("SDL_GameControllerGetAxisFromString", [c_char_p], SDL_GameControllerAxis)
SDL_GameControllerGetStringForAxis = _bind("SDL_GameControllerGetStringForAxis", [SDL_GameControllerAxis], c_char_p)
SDL_GameControllerGetBindForAxis = _bind("SDL_GameControllerGetBindForAxis", [POINTER(SDL_GameController), SDL_GameControllerAxis], SDL_GameControllerButtonBind)
SDL_GameControllerHasAxis = _bind("SDL_GameControllerHasAxis", [POINTER(SDL_GameController), SDL_GameControllerAxis], SDL_bool, added='2.0.14')
SDL_GameControllerGetAxis = _bind("SDL_GameControllerGetAxis", [POINTER(SDL_GameController), SDL_GameControllerAxis], Sint16)


SDL_GameControllerButton = c_int

SDL_CONTROLLER_BUTTON_INVALID = -1
SDL_CONTROLLER_BUTTON_A = 0
SDL_CONTROLLER_BUTTON_B = 1
SDL_CONTROLLER_BUTTON_X = 2
SDL_CONTROLLER_BUTTON_Y = 3
SDL_CONTROLLER_BUTTON_BACK = 4
SDL_CONTROLLER_BUTTON_GUIDE = 5
SDL_CONTROLLER_BUTTON_START = 6
SDL_CONTROLLER_BUTTON_LEFTSTICK = 7
SDL_CONTROLLER_BUTTON_RIGHTSTICK = 8
SDL_CONTROLLER_BUTTON_LEFTSHOULDER = 9
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER = 10
SDL_CONTROLLER_BUTTON_DPAD_UP = 11
SDL_CONTROLLER_BUTTON_DPAD_DOWN = 12
SDL_CONTROLLER_BUTTON_DPAD_LEFT = 13
SDL_CONTROLLER_BUTTON_DPAD_RIGHT = 14
SDL_CONTROLLER_BUTTON_MISC1 = 15    
SDL_CONTROLLER_BUTTON_PADDLE1 = 16
SDL_CONTROLLER_BUTTON_PADDLE2 = 17
SDL_CONTROLLER_BUTTON_PADDLE3 = 18
SDL_CONTROLLER_BUTTON_PADDLE4 = 19
SDL_CONTROLLER_BUTTON_TOUCHPAD = 20
SDL_CONTROLLER_BUTTON_MAX = 21
if sdl_version < 2014:
    SDL_CONTROLLER_BUTTON_MAX = 15  # For backwards compatibility


SDL_GameControllerGetButtonFromString = _bind("SDL_GameControllerGetButtonFromString", [c_char_p], SDL_GameControllerButton)
SDL_GameControllerGetStringForButton = _bind("SDL_GameControllerGetStringForButton", [SDL_GameControllerButton], c_char_p)
SDL_GameControllerGetBindForButton = _bind("SDL_GameControllerGetBindForButton", [POINTER(SDL_GameController), SDL_GameControllerButton], SDL_GameControllerButtonBind)
SDL_GameControllerHasButton = _bind("SDL_GameControllerHasButton", [POINTER(SDL_GameController), SDL_GameControllerButton], SDL_bool, added='2.0.14')
SDL_GameControllerGetButton = _bind("SDL_GameControllerGetButton", [POINTER(SDL_GameController), SDL_GameControllerButton], Uint8)
SDL_GameControllerGetNumTouchpads = _bind("SDL_GameControllerGetNumTouchpads", [POINTER(SDL_GameController)], c_int, added='2.0.14')
SDL_GameControllerGetNumTouchpadFingers = _bind("SDL_GameControllerGetNumTouchpadFingers", [POINTER(SDL_GameController), c_int], c_int, added='2.0.14')
SDL_GameControllerGetTouchpadFinger = _bind("SDL_GameControllerGetTouchpadFinger", [POINTER(SDL_GameController), c_int, c_int, POINTER(Uint8), POINTER(c_float), POINTER(c_float), POINTER(c_float)], c_int, added='2.0.14')
SDL_GameControllerHasSensor = _bind("SDL_GameControllerHasSensor", [POINTER(SDL_GameController), SDL_SensorType], SDL_bool, added='2.0.14')
SDL_GameControllerSetSensorEnabled = _bind("SDL_GameControllerSetSensorEnabled", [POINTER(SDL_GameController), SDL_SensorType, SDL_bool], c_int, added='2.0.14')
SDL_GameControllerIsSensorEnabled = _bind("SDL_GameControllerIsSensorEnabled", [POINTER(SDL_GameController), SDL_SensorType], SDL_bool, added='2.0.14')
SDL_GameControllerGetSensorDataRate = _bind("SDL_GameControllerGetSensorDataRate", [POINTER(SDL_GameController), SDL_SensorType], c_float, added='2.0.16')
# TODO: Read how GetSensorData is implemented to figure out how the # of floats is determined
SDL_GameControllerGetSensorData = _bind("SDL_GameControllerGetSensorData", [POINTER(SDL_GameController), SDL_SensorType, POINTER(c_float), c_int], c_int, added='2.0.14')
SDL_GameControllerAddMappingsFromRW = _bind("SDL_GameControllerAddMappingsFromRW", [POINTER(SDL_RWops), c_int], c_int)
SDL_GameControllerAddMappingsFromFile = lambda fname: SDL_GameControllerAddMappingsFromRW(SDL_RWFromFile(fname, b"rb"), 1)
SDL_GameControllerFromInstanceID = _bind("SDL_GameControllerFromInstanceID", [SDL_JoystickID], POINTER(SDL_GameController), added='2.0.4')
SDL_GameControllerFromPlayerIndex = _bind("SDL_GameControllerFromPlayerIndex", [c_int], POINTER(SDL_GameController), added='2.0.12')
SDL_GameControllerGetPlayerIndex = _bind("SDL_GameControllerGetPlayerIndex", [POINTER(SDL_GameController)], c_int, added='2.0.9')
SDL_GameControllerSetPlayerIndex = _bind("SDL_GameControllerSetPlayerIndex", [POINTER(SDL_GameController), c_int], added='2.0.12')
SDL_GameControllerGetVendor = _bind("SDL_GameControllerGetVendor", [POINTER(SDL_GameController)], Uint16, added='2.0.6')
SDL_GameControllerGetProduct = _bind("SDL_GameControllerGetProduct", [POINTER(SDL_GameController)], Uint16, added='2.0.6')
SDL_GameControllerGetProductVersion = _bind("SDL_GameControllerGetProductVersion", [POINTER(SDL_GameController)], Uint16, added='2.0.6')
SDL_GameControllerGetSerial = _bind("SDL_GameControllerGetSerial", [POINTER(SDL_GameController)], c_char_p, added='2.0.14')
SDL_GameControllerNumMappings = _bind("SDL_GameControllerNumMappings", None, c_int, added='2.0.6')
SDL_GameControllerMappingForIndex = _bind("SDL_GameControllerMappingForIndex", [c_int], c_char_p, added='2.0.6')
SDL_GameControllerMappingForDeviceIndex = _bind("SDL_GameControllerMappingForDeviceIndex", [c_int], c_char_p, added='2.0.9')
SDL_GameControllerRumble = _bind("SDL_GameControllerRumble", [POINTER(SDL_GameController), Uint16, Uint16, Uint32], c_int, added='2.0.9')
SDL_GameControllerRumbleTriggers = _bind("SDL_GameControllerRumbleTriggers", [POINTER(SDL_GameController), Uint16, Uint16, Uint32], c_int, added='2.0.14')
SDL_GameControllerHasLED = _bind("SDL_GameControllerHasLED", [POINTER(SDL_GameController)], SDL_bool, added='2.0.14')
SDL_GameControllerHasRumble = _bind("SDL_GameControllerHasRumble", [POINTER(SDL_GameController)], SDL_bool, added='2.0.18')
SDL_GameControllerHasRumbleTriggers = _bind("SDL_GameControllerHasRumbleTriggers", [POINTER(SDL_GameController)], SDL_bool, added='2.0.18')
SDL_GameControllerSetLED = _bind("SDL_GameControllerSetLED", [POINTER(SDL_GameController), Uint8, Uint8, Uint8], c_int, added='2.0.14')
SDL_GameControllerSendEffect = _bind("SDL_GameControllerSendEffect", [POINTER(SDL_GameController), c_void_p, c_int], c_int, added='2.0.16')
SDL_GameControllerClose = _bind("SDL_GameControllerClose", [POINTER(SDL_GameController)])
SDL_GameControllerGetAppleSFSymbolsNameForButton = _bind("SDL_GameControllerGetAppleSFSymbolsNameForButton", [POINTER(SDL_GameController), SDL_GameControllerButton], c_char_p, added='2.0.18')
SDL_GameControllerGetAppleSFSymbolsNameForAxis = _bind("SDL_GameControllerGetAppleSFSymbolsNameForAxis", [POINTER(SDL_GameController), SDL_GameControllerAxis], c_char_p, added='2.0.18')

# Reimplemented w/ other functions due to crash-causing ctypes bug (fixed in 3.8)
if sys.version_info >= (3, 8, 0, 'final'):
    SDL_GameControllerMappingForGUID = _bind("SDL_GameControllerMappingForGUID", [SDL_JoystickGUID], c_char_p)
else:
    def SDL_GameControllerMappingForGUID(guid):
        buff = create_string_buffer(33)
        SDL_JoystickGetGUIDString(guid, buff, 33) # Get GUID string
        guid_str = buff.value
        # Iterate over controller mappings and look for a GUID match
        # Note: iterates in reverse, so user-defined mappings are checked first
        num = SDL_GameControllerNumMappings()
        for i in range(num - 1, -1, -1): 
            m = SDL_GameControllerMappingForIndex(i)
            if m.split(b',')[0] == guid_str:
                return m
        return None
