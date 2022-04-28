from ctypes import Structure, c_int, c_char_p, c_double, c_void_p, CFUNCTYPE, \
    POINTER
from .dll import _bind
from .endian import SDL_BYTEORDER, SDL_LIL_ENDIAN
from .stdinc import Uint8, Uint16, Uint32
from .rwops import SDL_RWops, SDL_RWFromFile

__all__ = [
    # Structs & Opaque Types
    "SDL_AudioSpec", "SDL_AudioCVT",

    # Defines
    "SDL_AudioFormat", "SDL_AUDIO_MASK_BITSIZE", "SDL_AUDIO_MASK_DATATYPE",
    "SDL_AUDIO_MASK_ENDIAN", "SDL_AUDIO_MASK_SIGNED", "SDL_AUDIO_BITSIZE",
    "SDL_AUDIO_ISFLOAT",
    "AUDIO_U8", "AUDIO_S8", "AUDIO_U16LSB", "AUDIO_S16LSB", "AUDIO_U16MSB",
    "AUDIO_S16MSB", "AUDIO_U16", "AUDIO_S16", "AUDIO_S32LSB", "AUDIO_S32MSB",
    "AUDIO_S32", "AUDIO_F32LSB", "AUDIO_S32MSB","AUDIO_F32",
    "AUDIO_U16SYS", "AUDIO_S16SYS", "AUDIO_S32SYS", "AUDIO_F32SYS",
    "SDL_AUDIO_ALLOW_FREQUENCY_CHANGE", "SDL_AUDIO_ALLOW_FORMAT_CHANGE",
    "SDL_AUDIO_ALLOW_CHANNELS_CHANGE", "SDL_AUDIO_ALLOW_SAMPLES_CHANGE",
    "SDL_AUDIO_ALLOW_ANY_CHANGE",
    "SDL_AUDIOCVT_MAX_FILTERS", "SDL_MIX_MAXVOLUME",

    # Enums
    "SDL_AudioStatus",
    "SDL_AUDIO_STOPPED", "SDL_AUDIO_PLAYING", "SDL_AUDIO_PAUSED", 

    # Macro Functions
    "SDL_AUDIO_ISBIGENDIAN", "SDL_AUDIO_ISSIGNED", "SDL_AUDIO_ISINT",
    "SDL_AUDIO_ISLITTLEENDIAN", "SDL_AUDIO_ISUNSIGNED", "SDL_LoadWAV",

    # Functions
    "SDL_GetNumAudioDrivers", "SDL_GetAudioDriver", "SDL_AudioInit",
    "SDL_AudioQuit", "SDL_GetCurrentAudioDriver", "SDL_OpenAudio",
    "SDL_AudioDeviceID", "SDL_GetNumAudioDevices",
    "SDL_GetAudioDeviceName", "SDL_GetAudioDeviceSpec",
    "SDL_OpenAudioDevice", "SDL_GetAudioStatus", "SDL_GetAudioDeviceStatus",
    "SDL_PauseAudio", "SDL_PauseAudioDevice", "SDL_LoadWAV_RW", "SDL_FreeWAV",
    "SDL_BuildAudioCVT", "SDL_ConvertAudio",  "SDL_MixAudio",
    "SDL_MixAudioFormat", "SDL_LockAudio", "SDL_LockAudioDevice",
    "SDL_UnlockAudio", "SDL_UnlockAudioDevice", "SDL_CloseAudio",
    "SDL_CloseAudioDevice", "SDL_QueueAudio", "SDL_GetQueuedAudioSize",
    "SDL_ClearQueuedAudio", "SDL_DequeueAudio", "SDL_AudioStream",
    "SDL_NewAudioStream", "SDL_AudioStreamPut", "SDL_AudioStreamGet",
    "SDL_AudioStreamAvailable", "SDL_AudioStreamClear",
    "SDL_FreeAudioStream",

    # Callback Functions
    "SDL_AudioCallback", "SDL_AudioFilter",

    # Convenience Variables
    "AUDIO_FORMATS"
]


