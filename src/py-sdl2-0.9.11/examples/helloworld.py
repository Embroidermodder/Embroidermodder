"""The almighty Hello World! example"""
# We'll use sys to properly exit with an error code.
import os
import sys
import sdl2.ext

# Create a resource container, so that we can easily access all
# resource, we bundle with our application. We are using the current
# file's location and define the "resources" subdirectory as the
# location, in which we keep all data.
filepath = os.path.abspath(os.path.dirname(__file__))
RESOURCES = sdl2.ext.Resources(filepath, "resources")


def run():
    # Initialize the video system - this implicitly initializes some
    # necessary parts within the SDL2 DLL used by the video module.
    #
    # You SHOULD call this before using any video related methods or
    # classes.
    sdl2.ext.init()

    # Create a new window (like your browser window or editor window,
    # etc.) and give it a meaningful title and size. We definitely need
    # this, if we want to present something to the user.
    window = sdl2.ext.Window("Hello World!", size=(592, 460))

    # By default, every Window is hidden, not shown on the screen right
    # after creation. Thus we need to tell it to be shown now.
    window.show()

    # Create a Renderer for the new window, which we can use to copy and
    # draw things to the screen. Renderers can use hardware-accelerated
    # backends (e.g. OpenGL, Direct3D) as well as software-accelerated ones,
    # depending on the flags you create it with.
    renderflags = sdl2.SDL_RENDERER_SOFTWARE
    if "-hardware" in sys.argv:
        renderflags = (
            sdl2.SDL_RENDERER_ACCELERATED | sdl2.SDL_RENDERER_PRESENTVSYNC
        )
    renderer = sdl2.ext.Renderer(window, flags=renderflags)

    # Import an image file and convert it to a Texture. A Texture is an SDL
    # surface that has been prepared for use with a given Renderer. 
    tst_img = sdl2.ext.load_bmp(RESOURCES.get_path("hello.bmp"))
    tx = sdl2.ext.Texture(renderer, tst_img)

    # Display the image on the window. This code takes the texture we created
    # earlier and copies it to the renderer (with the top-left corner of the
    # texture placed at the coordinates (0, 0) on the window surface), then 
    # takes the contents of the renderer surface and presents them on its
    # associated window.
    renderer.copy(tx, dstrect=(0, 0))
    renderer.present()

    # Create a simple event loop. This fetches the SDL2 event queue and checks
    # for any quit events. Once a quit event is received, the loop will end
    # and we'll send the signal to quit the program.
    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break

    # Now that we're done with the SDL2 library, we shut it down nicely using
    # the `sdl2.ext.quit` function.
    sdl2.ext.quit()
    return 0

if __name__ == "__main__":
    sys.exit(run())
