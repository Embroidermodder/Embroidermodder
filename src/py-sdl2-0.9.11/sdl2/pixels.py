from ctypes import Structure, POINTER, c_int, c_char_p, c_float
from .dll import _bind
from .dll import version as sdl_version
from .endian import SDL_BYTEORDER, SDL_BIG_ENDIAN, SDL_LIL_ENDIAN
from .stdinc import Uint8, Uint16, Uint32, SDL_bool


SDL_ALPHA_OPAQUE = 255
SDL_ALPHA_TRANSPARENT = 0


SDL_PixelType = c_int

SDL_PIXELTYPE_UNKNOWN = 0
SDL_PIXELTYPE_INDEX1 = 1
SDL_PIXELTYPE_INDEX4 = 2
SDL_PIXELTYPE_INDEX8 = 3
SDL_PIXELTYPE_PACKED8 = 4
SDL_PIXELTYPE_PACKED16 = 5
SDL_PIXELTYPE_PACKED32 = 6
SDL_PIXELTYPE_ARRAYU8 = 7
SDL_PIXELTYPE_ARRAYU16 = 8
SDL_PIXELTYPE_ARRAYU32 = 9
SDL_PIXELTYPE_ARRAYF16 = 10
SDL_PIXELTYPE_ARRAYF32 = 11


SDL_BitmapOrder = c_int

SDL_BITMAPORDER_NONE = 0
SDL_BITMAPORDER_4321 = 1
SDL_BITMAPORDER_1234 = 2


SDL_PackedOrder = c_int

SDL_PACKEDORDER_NONE = 0
SDL_PACKEDORDER_XRGB = 1
SDL_PACKEDORDER_RGBX = 2
SDL_PACKEDORDER_ARGB = 3
SDL_PACKEDORDER_RGBA = 4
SDL_PACKEDORDER_XBGR = 5
SDL_PACKEDORDER_BGRX = 6
SDL_PACKEDORDER_ABGR = 7
SDL_PACKEDORDER_BGRA = 8


SDL_ArrayOrder = c_int

SDL_ARRAYORDER_NONE = 0
SDL_ARRAYORDER_RGB = 1
SDL_ARRAYORDER_RGBA = 2
SDL_ARRAYORDER_ARGB = 3
SDL_ARRAYORDER_BGR = 4
SDL_ARRAYORDER_BGRA = 5
SDL_ARRAYORDER_ABGR = 6


SDL_PackedLayout = c_int

SDL_PACKEDLAYOUT_NONE = 0
SDL_PACKEDLAYOUT_332 = 1
SDL_PACKEDLAYOUT_4444 = 2
SDL_PACKEDLAYOUT_1555 = 3
SDL_PACKEDLAYOUT_5551 = 4
SDL_PACKEDLAYOUT_565 = 5
SDL_PACKEDLAYOUT_8888 = 6
SDL_PACKEDLAYOUT_2101010 = 7
SDL_PACKEDLAYOUT_1010102 = 8


SDL_FOURCC = lambda a, b, c, d: (ord(a) << 0) | (ord(b) << 8) | (ord(c) << 16) | (ord(d) << 24)
SDL_DEFINE_PIXELFOURCC = SDL_FOURCC
SDL_DEFINE_PIXELFORMAT = lambda ptype, order, layout, bits, pbytes: ((1 << 28) | ((ptype) << 24) | ((order) << 20) | ((layout) << 16) | ((bits) << 8) | ((pbytes) << 0))
SDL_PIXELFLAG = lambda X: (((X) >> 28) & 0x0F)
SDL_PIXELTYPE = lambda X: (((X) >> 24) & 0x0F)
SDL_PIXELORDER = lambda X: (((X) >> 20) & 0x0F)
SDL_PIXELLAYOUT = lambda X: (((X) >> 16) & 0x0F)
SDL_BITSPERPIXEL = lambda X: (((X) >> 8) & 0xFF)
def SDL_BYTESPERPIXEL(x):
    valid = (SDL_PIXELFORMAT_YUY2, SDL_PIXELFORMAT_UYVY, SDL_PIXELFORMAT_YVYU)
    if SDL_ISPIXELFORMAT_FOURCC(x):
        if x in valid:
            return 2
        else:
            return 1
    else:
        return(((x) >> 0) & 0xFF)
