from ctypes import Structure, POINTER, c_int, c_char_p, c_void_p, c_float, \
    c_double
from .dll import _bind
from .stdinc import Uint8, Uint32, SDL_bool
from .pixels import SDL_Color
from .blendmode import SDL_BlendMode
from .rect import SDL_Point, SDL_FPoint, SDL_Rect, SDL_FRect
from .surface import SDL_Surface
from .video import SDL_Window

__all__ = [
    # Structs & Opaque Types
    "SDL_RendererInfo", "SDL_Renderer", "SDL_Texture", "SDL_Vertex",

    # Enums
    "SDL_RendererFlags",
    "SDL_RENDERER_SOFTWARE", "SDL_RENDERER_ACCELERATED",
    "SDL_RENDERER_PRESENTVSYNC", "SDL_RENDERER_TARGETTEXTURE",

    "SDL_ScaleMode",
    "SDL_ScaleModeNearest", "SDL_ScaleModeLinear", "SDL_ScaleModeBest",

    "SDL_TextureAccess",
    "SDL_TEXTUREACCESS_STATIC", "SDL_TEXTUREACCESS_STREAMING",
    "SDL_TEXTUREACCESS_TARGET",

    "SDL_TextureModulate",
    "SDL_TEXTUREMODULATE_NONE", "SDL_TEXTUREMODULATE_COLOR",
    "SDL_TEXTUREMODULATE_ALPHA",

    "SDL_RendererFlip",
    "SDL_FLIP_NONE", "SDL_FLIP_HORIZONTAL", "SDL_FLIP_VERTICAL",

    # Functions
    "SDL_GetNumRenderDrivers", "SDL_GetRenderDriverInfo",
    "SDL_CreateWindowAndRenderer", "SDL_CreateRenderer",
    "SDL_CreateSoftwareRenderer", "SDL_GetRenderer",
    "SDL_GetRendererInfo", "SDL_CreateTexture",
    "SDL_CreateTextureFromSurface", "SDL_QueryTexture",
    "SDL_SetTextureColorMod", "SDL_GetTextureColorMod",
    "SDL_SetTextureAlphaMod", "SDL_GetTextureAlphaMod",
    "SDL_SetTextureBlendMode", "SDL_GetTextureBlendMode",
    "SDL_SetTextureScaleMode", "SDL_GetTextureScaleMode",
    "SDL_SetTextureUserData", "SDL_GetTextureUserData",
    "SDL_UpdateTexture", "SDL_LockTexture", "SDL_LockTextureToSurface",
    "SDL_UnlockTexture",
    "SDL_RenderTargetSupported", "SDL_SetRenderTarget",
    "SDL_GetRenderTarget", "SDL_RenderSetLogicalSize",
    "SDL_RenderGetLogicalSize", "SDL_RenderSetViewport",
    "SDL_RenderGetClipRect", "SDL_RenderSetClipRect",
    "SDL_RenderGetViewport", "SDL_RenderSetScale", "SDL_RenderGetScale",
    "SDL_RenderWindowToLogical", "SDL_RenderLogicalToWindow",
    "SDL_SetRenderDrawColor", "SDL_GetRenderDrawColor",
    "SDL_SetRenderDrawBlendMode", "SDL_GetRenderDrawBlendMode",
    "SDL_RenderClear", "SDL_RenderDrawPoint", "SDL_RenderDrawPoints",
    "SDL_RenderDrawLine", "SDL_RenderDrawLines", "SDL_RenderDrawRect",
    "SDL_RenderDrawRects", "SDL_RenderFillRect", "SDL_RenderFillRects",
    "SDL_RenderCopy", "SDL_RenderCopyEx", "SDL_RenderDrawPointF",
    "SDL_RenderDrawPointsF", "SDL_RenderDrawLineF",
    "SDL_RenderDrawLinesF", "SDL_RenderDrawRectF",
    "SDL_RenderDrawRectsF", "SDL_RenderFillRectF",
    "SDL_RenderFillRectsF", "SDL_RenderCopyF", "SDL_RenderCopyExF",
    "SDL_RenderGeometry", "SDL_RenderGeometryRaw",
    "SDL_RenderReadPixels", "SDL_RenderPresent",
    "SDL_DestroyTexture", "SDL_DestroyRenderer", "SDL_RenderFlush",
    "SDL_UpdateYUVTexture", "SDL_UpdateNVTexture",
    "SDL_GL_BindTexture", "SDL_GL_UnbindTexture",
    "SDL_GetRendererOutputSize", "SDL_RenderGetIntegerScale",
    "SDL_RenderSetIntegerScale", "SDL_RenderGetMetalLayer",
    "SDL_RenderGetMetalCommandEncoder"
]


