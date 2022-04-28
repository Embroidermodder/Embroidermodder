import sys
import sdl2
import sdl2.ext

# Create a resource container, so that we can easily access all
# resource, we bundle with our application. We are using the current
# file's location and define the "resources" subdirectory as the
# location, in which we keep all data.
RESOURCES = sdl2.ext.Resources(__file__, "resources")

def run():
    # Initialize the video subsystem, create a window and make it visible.
    sdl2.ext.init()
    window = sdl2.ext.Window("Sprite Transformations", size=(800, 600))
    window.show()

    # Create a hardware-accelerated renderer for drawing to the new window.
    # We'll also set the default scaling quality to 'best' for smoother
    # edges when rendering.
    renderer = sdl2.ext.Renderer(window)
    sdl2.ext.set_texture_scale_quality(method="best")

    # Import an image file and convert it to a Texture for the renderer
    tst_img = sdl2.ext.load_bmp(RESOURCES.get_path("hello.bmp"))
    tx = sdl2.ext.Texture(renderer, tst_img)
    
    angle = 0
    flip = 0
    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
            elif event.type == sdl2.SDL_KEYDOWN:
                # Flip the sprite vertically using the up/down arrow keys
                if event.key.keysym.sym in (sdl2.SDLK_DOWN, sdl2.SDLK_UP):
                    flip ^= sdl2.SDL_FLIP_VERTICAL
                # Flip the sprite horizontally using the left/right arrow keys
                elif event.key.keysym.sym in (sdl2.SDLK_LEFT, sdl2.SDLK_RIGHT):
                    flip ^= sdl2.SDL_FLIP_HORIZONTAL
                # Rotate the sprite around its center using the (+/-) keys
                elif event.key.keysym.sym == sdl2.SDLK_EQUALS:
                    angle += 1.0
                    if angle >= 360.0:
                        angle = 0.0
                elif event.key.keysym.sym == sdl2.SDLK_MINUS:
                    angle -= 1.0
                    if angle <= 0.0:
                        angle = 360.0

        # Clear the renderer, copy our texture to it at an offset of (100, 75)
        # from the top-left corner, present it to the window, and wait 10 ms 
        # before moving on
        renderer.clear()
        renderer.copy(tx, dstrect=(100, 75), angle=angle, flip=flip)
        renderer.present()
        sdl2.SDL_Delay(10)

    sdl2.ext.quit()
    return 0

if __name__ == "__main__":
    sys.exit(run())