def SDL_ISPIXELFORMAT_INDEXED(pformat):
    """Checks, if the passed format value is an indexed format."""
    return ((not SDL_ISPIXELFORMAT_FOURCC(pformat)) and
            ((SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_INDEX1) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_INDEX4) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_INDEX8)))

def SDL_ISPIXELFORMAT_PACKED(pformat):
    """Checks, if the passed format value is a packed format."""
    return (not SDL_ISPIXELFORMAT_FOURCC(pformat) and
            ((SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_PACKED8) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_PACKED16) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_PACKED32)))

def SDL_ISPIXELFORMAT_ARRAY(pformat):
    """Checks, if the passed format value is an array format."""
    return (not SDL_ISPIXELFORMAT_FOURCC(pformat) and
            ((SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_ARRAYU8) and
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_ARRAYU16) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_ARRAYU32) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_ARRAYF16) or
             (SDL_PIXELTYPE(pformat) == SDL_PIXELTYPE_ARRAYF32)))

def SDL_ISPIXELFORMAT_ALPHA(pformat):
    """Checks, if the passed format value is an alpha channel supporting
    format.
    """
    return ((SDL_ISPIXELFORMAT_PACKED(pformat) and
             ((SDL_PIXELORDER(pformat) == SDL_PACKEDORDER_ARGB) or
              (SDL_PIXELORDER(pformat) == SDL_PACKEDORDER_RGBA) or
              (SDL_PIXELORDER(pformat) == SDL_PACKEDORDER_ABGR) or
              (SDL_PIXELORDER(pformat) == SDL_PACKEDORDER_BGRA))) or
            (SDL_ISPIXELFORMAT_ARRAY(pformat) and
             ((SDL_PIXELORDER(pformat) == SDL_ARRAYORDER_ARGB) or
              (SDL_PIXELORDER(pformat) == SDL_ARRAYORDER_RGBA) or
              (SDL_PIXELORDER(pformat) == SDL_ARRAYORDER_ABGR) or
              (SDL_PIXELORDER(pformat) == SDL_ARRAYORDER_BGRA))))

SDL_ISPIXELFORMAT_FOURCC = lambda fmt: ((fmt) and (SDL_PIXELFLAG(fmt) != 1))

SDL_PixelFormatEnum = c_int
SDL_PIXELFORMAT_UNKNOWN = 0
SDL_PIXELFORMAT_INDEX1LSB = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1,
                                                   SDL_BITMAPORDER_4321,
                                                   0, 1, 0)
SDL_PIXELFORMAT_INDEX1MSB = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1,
                                                   SDL_BITMAPORDER_1234,
                                                   0, 1, 0)
SDL_PIXELFORMAT_INDEX4LSB = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX4,
                                                   SDL_BITMAPORDER_4321,
                                                   0, 4, 0)
SDL_PIXELFORMAT_INDEX4MSB = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX4,
                                                   SDL_BITMAPORDER_1234,
                                                   0, 4, 0)
SDL_PIXELFORMAT_INDEX8 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX8, 0,
                                                0, 8, 1)
SDL_PIXELFORMAT_RGB332 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED8,
                                                SDL_PACKEDORDER_XRGB,
                                                SDL_PACKEDLAYOUT_332, 8, 1)
SDL_PIXELFORMAT_XRGB4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_XRGB,
                                                  SDL_PACKEDLAYOUT_4444, 12, 2)
SDL_PIXELFORMAT_RGB444 = SDL_PIXELFORMAT_XRGB4444
SDL_PIXELFORMAT_XBGR4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_XBGR,
                                                  SDL_PACKEDLAYOUT_4444, 12, 2)
SDL_PIXELFORMAT_BGR444 = SDL_PIXELFORMAT_XBGR4444
SDL_PIXELFORMAT_XRGB1555 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_XRGB,
                                                  SDL_PACKEDLAYOUT_1555, 15, 2)
SDL_PIXELFORMAT_RGB555 = SDL_PIXELFORMAT_XRGB1555
SDL_PIXELFORMAT_XBGR1555 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_XBGR,
                                                  SDL_PACKEDLAYOUT_1555, 15, 2)