SDL_RendererFlags = c_int
SDL_RENDERER_SOFTWARE = 0x00000001
SDL_RENDERER_ACCELERATED = 0x00000002
SDL_RENDERER_PRESENTVSYNC = 0x00000004
SDL_RENDERER_TARGETTEXTURE = 0x00000008


class SDL_RendererInfo(Structure):
    _fields_ = [("name", c_char_p),
                ("flags", Uint32),
                ("num_texture_formats", Uint32),
                ("texture_formats", Uint32 * 16),
                ("max_texture_width", c_int),
                ("max_texture_height", c_int)]


class SDL_Vertex(Structure):
    _fields_ = [("position", SDL_FPoint),
                ("color", SDL_Color),
                ("tex_coord", SDL_FPoint)]

    def __init__(
        self, position=SDL_FPoint(), color=SDL_Color(), tex_coord=SDL_FPoint()
    ):
        super(SDL_Vertex, self).__init__()
        self.position = self._get_point(position, "position")
        self.color = self._get_color(color)
        self.tex_coord = self._get_point(tex_coord, "tex_coord")

    def _get_point(self, p, argname):
        if type(p) in (tuple, list) and len(p) == 2:
            p = SDL_FPoint(p[0], p[1])
        elif type(p) == SDL_FPoint:
            p = SDL_FPoint(p.x, p.y)
        else:
            err = "'{0}' must be an (x, y) tuple or an SDL_FPoint."
            raise ValueError(err.format(argname))
        return p

    def _get_color(self, col):
        if type(col).__name__ in ("Color", "SDL_Color"):
            col = SDL_Color(col.r, col.g, col.b, col.a)
        elif type(col) in (tuple, list) and len(col) in (3, 4):
            if len(col) == 3:
                col = SDL_Color(col[0], col[1], col[2], 255)
            else:
                col = SDL_Color(col[0], col[1], col[2], col[3])
        else:
            err = "'color' must be an RGBA tuple or an SDL_Color."
            raise ValueError(err)
        return col

    def __repr__(self):
        x = round(self.position.x, 4)
        y = round(self.position.y, 4)
        c = self.color
        col = str([c.r, c.g, c.b, c.a])
        return "SDL_Vertex(x={0}, y={1}, color={2})".format(x, y, col)

    def __copy__(self):
        return SDL_Vertex(self.position, self.color, self.tex_coord)

    def __deepcopy__(self, memo):
        return SDL_Vertex(self.position, self.color, self.tex_coord)


SDL_ScaleMode = c_int
SDL_ScaleModeNearest = 0
SDL_ScaleModeLinear = 1
SDL_ScaleModeBest = 2

SDL_TextureAccess = c_int
SDL_TEXTUREACCESS_STATIC = 0
SDL_TEXTUREACCESS_STREAMING = 1
SDL_TEXTUREACCESS_TARGET = 2

SDL_TextureModulate = c_int
SDL_TEXTUREMODULATE_NONE = 0x00000000
SDL_TEXTUREMODULATE_COLOR = 0x00000001
SDL_TEXTUREMODULATE_ALPHA = 0x00000002

SDL_RendererFlip = c_int
SDL_FLIP_NONE = 0x00000000
SDL_FLIP_HORIZONTAL = 0x00000001
SDL_FLIP_VERTICAL = 0x00000002


class SDL_Renderer(c_void_p):
    pass

class SDL_Texture(c_void_p):
    pass


