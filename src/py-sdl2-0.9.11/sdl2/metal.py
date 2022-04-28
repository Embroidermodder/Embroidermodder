from ctypes import POINTER, c_int, c_void_p
from .dll import _bind
from .video import SDL_Window

__all__ = [
    # Opaque Types
    "SDL_MetalView",
    
    # Functions
    "SDL_Metal_CreateView", "SDL_Metal_DestroyView", "SDL_Metal_GetLayer",
    "SDL_Metal_GetDrawableSize"
]

# NOTE: These functions are currently untested, but proper usage likely involves
# the use of pyobjc to create an NSView from the created SDL_MetalView.


#SDL_MetalView = c_void_p
class SDL_MetalView(c_void_p):
    pass

SDL_Metal_CreateView = _bind("SDL_Metal_CreateView", [POINTER(SDL_Window)], SDL_MetalView, added='2.0.12')
SDL_Metal_DestroyView = _bind("SDL_Metal_DestroyView", [SDL_MetalView], None, added='2.0.12')
SDL_Metal_GetLayer = _bind("SDL_Metal_GetLayer", [SDL_MetalView], c_void_p, added='2.0.14')
SDL_Metal_GetDrawableSize = _bind("SDL_Metal_GetDrawableSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)], None, added='2.0.14')