# Constants, enums, and macros

SDL_AudioFormat = Uint16

SDL_AUDIO_MASK_BITSIZE = 0xFF
SDL_AUDIO_MASK_DATATYPE = 1 << 8
SDL_AUDIO_MASK_ENDIAN = 1 << 12
SDL_AUDIO_MASK_SIGNED = 1 << 15
SDL_AUDIO_BITSIZE = lambda x: (x & SDL_AUDIO_MASK_BITSIZE)
SDL_AUDIO_ISFLOAT = lambda x: (x & SDL_AUDIO_MASK_DATATYPE)
SDL_AUDIO_ISBIGENDIAN = lambda x: (x & SDL_AUDIO_MASK_ENDIAN)
SDL_AUDIO_ISSIGNED = lambda x: (x & SDL_AUDIO_MASK_SIGNED)
SDL_AUDIO_ISINT = lambda x: (not SDL_AUDIO_ISFLOAT(x))
SDL_AUDIO_ISLITTLEENDIAN = lambda x: (not SDL_AUDIO_ISBIGENDIAN(x))
SDL_AUDIO_ISUNSIGNED = lambda x: (not SDL_AUDIO_ISSIGNED(x))

AUDIO_U8 = 0x0008
AUDIO_S8 = 0x8008
AUDIO_U16LSB = 0x0010
AUDIO_S16LSB = 0x8010
AUDIO_U16MSB = 0x1010
AUDIO_S16MSB = 0x9010
AUDIO_U16 = AUDIO_U16LSB
AUDIO_S16 = AUDIO_S16LSB
AUDIO_S32LSB = 0x8020
AUDIO_S32MSB = 0x9020
AUDIO_S32 = AUDIO_S32LSB
AUDIO_F32LSB = 0x8120
AUDIO_F32MSB = 0x9120
AUDIO_F32 = AUDIO_F32LSB

# All of the audio formats should be in this set which is provided as a
# convenience to the end user for purposes of iteration and validation.
# (is the provided audio format in the supported set?)
FORMAT_NAME_MAP = {
    AUDIO_U8: "AUDIO_U8",
    AUDIO_S8: "AUDIO_S8",
    AUDIO_U16LSB: "AUDIO_U16LSB",
    AUDIO_S16LSB: "AUDIO_S16LSB",
    AUDIO_U16MSB: "AUDIO_U16MSB",
    AUDIO_S16MSB: "AUDIO_S16MSB",
    AUDIO_S32LSB: "AUDIO_S32LSB",
    AUDIO_S32MSB: "AUDIO_S32MSB",
    AUDIO_F32LSB: "AUDIO_F32LSB",
    AUDIO_F32MSB: "AUDIO_F32MSB",
}
AUDIO_FORMATS = set([
    AUDIO_U8, AUDIO_S8,
    AUDIO_U16LSB, AUDIO_U16MSB, AUDIO_U16,
    AUDIO_S16LSB, AUDIO_S16MSB, AUDIO_S16,
    AUDIO_S32LSB, AUDIO_S32MSB, AUDIO_S32,
    AUDIO_F32LSB, AUDIO_F32MSB, AUDIO_F32
])

if SDL_BYTEORDER == SDL_LIL_ENDIAN:
    AUDIO_U16SYS = AUDIO_U16LSB
    AUDIO_S16SYS = AUDIO_S16LSB
    AUDIO_S32SYS = AUDIO_S32LSB
    AUDIO_F32SYS = AUDIO_F32LSB
else:
    AUDIO_U16SYS = AUDIO_U16MSB
    AUDIO_S16SYS = AUDIO_S16MSB
    AUDIO_S32SYS = AUDIO_S32MSB
    AUDIO_F32SYS = AUDIO_F32MSB

