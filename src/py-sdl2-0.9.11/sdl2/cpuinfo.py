from ctypes import c_int, c_size_t, c_void_p
from .dll import _bind
from .stdinc import SDL_bool

__all__ = [
    # Defines
    "SDL_CACHELINE_SIZE",

    # Functions
    "SDL_GetCPUCount", "SDL_GetCPUCacheLineSize",
    "SDL_HasRDTSC", "SDL_HasAltiVec", "SDL_HasMMX", "SDL_Has3DNow",
    "SDL_HasSSE", "SDL_HasSSE2", "SDL_HasSSE3", "SDL_HasSSE41",
    "SDL_HasSSE42", "SDL_GetSystemRAM", "SDL_HasAVX", "SDL_HasAVX2",
    "SDL_HasAVX512F", "SDL_HasARMSIMD", "SDL_HasNEON",
    "SDL_SIMDGetAlignment", "SDL_SIMDAlloc", "SDL_SIMDRealloc", "SDL_SIMDFree"
]


SDL_CACHELINE_SIZE = 128
SDL_GetCPUCount = _bind("SDL_GetCPUCount", None, c_int)
SDL_GetCPUCacheLineSize = _bind("SDL_GetCPUCacheLineSize", None, c_int)
SDL_HasRDTSC = _bind("SDL_HasRDTSC", None, SDL_bool)
SDL_HasAltiVec = _bind("SDL_HasAltiVec", None, SDL_bool)
SDL_HasMMX = _bind("SDL_HasMMX", None, SDL_bool)
SDL_Has3DNow = _bind("SDL_Has3DNow", None, SDL_bool)
SDL_HasSSE = _bind("SDL_HasSSE", None, SDL_bool)
SDL_HasSSE2 = _bind("SDL_HasSSE2", None, SDL_bool)
SDL_HasSSE3 = _bind("SDL_HasSSE3", None, SDL_bool)
SDL_HasSSE41 = _bind("SDL_HasSSE41", None, SDL_bool)
SDL_HasSSE42 = _bind("SDL_HasSSE42", None, SDL_bool)
SDL_GetSystemRAM = _bind("SDL_GetSystemRAM", None, c_int)
SDL_HasAVX = _bind("SDL_HasAVX", None, SDL_bool)
SDL_HasAVX2 = _bind("SDL_HasAVX2", None, SDL_bool, added='2.0.4')
SDL_HasAVX512F = _bind("SDL_HasAVX512F", None, SDL_bool, added='2.0.9')
SDL_HasARMSIMD = _bind("SDL_HasARMSIMD", None, SDL_bool, added='2.0.12')
SDL_HasNEON = _bind("SDL_HasNEON", None, SDL_bool, added='2.0.6')
SDL_SIMDGetAlignment = _bind("SDL_SIMDGetAlignment", None, c_size_t, added='2.0.10')
SDL_SIMDAlloc = _bind("SDL_SIMDAlloc", [c_size_t], c_void_p, added='2.0.10')
SDL_SIMDRealloc = _bind("SDL_SIMDRealloc", [c_void_p, c_size_t], c_void_p, added='2.0.14')
SDL_SIMDFree = _bind("SDL_SIMDFree", [c_void_p], None, added='2.0.10')
