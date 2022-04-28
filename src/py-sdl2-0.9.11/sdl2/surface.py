from ctypes import CFUNCTYPE, Structure, POINTER, c_int, c_void_p
from .dll import _bind
from .stdinc import Uint8, Uint32, SDL_bool
from .blendmode import SDL_BlendMode
from .rect import SDL_Rect
from .pixels import SDL_PixelFormat, SDL_Palette
from .rwops import SDL_RWops, SDL_RWFromFile

__all__ = [
    # Structs & Opaque Types
    "SDL_BlitMap", "SDL_Surface",

    # Defines
    "SDL_SWSURFACE", "SDL_PREALLOC", "SDL_RLEACCEL", "SDL_DONTFREE",
    "SDL_SIMD_ALIGNED",

    # Macro Functions
    "SDL_MUSTLOCK",
    
    # Functions
    "SDL_CreateRGBSurface", "SDL_CreateRGBSurfaceFrom", "SDL_FreeSurface",
    "SDL_SetSurfacePalette", "SDL_LockSurface", "SDL_UnlockSurface",
    "SDL_LoadBMP_RW", "SDL_LoadBMP", "SDL_SaveBMP_RW", "SDL_SaveBMP",
    "SDL_SetSurfaceRLE", "SDL_HasSurfaceRLE",
    "SDL_HasColorKey", "SDL_SetColorKey", "SDL_GetColorKey",
    "SDL_SetSurfaceColorMod", "SDL_GetSurfaceColorMod",
    "SDL_SetSurfaceAlphaMod", "SDL_GetSurfaceAlphaMod",
    "SDL_SetSurfaceBlendMode", "SDL_GetSurfaceBlendMode",
    "SDL_SetClipRect", "SDL_GetClipRect", "SDL_ConvertSurface",
    "SDL_ConvertSurfaceFormat", "SDL_ConvertPixels", "SDL_PremultiplyAlpha",
    "SDL_FillRect", "SDL_FillRects",
    "SDL_UpperBlit", "SDL_BlitSurface", "SDL_LowerBlit",
    "SDL_SoftStretch", "SDL_SoftStretchLinear",
    "SDL_UpperBlitScaled", "SDL_BlitScaled",
    "SDL_LowerBlitScaled", "SDL_CreateRGBSurfaceWithFormat",
    "SDL_CreateRGBSurfaceWithFormatFrom", "SDL_DuplicateSurface",
    "SDL_SetYUVConversionMode", "SDL_GetYUVConversionMode",
    "SDL_GetYUVConversionModeForResolution",

    # Callback Functions
    "SDL_Blit"
]


SDL_SWSURFACE = 0
SDL_PREALLOC = 0x00000001
SDL_RLEACCEL = 0x00000002
SDL_DONTFREE = 0x00000004
SDL_SIMD_ALIGNED = 0x00000008

SDL_MUSTLOCK = lambda s: ((s.flags & SDL_RLEACCEL) != 0)

SDL_YUV_CONVERSION_MODE = c_int
SDL_YUV_CONVERSION_JPEG = 0
SDL_YUV_CONVERSION_BT601 = 1
SDL_YUV_CONVERSION_BT709 = 2
SDL_YUV_CONVERSION_AUTOMATIC = 3

class SDL_BlitMap(c_void_p):
    pass

class SDL_Surface(Structure):
    _fields_ = [("flags", Uint32),
                ("format", POINTER(SDL_PixelFormat)),
                ("w", c_int), ("h", c_int),
                ("pitch", c_int),
                ("pixels", c_void_p),
                ("userdata", c_void_p),
                ("locked", c_int),
                ("list_blitmap", c_void_p),
                ("clip_rect", SDL_Rect),
                ("map", POINTER(SDL_BlitMap)),
                ("refcount", c_int)
               ]

SDL_Blit = CFUNCTYPE(c_int, POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect))

SDL_CreateRGBSurface = _bind("SDL_CreateRGBSurface", [Uint32, c_int, c_int, c_int, Uint32, Uint32, Uint32, Uint32], POINTER(SDL_Surface))
SDL_CreateRGBSurfaceFrom = _bind("SDL_CreateRGBSurfaceFrom", [c_void_p, c_int, c_int, c_int, c_int, Uint32, Uint32, Uint32, Uint32], POINTER(SDL_Surface))
SDL_CreateRGBSurfaceWithFormat = _bind("SDL_CreateRGBSurfaceWithFormat", [Uint32, c_int, c_int, c_int, Uint32], POINTER(SDL_Surface))
SDL_CreateRGBSurfaceWithFormatFrom = _bind("SDL_CreateRGBSurfaceWithFormatFrom", [c_void_p, c_int, c_int, c_int, c_int, Uint32], POINTER(SDL_Surface))
SDL_FreeSurface = _bind("SDL_FreeSurface", [POINTER(SDL_Surface)])
SDL_SetSurfacePalette = _bind("SDL_SetSurfacePalette", [POINTER(SDL_Surface), POINTER(SDL_Palette)], c_int)
SDL_LockSurface = _bind("SDL_LockSurface", [POINTER(SDL_Surface)], c_int)
SDL_UnlockSurface = _bind("SDL_UnlockSurface", [POINTER(SDL_Surface)])
SDL_DuplicateSurface = _bind("SDL_DuplicateSurface", [POINTER(SDL_Surface)], POINTER(SDL_Surface), added='2.0.6')