SDL_AUDIO_ALLOW_FREQUENCY_CHANGE = 0x00000001
SDL_AUDIO_ALLOW_FORMAT_CHANGE = 0x00000002
SDL_AUDIO_ALLOW_CHANNELS_CHANGE = 0x00000004
SDL_AUDIO_ALLOW_SAMPLES_CHANGE = 0x00000008
SDL_AUDIO_ALLOW_ANY_CHANGE = (
    SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_FORMAT_CHANGE |
    SDL_AUDIO_ALLOW_CHANNELS_CHANGE | SDL_AUDIO_ALLOW_SAMPLES_CHANGE
)

SDL_AudioDeviceID = Uint32

SDL_AudioStatus = c_int
SDL_AUDIO_STOPPED = 0
SDL_AUDIO_PLAYING = 1
SDL_AUDIO_PAUSED = 2

SDL_MIX_MAXVOLUME = 128
SDL_AUDIOCVT_MAX_FILTERS = 9


# Structure, opaque type, and callback definitions

SDL_AudioCallback = CFUNCTYPE(None, c_void_p, POINTER(Uint8), c_int)

class SDL_AudioSpec(Structure):
    _fields_ = [
        ("freq", c_int),
        ("format", SDL_AudioFormat),
        ("channels", Uint8),
        ("silence", Uint8),
        ("samples", Uint16),
        ("padding", Uint16),
        ("size", Uint32),
        ("callback", SDL_AudioCallback),
        ("userdata", c_void_p),
    ]
    def __init__(
        self, freq, aformat, channels, samples, callback=SDL_AudioCallback(),
        userdata=c_void_p(0)
    ):
        super(SDL_AudioSpec, self).__init__()
        self.freq = freq
        self.format = aformat
        self.channels = channels
        self.samples = samples
        self.callback = callback
        self.userdata = userdata

class SDL_AudioCVT(Structure):
    pass

SDL_AudioFilter = CFUNCTYPE(POINTER(SDL_AudioCVT), SDL_AudioFormat)

SDL_AudioCVT._fields_ = [
    ("needed", c_int),
    ("src_format", SDL_AudioFormat),
    ("dst_format", SDL_AudioFormat),
    ("rate_incr", c_double),
    ("buf", POINTER(Uint8)),
    ("len", c_int),
    ("len_cvt", c_int),
    ("len_mult", c_int),
    ("len_ratio", c_double),
    ("filters", (SDL_AudioFilter * (SDL_AUDIOCVT_MAX_FILTERS+1))),
    ("filter_index", c_int),
]

class SDL_AudioStream(c_void_p):
    pass


# Raw ctypes function definitions