SDL_GetNumRenderDrivers = _bind("SDL_GetNumRenderDrivers", None, c_int)
SDL_GetRenderDriverInfo = _bind("SDL_GetRenderDriverInfo", [c_int, POINTER(SDL_RendererInfo)], c_int)
SDL_CreateWindowAndRenderer = _bind("SDL_CreateWindowAndRenderer", [c_int, c_int, Uint32, POINTER(POINTER(SDL_Window)), POINTER(POINTER(SDL_Renderer))], c_int)
SDL_CreateRenderer = _bind("SDL_CreateRenderer", [POINTER(SDL_Window), c_int, Uint32], POINTER(SDL_Renderer))
SDL_CreateSoftwareRenderer = _bind("SDL_CreateSoftwareRenderer", [POINTER(SDL_Surface)], POINTER(SDL_Renderer))
SDL_GetRenderer = _bind("SDL_GetRenderer", [POINTER(SDL_Window)], POINTER(SDL_Renderer))
SDL_GetRendererInfo = _bind("SDL_GetRendererInfo", [POINTER(SDL_Renderer), POINTER(SDL_RendererInfo)], c_int)
SDL_GetRendererOutputSize = _bind("SDL_GetRendererOutputSize", [POINTER(SDL_Renderer), POINTER(c_int), POINTER(c_int)], c_int)
SDL_CreateTexture = _bind("SDL_CreateTexture", [POINTER(SDL_Renderer), Uint32, c_int, c_int, c_int], POINTER(SDL_Texture))
SDL_CreateTextureFromSurface = _bind("SDL_CreateTextureFromSurface", [POINTER(SDL_Renderer), POINTER(SDL_Surface)], POINTER(SDL_Texture))
SDL_QueryTexture = _bind("SDL_QueryTexture", [POINTER(SDL_Texture), POINTER(Uint32), POINTER(c_int), POINTER(c_int), POINTER(c_int)], c_int)
SDL_SetTextureColorMod = _bind("SDL_SetTextureColorMod", [POINTER(SDL_Texture), Uint8, Uint8, Uint8], c_int)
SDL_GetTextureColorMod = _bind("SDL_GetTextureColorMod", [POINTER(SDL_Texture), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8)], c_int)
SDL_SetTextureAlphaMod = _bind("SDL_SetTextureAlphaMod", [POINTER(SDL_Texture), Uint8], c_int)
SDL_GetTextureAlphaMod = _bind("SDL_GetTextureAlphaMod", [POINTER(SDL_Texture), POINTER(Uint8)], c_int)
SDL_SetTextureBlendMode = _bind("SDL_SetTextureBlendMode", [POINTER(SDL_Texture), SDL_BlendMode], c_int)
SDL_GetTextureBlendMode = _bind("SDL_GetTextureBlendMode", [POINTER(SDL_Texture), POINTER(SDL_BlendMode)], c_int)
SDL_SetTextureScaleMode = _bind("SDL_SetTextureScaleMode", [POINTER(SDL_Texture), SDL_ScaleMode], c_int, added='2.0.12')
SDL_GetTextureScaleMode = _bind("SDL_GetTextureScaleMode", [POINTER(SDL_Texture), POINTER(SDL_ScaleMode)], c_int, added='2.0.12')
SDL_SetTextureUserData = _bind("SDL_SetTextureUserData", [POINTER(SDL_Texture), c_void_p], c_int, added='2.0.18')
SDL_GetTextureUserData = _bind("SDL_GetTextureUserData", [POINTER(SDL_Texture)], c_void_p, added='2.0.18')
SDL_UpdateTexture = _bind("SDL_UpdateTexture", [POINTER(SDL_Texture), POINTER(SDL_Rect), c_void_p, c_int], c_int)
SDL_UpdateYUVTexture = _bind("SDL_UpdateYUVTexture", [POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(Uint8), c_int, POINTER(Uint8), c_int, POINTER(Uint8), c_int], c_int)
SDL_UpdateNVTexture = _bind("SDL_UpdateNVTexture", [POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(Uint8), c_int, POINTER(Uint8), c_int], c_int, added='2.0.16')
SDL_LockTexture = _bind("SDL_LockTexture", [POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(c_void_p), POINTER(c_int)], c_int)
SDL_LockTextureToSurface = _bind("SDL_LockTextureToSurface", [POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(POINTER(SDL_Surface))], c_int, added='2.0.12')
SDL_UnlockTexture = _bind("SDL_UnlockTexture", [POINTER(SDL_Texture)])
SDL_RenderTargetSupported = _bind("SDL_RenderTargetSupported", [POINTER(SDL_Renderer)], SDL_bool)
SDL_SetRenderTarget = _bind("SDL_SetRenderTarget", [POINTER(SDL_Renderer), POINTER(SDL_Texture)], c_int)
SDL_GetRenderTarget = _bind("SDL_GetRenderTarget", [POINTER(SDL_Renderer)], POINTER(SDL_Texture))
SDL_RenderSetLogicalSize = _bind("SDL_RenderSetLogicalSize", [POINTER(SDL_Renderer), c_int, c_int], c_int)
SDL_RenderGetLogicalSize = _bind("SDL_RenderGetLogicalSize", [POINTER(SDL_Renderer), POINTER(c_int), POINTER(c_int)])
SDL_RenderSetIntegerScale = _bind("SDL_RenderSetIntegerScale", [POINTER(SDL_Renderer), SDL_bool], c_int, added='2.0.5')
SDL_RenderGetIntegerScale = _bind("SDL_RenderGetIntegerScale", [POINTER(SDL_Renderer)], SDL_bool, added='2.0.5')
SDL_RenderSetViewport = _bind("SDL_RenderSetViewport", [POINTER(SDL_Renderer), POINTER(SDL_Rect)], c_int)
SDL_RenderGetViewport = _bind("SDL_RenderGetViewport", [POINTER(SDL_Renderer), POINTER(SDL_Rect)])
SDL_RenderGetClipRect = _bind("SDL_RenderGetClipRect", [POINTER(SDL_Renderer), POINTER(SDL_Rect)])
SDL_RenderSetClipRect = _bind("SDL_RenderSetClipRect", [POINTER(SDL_Renderer), POINTER(SDL_Rect)], c_int)
SDL_RenderIsClipEnabled = _bind("SDL_RenderIsClipEnabled", [POINTER(SDL_Renderer)], SDL_bool, added='2.0.4')
SDL_RenderSetScale = _bind("SDL_RenderSetScale", [POINTER(SDL_Renderer), c_float, c_float], c_int)
SDL_RenderGetScale = _bind("SDL_RenderGetScale", [POINTER(SDL_Renderer), POINTER(c_float), POINTER(c_float)])
SDL_RenderWindowToLogical = _bind("SDL_RenderWindowToLogical", [POINTER(SDL_Renderer), c_int, c_int, POINTER(c_float), POINTER(c_float)], added='2.0.18')
SDL_RenderLogicalToWindow = _bind("SDL_RenderLogicalToWindow", [POINTER(SDL_Renderer), c_float, c_float, POINTER(c_int), POINTER(c_int)], added='2.0.18')
SDL_SetRenderDrawColor = _bind("SDL_SetRenderDrawColor", [POINTER(SDL_Renderer), Uint8, Uint8, Uint8, Uint8], c_int)
SDL_GetRenderDrawColor = _bind("SDL_GetRenderDrawColor", [POINTER(SDL_Renderer), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8), POINTER(Uint8)], c_int)
SDL_SetRenderDrawBlendMode = _bind("SDL_SetRenderDrawBlendMode", [POINTER(SDL_Renderer), SDL_BlendMode], c_int)
SDL_GetRenderDrawBlendMode = _bind("SDL_GetRenderDrawBlendMode", [POINTER(SDL_Renderer), POINTER(SDL_BlendMode)], c_int)
SDL_RenderClear = _bind("SDL_RenderClear", [POINTER(SDL_Renderer)], c_int)
SDL_RenderDrawPoint = _bind("SDL_RenderDrawPoint", [POINTER(SDL_Renderer), c_int, c_int], c_int)
SDL_RenderDrawPoints = _bind("SDL_RenderDrawPoints", [POINTER(SDL_Renderer), POINTER(SDL_Point), c_int], c_int)
SDL_RenderDrawLine = _bind("SDL_RenderDrawLine", [POINTER(SDL_Renderer), c_int, c_int, c_int, c_int], c_int)
SDL_RenderDrawLines = _bind("SDL_RenderDrawLines", [POINTER(SDL_Renderer), POINTER(SDL_Point), c_int], c_int)
SDL_RenderDrawRect = _bind("SDL_RenderDrawRect", [POINTER(SDL_Renderer), POINTER(SDL_Rect)], c_int)
SDL_RenderDrawRects = _bind("SDL_RenderDrawRects", [POINTER(SDL_Renderer), POINTER(SDL_Rect), c_int], c_int)
SDL_RenderFillRect = _bind("SDL_RenderFillRect", [POINTER(SDL_Renderer), POINTER(SDL_Rect)], c_int)
SDL_RenderFillRects = _bind("SDL_RenderFillRects", [POINTER(SDL_Renderer), POINTER(SDL_Rect), c_int], c_int)
SDL_RenderCopy = _bind("SDL_RenderCopy", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(SDL_Rect)], c_int)
SDL_RenderCopyEx = _bind("SDL_RenderCopyEx", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(SDL_Rect), c_double, POINTER(SDL_Point), SDL_RendererFlip], c_int)
SDL_RenderDrawPointF = _bind("SDL_RenderDrawPointF", [POINTER(SDL_Renderer), c_float, c_float], c_int, added='2.0.10')
SDL_RenderDrawPointsF = _bind("SDL_RenderDrawPointsF", [POINTER(SDL_Renderer), POINTER(SDL_FPoint), c_int], c_int, added='2.0.10')
SDL_RenderDrawLineF = _bind("SDL_RenderDrawLineF", [POINTER(SDL_Renderer), c_float, c_float, c_float, c_float], c_int, added='2.0.10')
SDL_RenderDrawLinesF = _bind("SDL_RenderDrawLinesF", [POINTER(SDL_Renderer), POINTER(SDL_FPoint), c_int], c_int, added='2.0.10')
SDL_RenderDrawRectF = _bind("SDL_RenderDrawRectF", [POINTER(SDL_Renderer), POINTER(SDL_FRect)], c_int, added='2.0.10')
SDL_RenderDrawRectsF = _bind("SDL_RenderDrawRectsF", [POINTER(SDL_Renderer), POINTER(SDL_FRect), c_int], c_int, added='2.0.10')
SDL_RenderFillRectF = _bind("SDL_RenderFillRectF", [POINTER(SDL_Renderer), POINTER(SDL_FRect)], c_int, added='2.0.10')
SDL_RenderFillRectsF = _bind("SDL_RenderFillRectsF", [POINTER(SDL_Renderer), POINTER(SDL_FRect), c_int], c_int, added='2.0.10')
SDL_RenderCopyF = _bind("SDL_RenderCopyF", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(SDL_FRect)], c_int, added='2.0.10')
SDL_RenderCopyExF = _bind("SDL_RenderCopyExF", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(SDL_Rect), POINTER(SDL_FRect), c_double, POINTER(SDL_FPoint), SDL_RendererFlip], c_int, added='2.0.10')
SDL_RenderGeometry = _bind("SDL_RenderGeometry", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(SDL_Vertex), c_int, POINTER(c_int), c_int], c_int, added='2.0.18')
SDL_RenderGeometryRaw = _bind("SDL_RenderGeometryRaw", [POINTER(SDL_Renderer), POINTER(SDL_Texture), POINTER(c_float), c_int, POINTER(SDL_Color), c_int, POINTER(c_float), c_int, c_int, c_void_p, c_int, c_int], c_int, added='2.0.18')
SDL_RenderReadPixels = _bind("SDL_RenderReadPixels", [POINTER(SDL_Renderer), POINTER(SDL_Rect), Uint32, c_void_p, c_int], c_int)
SDL_RenderPresent = _bind("SDL_RenderPresent", [POINTER(SDL_Renderer)])
SDL_DestroyTexture = _bind("SDL_DestroyTexture", [POINTER(SDL_Texture)])
SDL_DestroyRenderer = _bind("SDL_DestroyRenderer", [POINTER(SDL_Renderer)])
SDL_RenderFlush = _bind("SDL_RenderFlush", [POINTER(SDL_Renderer)], c_int, added='2.0.10')
SDL_GL_BindTexture = _bind("SDL_GL_BindTexture", [POINTER(SDL_Texture), POINTER(c_float), POINTER(c_float)], c_int)
SDL_GL_UnbindTexture = _bind("SDL_GL_UnbindTexture", [POINTER(SDL_Texture)], c_int)
SDL_RenderGetMetalLayer = _bind("SDL_RenderGetMetalLayer", [POINTER(SDL_Renderer)], c_void_p, added='2.0.8')
SDL_RenderGetMetalCommandEncoder = _bind("SDL_RenderGetMetalCommandEncoder", [POINTER(SDL_Renderer)], c_void_p, added='2.0.8')
SDL_RenderSetVSync = _bind("SDL_RenderSetVSync", [POINTER(SDL_Renderer), c_int], c_int, added='2.0.18')