SDL_LoadBMP_RW = _bind("SDL_LoadBMP_RW", [POINTER(SDL_RWops), c_int], POINTER(SDL_Surface))
SDL_LoadBMP = lambda fname: SDL_LoadBMP_RW(SDL_RWFromFile(fname, b"rb"), 1)
SDL_SaveBMP_RW = _bind("SDL_SaveBMP_RW", [POINTER(SDL_Surface), POINTER(SDL_RWops), c_int], c_int)
SDL_SaveBMP = lambda surface, fname: SDL_SaveBMP_RW(surface, SDL_RWFromFile(fname, b"wb"), 1)

SDL_SetSurfaceRLE = _bind("SDL_SetSurfaceRLE", [POINTER(SDL_Surface), c_int], c_int)
SDL_HasSurfaceRLE = _bind("SDL_HasSurfaceRLE", [POINTER(SDL_Surface)], SDL_bool, added='2.0.14')
SDL_HasColorKey = _bind("SDL_HasColorKey", [POINTER(SDL_Surface)], SDL_bool, added='2.0.9')
SDL_SetColorKey = _bind("SDL_SetColorKey", [POINTER(SDL_Surface), c_int, Uint32], c_int)
SDL_GetColorKey = _bind("SDL_GetColorKey", [POINTER(SDL_Surface), POINTER(Uint32)], c_int)
SDL_SetSurfaceColorMod = _bind("SDL_SetSurfaceColorMod", [POINTER(SDL_Surface), Uint8, Uint8, Uint8], c_int)
SDL_GetSurfaceColorMod = _bind("SDL_GetSurfaceColorMod", [POINTER(SDL_Surface), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8)], c_int)
SDL_SetSurfaceAlphaMod = _bind("SDL_SetSurfaceAlphaMod", [POINTER(SDL_Surface), Uint8], c_int)
SDL_GetSurfaceAlphaMod = _bind("SDL_GetSurfaceAlphaMod", [POINTER(SDL_Surface), POINTER(Uint8)], c_int)
SDL_SetSurfaceBlendMode = _bind("SDL_SetSurfaceBlendMode", [POINTER(SDL_Surface), SDL_BlendMode], c_int)
SDL_GetSurfaceBlendMode = _bind("SDL_GetSurfaceBlendMode", [POINTER(SDL_Surface), POINTER(SDL_BlendMode)], c_int)
SDL_SetClipRect = _bind("SDL_SetClipRect", [POINTER(SDL_Surface), POINTER(SDL_Rect)], SDL_bool)
SDL_GetClipRect = _bind("SDL_GetClipRect", [POINTER(SDL_Surface), POINTER(SDL_Rect)])
SDL_ConvertSurface = _bind("SDL_ConvertSurface", [POINTER(SDL_Surface), POINTER(SDL_PixelFormat), Uint32], POINTER(SDL_Surface))
SDL_ConvertSurfaceFormat = _bind("SDL_ConvertSurfaceFormat", [POINTER(SDL_Surface), Uint32, Uint32], POINTER(SDL_Surface))
SDL_ConvertPixels = _bind("SDL_ConvertPixels", [c_int, c_int, Uint32, c_void_p, c_int, Uint32, c_void_p, c_int], c_int)
SDL_PremultiplyAlpha = _bind("SDL_PremultiplyAlpha", [c_int, c_int, Uint32, c_void_p, c_int, Uint32, c_void_p, c_int], c_int, added='2.0.18')
SDL_FillRect = _bind("SDL_FillRect", [POINTER(SDL_Surface), POINTER(SDL_Rect), Uint32], c_int)
SDL_FillRects = _bind("SDL_FillRects", [POINTER(SDL_Surface), POINTER(SDL_Rect), c_int, Uint32], c_int)

SDL_UpperBlit = _bind("SDL_UpperBlit", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int)
SDL_BlitSurface = SDL_UpperBlit
SDL_LowerBlit = _bind("SDL_LowerBlit", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int)
SDL_SoftStretch = _bind("SDL_SoftStretch", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int)
SDL_SoftStretchLinear = _bind("SDL_SoftStretchLinear", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int, added='2.0.16')
SDL_UpperBlitScaled = _bind("SDL_UpperBlitScaled", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int)
SDL_BlitScaled = SDL_UpperBlitScaled
SDL_LowerBlitScaled = _bind("SDL_LowerBlitScaled", [POINTER(SDL_Surface), POINTER(SDL_Rect), POINTER(SDL_Surface), POINTER(SDL_Rect)], c_int)

SDL_SetYUVConversionMode = _bind("SDL_SetYUVConversionMode", [SDL_YUV_CONVERSION_MODE], None, added='2.0.8')
SDL_GetYUVConversionMode = _bind("SDL_GetYUVConversionMode", None, SDL_YUV_CONVERSION_MODE, added='2.0.8')
SDL_GetYUVConversionModeForResolution = _bind("SDL_GetYUVConversionModeForResolution", [c_int, c_int], SDL_YUV_CONVERSION_MODE, added='2.0.8')
