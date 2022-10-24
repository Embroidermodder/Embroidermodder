import sys
import array

__all__ = [
    # Defines
    "SDL_LIL_ENDIAN", "SDL_BIG_ENDIAN", "SDL_BYTEORDER",
    
    # Macro & Inline Functions
    "SDL_Swap16", "SDL_Swap32", "SDL_Swap64", "SDL_SwapFloat",
    "SDL_SwapLE16", "SDL_SwapLE32", "SDL_SwapLE64", "SDL_SwapFloatLE",
    "SDL_SwapBE16", "SDL_SwapBE32", "SDL_SwapBE64", "SDL_SwapFloatBE"
]


SDL_LIL_ENDIAN = 1234
SDL_BIG_ENDIAN = 4321
if sys.byteorder == "little":
    SDL_BYTEORDER = SDL_LIL_ENDIAN
else:
    SDL_BYTEORDER = SDL_BIG_ENDIAN

SDL_Swap16 = lambda x: ((x << 8 & 0xFF00) | (x >> 8 & 0x00FF))
SDL_Swap32 = lambda x: (((x << 24) & 0xFF000000) |
                        ((x << 8) & 0x00FF0000) |
                        ((x >> 8) & 0x0000FF00) |
                        ((x >> 24) & 0x000000FF))
SDL_Swap64 = lambda x: ((SDL_Swap32(x & 0xFFFFFFFF) << 32) |
                        (SDL_Swap32(x >> 32 & 0xFFFFFFFF)))
def SDL_SwapFloat(x):
    ar = array.array("d", (x,))
    ar.byteswap()
    return ar[0]

def _nop(x):
    return x
if SDL_BYTEORDER == SDL_LIL_ENDIAN:
    SDL_SwapLE16 = _nop
    SDL_SwapLE32 = _nop
    SDL_SwapLE64 = _nop
    SDL_SwapFloatLE = _nop
    SDL_SwapBE16 = SDL_Swap16
    SDL_SwapBE32 = SDL_Swap32
    SDL_SwapBE64 = SDL_Swap64
    SDL_SwapFloatBE = SDL_SwapFloat
else:
    SDL_SwapLE16 = SDL_Swap16
    SDL_SwapLE32 = SDL_Swap32
    SDL_SwapLE64 = SDL_Swap64
    SDL_SwapFloatLE = SDL_SwapFloat
    SDL_SwapBE16 = _nop
    SDL_SwapBE32 = _nop
    SDL_SwapBE64 = _nop
    SDL_SwapFloatBE = _nop
