"""SDL2 wrapper package"""
from .dll import get_dll_file, _bind
from ctypes import c_int as _cint

from .audio import *
from .blendmode import *
from .clipboard import *
from .cpuinfo import *
from .endian import *
from .error import *
from .events import *
from .filesystem import *
from .gamecontroller import *
from .gesture import *
from .haptic import *
from .hidapi import *
from .hints import *
from .joystick import *
from .keyboard import *
from .loadso import *
from .log import *
from .messagebox import *
from .metal import *
from .mouse import *
from .pixels import *
from .platform import *
from .power import *
from .rect import *
from .render import *
from .rwops import *
from .sensor import *
from .shape import *
from .stdinc import *
from .surface import *
from .syswm import *
from .timer import *
from .touch import *
from .version import *
from .video import *
from .locale import *
from .misc import *

from .keycode import *
from .scancode import *

# At least Win32 platforms need this now.
_SDL_SetMainReady = _bind("SDL_SetMainReady")
_SDL_SetMainReady()


SDL_INIT_TIMER = 0x00000001
SDL_INIT_AUDIO = 0x00000010
SDL_INIT_VIDEO = 0x00000020
SDL_INIT_JOYSTICK = 0x00000200
SDL_INIT_HAPTIC = 0x00001000
SDL_INIT_GAMECONTROLLER = 0x00002000
SDL_INIT_EVENTS = 0x00004000
SDL_INIT_SENSOR = 0x00008000
SDL_INIT_NOPARACHUTE = 0x00100000
SDL_INIT_EVERYTHING = (
    SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
    SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER |
    SDL_INIT_SENSOR
)

SDL_Init = _bind("SDL_Init", [Uint32], _cint)
SDL_InitSubSystem = _bind("SDL_InitSubSystem", [Uint32], _cint)
SDL_QuitSubSystem = _bind("SDL_QuitSubSystem", [Uint32])
SDL_WasInit = _bind("SDL_WasInit", [Uint32], Uint32)
SDL_Quit = _bind("SDL_Quit")

__version__ = "0.9.11"
version_info = (0, 9, 11)
