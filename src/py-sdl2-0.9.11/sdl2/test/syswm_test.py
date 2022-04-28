import os
import sys
import pytest
import ctypes
from sdl2.stdinc import SDL_TRUE
from sdl2 import video, syswm, version, SDL_GetError

# Check if using dummy video driver
DRIVER_DUMMY = os.getenv("SDL_VIDEODRIVER", "") == "dummy"


@pytest.mark.skipif(DRIVER_DUMMY, reason="Doesn't work with dummy driver")
def test_SDL_GetWindowWMInfo(with_sdl):
    unix_wms = [
        syswm.SDL_SYSWM_X11, syswm.SDL_SYSWM_DIRECTFB,
        syswm.SDL_SYSWM_WAYLAND, syswm.SDL_SYSWM_KMSDRM, 
    ]
    platform_wms = {
        "windows": [syswm.SDL_SYSWM_WINDOWS],
        "darwin": [syswm.SDL_SYSWM_COCOA],
        "unixlike": unix_wms,
        "os2": [syswm.SDL_SYSWM_OS2],
    }
    # Create a window and retrieve the current window manager info
    window = video.SDL_CreateWindow(
        b"Test", 10, 10, 10, 10, video.SDL_WINDOW_HIDDEN
    )
    wminfo = syswm.SDL_SysWMinfo()
    version.SDL_VERSION(wminfo.version)
    ret = syswm.SDL_GetWindowWMInfo(window, ctypes.byref(wminfo))
    video.SDL_DestroyWindow(window)
    assert SDL_GetError() == b""
    assert ret == SDL_TRUE
    # Test window manager types for different platforms
    platform = sys.platform
    if platform in ("win32", "cygwin", "msys"):
        platform = "windows"
    elif "linux" in platform or "bsd" in platform:
        platform = "unixlike"
    if platform in platform_wms.keys():
        assert wminfo.subsystem in platform_wms[platform]