SDL_PIXELFORMAT_BGR555 = SDL_PIXELFORMAT_XBGR1555
SDL_PIXELFORMAT_ARGB4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_ARGB,
                                                  SDL_PACKEDLAYOUT_4444, 16, 2)
SDL_PIXELFORMAT_RGBA4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_RGBA,
                                                  SDL_PACKEDLAYOUT_4444, 16, 2)
SDL_PIXELFORMAT_ABGR4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_ABGR,
                                                  SDL_PACKEDLAYOUT_4444, 16, 2)
SDL_PIXELFORMAT_BGRA4444 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_BGRA,
                                                  SDL_PACKEDLAYOUT_4444, 16, 2)
SDL_PIXELFORMAT_ARGB1555 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_ARGB,
                                                  SDL_PACKEDLAYOUT_1555, 16, 2)
SDL_PIXELFORMAT_RGBA5551 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_RGBA,
                                                  SDL_PACKEDLAYOUT_5551, 16, 2)
SDL_PIXELFORMAT_ABGR1555 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_ABGR,
                                                  SDL_PACKEDLAYOUT_1555, 16, 2)
SDL_PIXELFORMAT_BGRA5551 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                  SDL_PACKEDORDER_BGRA,
                                                  SDL_PACKEDLAYOUT_5551, 16, 2)
SDL_PIXELFORMAT_RGB565 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                SDL_PACKEDORDER_XRGB,
                                                SDL_PACKEDLAYOUT_565, 16, 2)
SDL_PIXELFORMAT_BGR565 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16,
                                                SDL_PACKEDORDER_XBGR,
                                                SDL_PACKEDLAYOUT_565, 16, 2)
SDL_PIXELFORMAT_RGB24 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU8,
                                               SDL_ARRAYORDER_RGB, 0, 24, 3)
SDL_PIXELFORMAT_BGR24 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU8,
                                               SDL_ARRAYORDER_BGR, 0, 24, 3)
SDL_PIXELFORMAT_XRGB8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_XRGB,
                                                  SDL_PACKEDLAYOUT_8888, 24, 4)
SDL_PIXELFORMAT_RGB888 = SDL_PIXELFORMAT_XRGB8888
SDL_PIXELFORMAT_RGBX8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_RGBX,
                                                  SDL_PACKEDLAYOUT_8888, 24, 4)
SDL_PIXELFORMAT_XBGR8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_XBGR,
                                                  SDL_PACKEDLAYOUT_8888, 24, 4)
SDL_PIXELFORMAT_BGR888 = SDL_PIXELFORMAT_XBGR8888
SDL_PIXELFORMAT_BGRX8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_BGRX,
                                                  SDL_PACKEDLAYOUT_8888, 24, 4)
SDL_PIXELFORMAT_ARGB8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_ARGB,
                                                  SDL_PACKEDLAYOUT_8888, 32, 4)
SDL_PIXELFORMAT_RGBA8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_RGBA,
                                                  SDL_PACKEDLAYOUT_8888, 32, 4)
SDL_PIXELFORMAT_ABGR8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_ABGR,
                                                  SDL_PACKEDLAYOUT_8888, 32, 4)
SDL_PIXELFORMAT_BGRA8888 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                  SDL_PACKEDORDER_BGRA,
                                                  SDL_PACKEDLAYOUT_8888, 32, 4)
SDL_PIXELFORMAT_ARGB2101010 = SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32,
                                                     SDL_PACKEDORDER_ARGB,
                                                     SDL_PACKEDLAYOUT_2101010,
                                                     32, 4)
if SDL_BYTEORDER == SDL_BIG_ENDIAN:
    SDL_PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_RGBA8888
    SDL_PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_ARGB8888
    SDL_PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_BGRA8888
    SDL_PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_ABGR8888
else:
    SDL_PIXELFORMAT_RGBA32 = SDL_PIXELFORMAT_ABGR8888
    SDL_PIXELFORMAT_ARGB32 = SDL_PIXELFORMAT_BGRA8888
    SDL_PIXELFORMAT_BGRA32 = SDL_PIXELFORMAT_ARGB8888
    SDL_PIXELFORMAT_ABGR32 = SDL_PIXELFORMAT_RGBA8888
