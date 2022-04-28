from ctypes import POINTER, c_int, c_uint, c_uint64, c_void_p, c_char_p
from .dll import _bind
from .stdinc import SDL_bool
from .video import SDL_Window

# NOTE: I have no idea whether this module actually works

__all__ = [
    "SDL_Vulkan_LoadLibrary", "SDL_Vulkan_GetVkGetInstanceProcAddr",
    "SDL_Vulkan_UnloadLibrary", "SDL_Vulkan_GetInstanceExtensions",
    "SDL_Vulkan_CreateSurface", "SDL_Vulkan_GetDrawableSize"
]


VkInstance = c_void_p
# TODO: 32bit/64bit
VkSurfaceKHR = c_uint64

SDL_Vulkan_LoadLibrary = _bind("SDL_Vulkan_LoadLibrary", [c_char_p], c_int, added='2.0.6')
SDL_Vulkan_GetVkGetInstanceProcAddr = _bind("SDL_Vulkan_GetVkGetInstanceProcAddr", None, c_void_p, added='2.0.6')
SDL_Vulkan_UnloadLibrary = _bind("SDL_Vulkan_UnloadLibrary", None, None, added='2.0.6')
SDL_Vulkan_GetInstanceExtensions = _bind("SDL_Vulkan_GetInstanceExtensions", [POINTER(SDL_Window), POINTER(c_uint), POINTER(c_char_p)], SDL_bool, added='2.0.6')
SDL_Vulkan_CreateSurface = _bind("SDL_Vulkan_CreateSurface", [POINTER(SDL_Window), VkInstance, POINTER(VkSurfaceKHR)], SDL_bool, added='2.0.6')
SDL_Vulkan_GetDrawableSize = _bind("SDL_Vulkan_GetDrawableSize", [POINTER(SDL_Window), POINTER(c_int), POINTER(c_int)], None, added='2.0.6')
