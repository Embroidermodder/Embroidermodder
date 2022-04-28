from ctypes import c_int, c_char_p, c_void_p, CFUNCTYPE, POINTER
from .dll import _bind

__all__ = [
    # Defines
    "SDL_MAX_LOG_MESSAGE", 
    
    # Enums
    "SDL_LogCategory",
    "SDL_LOG_CATEGORY_APPLICATION",
    "SDL_LOG_CATEGORY_ERROR", "SDL_LOG_CATEGORY_ASSERT",
    "SDL_LOG_CATEGORY_SYSTEM", "SDL_LOG_CATEGORY_AUDIO",
    "SDL_LOG_CATEGORY_VIDEO", "SDL_LOG_CATEGORY_RENDER",
    "SDL_LOG_CATEGORY_INPUT", "SDL_LOG_CATEGORY_TEST",
    "SDL_LOG_CATEGORY_RESERVED1", "SDL_LOG_CATEGORY_RESERVED2",
    "SDL_LOG_CATEGORY_RESERVED3", "SDL_LOG_CATEGORY_RESERVED4",
    "SDL_LOG_CATEGORY_RESERVED5", "SDL_LOG_CATEGORY_RESERVED6",
    "SDL_LOG_CATEGORY_RESERVED7", "SDL_LOG_CATEGORY_RESERVED8",
    "SDL_LOG_CATEGORY_RESERVED9", "SDL_LOG_CATEGORY_RESERVED10",
    "SDL_LOG_CATEGORY_CUSTOM",

    "SDL_LogPriority",
    "SDL_LOG_PRIORITY_VERBOSE",
    "SDL_LOG_PRIORITY_DEBUG", "SDL_LOG_PRIORITY_INFO",
    "SDL_LOG_PRIORITY_WARN", "SDL_LOG_PRIORITY_ERROR",
    "SDL_LOG_PRIORITY_CRITICAL", "SDL_NUM_LOG_PRIORITIES",
    
    # Functions
    "SDL_LogSetAllPriority", "SDL_LogSetPriority",
    "SDL_LogGetPriority", "SDL_LogResetPriorities", "SDL_Log",
    "SDL_LogVerbose", "SDL_LogDebug", "SDL_LogInfo", "SDL_LogWarn",
    "SDL_LogError", "SDL_LogCritical", "SDL_LogMessage",
    "SDL_LogGetOutputFunction", "SDL_LogSetOutputFunction",

    # Callback Functions
    "SDL_LogOutputFunction",
]

SDL_MAX_LOG_MESSAGE = 4096


SDL_LogCategory = c_int

SDL_LOG_CATEGORY_APPLICATION = 0
SDL_LOG_CATEGORY_ERROR = 1
SDL_LOG_CATEGORY_ASSERT = 2
SDL_LOG_CATEGORY_SYSTEM = 3
SDL_LOG_CATEGORY_AUDIO = 4
SDL_LOG_CATEGORY_VIDEO = 5
SDL_LOG_CATEGORY_RENDER = 6
SDL_LOG_CATEGORY_INPUT = 7
SDL_LOG_CATEGORY_TEST = 8
SDL_LOG_CATEGORY_RESERVED1 = 9
SDL_LOG_CATEGORY_RESERVED2 = 10
SDL_LOG_CATEGORY_RESERVED3 = 11
SDL_LOG_CATEGORY_RESERVED4 = 12
SDL_LOG_CATEGORY_RESERVED5 = 13
SDL_LOG_CATEGORY_RESERVED6 = 14
SDL_LOG_CATEGORY_RESERVED7 = 15
SDL_LOG_CATEGORY_RESERVED8 = 16
SDL_LOG_CATEGORY_RESERVED9 = 17
SDL_LOG_CATEGORY_RESERVED10 = 18
SDL_LOG_CATEGORY_CUSTOM = 19


SDL_LogPriority = c_int

SDL_LOG_PRIORITY_VERBOSE = 1
SDL_LOG_PRIORITY_DEBUG = 2
SDL_LOG_PRIORITY_INFO = 3
SDL_LOG_PRIORITY_WARN = 4
SDL_LOG_PRIORITY_ERROR = 5
SDL_LOG_PRIORITY_CRITICAL = 6
SDL_NUM_LOG_PRIORITIES = 7


SDL_LogSetAllPriority = _bind("SDL_LogSetAllPriority", [SDL_LogPriority])
SDL_LogSetPriority = _bind("SDL_LogSetPriority", [c_int, SDL_LogPriority])
SDL_LogGetPriority = _bind("SDL_LogGetPriority", [c_int], SDL_LogPriority)
SDL_LogResetPriorities = _bind("SDL_LogResetPriorities")
SDL_Log = _bind("SDL_Log", [c_char_p])
SDL_LogVerbose = _bind("SDL_LogVerbose", [c_int, c_char_p])
SDL_LogDebug = _bind("SDL_LogDebug", [c_int, c_char_p])
SDL_LogInfo = _bind("SDL_LogInfo", [c_int, c_char_p])
SDL_LogWarn = _bind("SDL_LogWarn", [c_int, c_char_p])
SDL_LogError = _bind("SDL_LogError", [c_int, c_char_p])
SDL_LogCritical = _bind("SDL_LogCritical", [c_int, c_char_p])
SDL_LogMessage = _bind("SDL_LogMessage", [c_int, SDL_LogPriority, c_char_p])
# TODO: do we want SDL_LogMessageV?
SDL_LogOutputFunction = CFUNCTYPE(None, c_void_p, c_int, SDL_LogPriority, c_char_p)
SDL_LogGetOutputFunction = _bind("SDL_LogGetOutputFunction", [POINTER(SDL_LogOutputFunction), c_void_p])
SDL_LogSetOutputFunction = _bind("SDL_LogSetOutputFunction", [SDL_LogOutputFunction, c_void_p])