SDL_PIXELFORMAT_YV12 = SDL_DEFINE_PIXELFOURCC('Y', 'V', '1', '2')
SDL_PIXELFORMAT_IYUV = SDL_DEFINE_PIXELFOURCC('I', 'Y', 'U', 'V')
SDL_PIXELFORMAT_YUY2 = SDL_DEFINE_PIXELFOURCC('Y', 'U', 'Y', '2')
SDL_PIXELFORMAT_UYVY = SDL_DEFINE_PIXELFOURCC('U', 'Y', 'V', 'Y')
SDL_PIXELFORMAT_YVYU = SDL_DEFINE_PIXELFOURCC('Y', 'V', 'Y', 'U')
SDL_PIXELFORMAT_NV12 = SDL_DEFINE_PIXELFOURCC('N', 'V', '1', '2')
SDL_PIXELFORMAT_NV21 = SDL_DEFINE_PIXELFOURCC('N', 'V', '2', '1')
SDL_PIXELFORMAT_EXTERNAL_OES = SDL_DEFINE_PIXELFOURCC('O', 'E', 'S', ' ')

NAME_MAP = {
    'INDEX1LSB': SDL_PIXELFORMAT_INDEX1LSB,
    'INDEX1MSB': SDL_PIXELFORMAT_INDEX1MSB,
    'INDEX4LSB': SDL_PIXELFORMAT_INDEX4LSB,
    'INDEX4MSB': SDL_PIXELFORMAT_INDEX4MSB,
    'INDEX8': SDL_PIXELFORMAT_INDEX8,
    'RGB332': SDL_PIXELFORMAT_RGB332,
    'RGB444': SDL_PIXELFORMAT_RGB444,
    'RGB555': SDL_PIXELFORMAT_RGB555,
    'BGR555': SDL_PIXELFORMAT_BGR555,
    'ARGB4444': SDL_PIXELFORMAT_ARGB4444,
    'RGBA4444': SDL_PIXELFORMAT_RGBA4444,
    'ABGR4444': SDL_PIXELFORMAT_ABGR4444,
    'BGRA4444': SDL_PIXELFORMAT_BGRA4444,
    'ARGB1555': SDL_PIXELFORMAT_ARGB1555,
    'RGBA5551': SDL_PIXELFORMAT_RGBA5551,
    'ABGR1555': SDL_PIXELFORMAT_ABGR1555,
    'BGRA5551': SDL_PIXELFORMAT_BGRA5551,
    'RGB565': SDL_PIXELFORMAT_RGB565,
    'BGR565': SDL_PIXELFORMAT_BGR565,
    'RGB24': SDL_PIXELFORMAT_RGB24,
    'BGR24': SDL_PIXELFORMAT_BGR24,
    'RGB888': SDL_PIXELFORMAT_RGB888,
    'RGBX8888': SDL_PIXELFORMAT_RGBX8888,
    'BGR888': SDL_PIXELFORMAT_BGR888,
    'BGRX8888': SDL_PIXELFORMAT_BGRX8888,
    'ARGB8888': SDL_PIXELFORMAT_ARGB8888,
    'RGBA8888': SDL_PIXELFORMAT_RGBA8888,
    'ABGR8888': SDL_PIXELFORMAT_ABGR8888,
    'BGRA8888': SDL_PIXELFORMAT_BGRA8888,
    'ARGB2101010': SDL_PIXELFORMAT_ARGB2101010,
    'RGBA32': SDL_PIXELFORMAT_RGBA32,
    'ARGB32': SDL_PIXELFORMAT_ARGB32,
    'BGRA32': SDL_PIXELFORMAT_BGRA32,
    'ABGR32': SDL_PIXELFORMAT_ABGR32,
    'YV12': SDL_PIXELFORMAT_YV12,
    'IYUV': SDL_PIXELFORMAT_IYUV,
    'YUY2': SDL_PIXELFORMAT_YUY2,
    'UYVY': SDL_PIXELFORMAT_UYVY,
    'YVYU': SDL_PIXELFORMAT_YVYU,
    'NV12': SDL_PIXELFORMAT_NV12,
    'NV21': SDL_PIXELFORMAT_NV21,
    'EXTERNAL_OES': SDL_PIXELFORMAT_EXTERNAL_OES,
}
if sdl_version >= 2012:
    NAME_MAP['BGR444'] = SDL_PIXELFORMAT_BGR444
