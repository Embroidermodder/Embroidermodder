"""Simple example for using sdl2 directly."""
import os
import sys
import ctypes
import sdl2


def run():
    sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO)
    window = sdl2.SDL_CreateWindow(b"Hello World",
                                   sdl2.SDL_WINDOWPOS_CENTERED,
                                   sdl2.SDL_WINDOWPOS_CENTERED,
                                   592, 460, sdl2.SDL_WINDOW_SHOWN)
    fname = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                         "resources", "hello.bmp")
    image = sdl2.SDL_LoadBMP(fname.encode("utf-8"))
    windowsurface = sdl2.SDL_GetWindowSurface(window)
    sdl2.SDL_BlitSurface(image, None, windowsurface, None)
    sdl2.SDL_UpdateWindowSurface(window)
    sdl2.SDL_FreeSurface(image)

    running = True
    event = sdl2.SDL_Event()
    while running:
        while sdl2.SDL_PollEvent(ctypes.byref(event)) != 0:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
        sdl2.SDL_Delay(10)

    sdl2.SDL_DestroyWindow(window)
    sdl2.SDL_Quit()
    return 0


if __name__ == "__main__":
    sys.exit(run())