SDL_GetNumAudioDrivers = _bind("SDL_GetNumAudioDrivers", None, c_int)
SDL_GetAudioDriver = _bind("SDL_GetAudioDriver", [c_int], c_char_p)
SDL_AudioInit = _bind("SDL_AudioInit", [c_char_p], c_int)
SDL_AudioQuit = _bind("SDL_AudioQuit")
SDL_GetCurrentAudioDriver = _bind("SDL_GetCurrentAudioDriver", None, c_char_p)
SDL_OpenAudio = _bind("SDL_OpenAudio", [POINTER(SDL_AudioSpec), POINTER(SDL_AudioSpec)], c_int)
SDL_GetNumAudioDevices = _bind("SDL_GetNumAudioDevices", [c_int], c_int)
SDL_GetAudioDeviceName = _bind("SDL_GetAudioDeviceName", [c_int, c_int], c_char_p)
SDL_GetAudioDeviceSpec = _bind("SDL_GetAudioDeviceSpec", [c_int, c_int, POINTER(SDL_AudioSpec)], c_int, added='2.0.16')
SDL_OpenAudioDevice = _bind("SDL_OpenAudioDevice", [c_char_p, c_int, POINTER(SDL_AudioSpec), POINTER(SDL_AudioSpec), c_int], SDL_AudioDeviceID)
SDL_GetAudioStatus = _bind("SDL_GetAudioStatus", None, SDL_AudioStatus)
SDL_GetAudioDeviceStatus = _bind("SDL_GetAudioDeviceStatus", [SDL_AudioDeviceID], SDL_AudioStatus)
SDL_PauseAudio = _bind("SDL_PauseAudio", [c_int])
SDL_PauseAudioDevice = _bind("SDL_PauseAudioDevice", [SDL_AudioDeviceID, c_int])
SDL_LoadWAV_RW = _bind("SDL_LoadWAV_RW", [POINTER(SDL_RWops), c_int, POINTER(SDL_AudioSpec), POINTER(POINTER(Uint8)), POINTER(Uint32)], POINTER(SDL_AudioSpec))
SDL_LoadWAV = lambda f, s, ab, al: SDL_LoadWAV_RW(SDL_RWFromFile(f, b"rb"), 1, s, ab , al)
SDL_FreeWAV = _bind("SDL_FreeWAV", [POINTER(Uint8)])
SDL_BuildAudioCVT = _bind("SDL_BuildAudioCVT", [POINTER(SDL_AudioCVT), SDL_AudioFormat, Uint8, c_int, SDL_AudioFormat, Uint8, c_int], c_int)
SDL_ConvertAudio = _bind("SDL_ConvertAudio", [POINTER(SDL_AudioCVT)], c_int)
SDL_MixAudio = _bind("SDL_MixAudio", [POINTER(Uint8), POINTER(Uint8), Uint32, c_int])
SDL_MixAudioFormat = _bind("SDL_MixAudioFormat", [POINTER(Uint8), POINTER(Uint8), SDL_AudioFormat, Uint32, c_int])
SDL_LockAudio = _bind("SDL_LockAudio")
SDL_LockAudioDevice = _bind("SDL_LockAudioDevice", [SDL_AudioDeviceID])
SDL_UnlockAudio = _bind("SDL_UnlockAudio")
SDL_UnlockAudioDevice = _bind("SDL_UnlockAudioDevice", [SDL_AudioDeviceID])
SDL_CloseAudio = _bind("SDL_CloseAudio")
SDL_CloseAudioDevice = _bind("SDL_CloseAudioDevice", [SDL_AudioDeviceID])
SDL_QueueAudio = _bind("SDL_QueueAudio", [SDL_AudioDeviceID, c_void_p, Uint32], c_int, added='2.0.4')
SDL_DequeueAudio = _bind("SDL_DequeueAudio", [SDL_AudioDeviceID, c_void_p, Uint32], Uint32, added='2.0.5')
SDL_GetQueuedAudioSize = _bind("SDL_GetQueuedAudioSize", [SDL_AudioDeviceID], Uint32, added='2.0.4')
SDL_ClearQueuedAudio = _bind("SDL_ClearQueuedAudio", [SDL_AudioDeviceID], added='2.0.4')
SDL_NewAudioStream = _bind("SDL_NewAudioStream", [SDL_AudioFormat, Uint8, c_int, SDL_AudioFormat, Uint8, c_int], POINTER(SDL_AudioStream), added='2.0.7')
SDL_AudioStreamPut = _bind("SDL_AudioStreamPut", [POINTER(SDL_AudioStream), c_void_p, c_int], c_int, added='2.0.7')
SDL_AudioStreamGet = _bind("SDL_AudioStreamGet", [POINTER(SDL_AudioStream), c_void_p, c_int], c_int, added='2.0.7')
SDL_AudioStreamAvailable = _bind("SDL_AudioStreamAvailable", [POINTER(SDL_AudioStream)], c_int, added='2.0.7')
SDL_AudioStreamClear = _bind("SDL_AudioStreamClear", [POINTER(SDL_AudioStream)], added='2.0.7')
SDL_FreeAudioStream = _bind("SDL_FreeAudioStream", [POINTER(SDL_AudioStream)], added='2.0.7')