ALL_PIXELFORMATS = tuple(NAME_MAP.values())

class SDL_Color(Structure):
    _fields_ = [("r", Uint8),
                ("g", Uint8),
                ("b", Uint8),
                ("a", Uint8),
                ]

    def __init__(self, r=255, g=255, b=255, a=255):
        super(SDL_Color, self).__init__()
        self.r = r
        self.g = g
        self.b = b
        self.a = a

    def __repr__(self):
        return "SDL_Color(r=%d, g=%d, b=%d, a=%d)" % (self.r, self.g, self.b,
                                                      self.a)

    def __copy__(self):
        return SDL_Color(self.r, self.g, self.b, self.a)

    def __deepcopy__(self, memo):
        return SDL_Color(self.r, self.g, self.b, self.a)

    def __eq__(self, color):
        return self.r == color.r and self.g == color.g and \
            self.b == color.b and self.a == color.a

    def __ne__(self, color):
        return self.r != color.r or self.g != color.g or self.b != color.b or \
            self.a != color.a

SDL_Colour = SDL_Color


class SDL_Palette(Structure):
    _fields_ = [("ncolors", c_int),
                ("colors", POINTER(SDL_Color)),
                ("version", Uint32),
                ("refcount", c_int)]


class SDL_PixelFormat(Structure):
    pass
SDL_PixelFormat._fields_ = \
    [("format", Uint32),
     ("palette", POINTER(SDL_Palette)),
     ("BitsPerPixel", Uint8),
     ("BytesPerPixel", Uint8),
     ("padding", Uint8 * 2),
     ("Rmask", Uint32),
     ("Gmask", Uint32),
     ("Bmask", Uint32),
     ("Amask", Uint32),
     ("Rloss", Uint8),
     ("Gloss", Uint8),
     ("Bloss", Uint8),
     ("Aloss", Uint8),
     ("Rshift", Uint8),
     ("Gshift", Uint8),
     ("Bshift", Uint8),
     ("Ashift", Uint8),
     ("refcount", c_int),
     ("next", POINTER(SDL_PixelFormat))]


SDL_GetPixelFormatName = _bind("SDL_GetPixelFormatName", [Uint32], c_char_p)
SDL_PixelFormatEnumToMasks = _bind("SDL_PixelFormatEnumToMasks", [Uint32, POINTER(c_int), POINTER(Uint32), POINTER(Uint32), POINTER(Uint32), POINTER(Uint32)], SDL_bool)
SDL_MasksToPixelFormatEnum = _bind("SDL_MasksToPixelFormatEnum", [c_int, Uint32, Uint32, Uint32, Uint32], Uint32)
SDL_AllocFormat = _bind("SDL_AllocFormat", [Uint32], POINTER(SDL_PixelFormat))
SDL_FreeFormat = _bind("SDL_FreeFormat", [POINTER(SDL_PixelFormat)])
SDL_AllocPalette = _bind("SDL_AllocPalette", [c_int], POINTER(SDL_Palette))
SDL_SetPixelFormatPalette = _bind("SDL_SetPixelFormatPalette", [POINTER(SDL_PixelFormat), POINTER(SDL_Palette)], c_int)
SDL_SetPaletteColors = _bind("SDL_SetPaletteColors", [POINTER(SDL_Palette), POINTER(SDL_Color), c_int, c_int], c_int)
SDL_FreePalette = _bind("SDL_FreePalette", [POINTER(SDL_Palette)])
SDL_MapRGB = _bind("SDL_MapRGB", [POINTER(SDL_PixelFormat), Uint8, Uint8, Uint8], Uint32)
SDL_MapRGBA = _bind("SDL_MapRGBA", [POINTER(SDL_PixelFormat), Uint8, Uint8, Uint8, Uint8], Uint32)
SDL_GetRGB = _bind("SDL_GetRGB", [Uint32, POINTER(SDL_PixelFormat), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8)])
SDL_GetRGBA = _bind("SDL_GetRGBA", [Uint32, POINTER(SDL_PixelFormat), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8)])
SDL_CalculateGammaRamp = _bind("SDL_CalculateGammaRamp", [c_float, POINTER(Uint16)])
