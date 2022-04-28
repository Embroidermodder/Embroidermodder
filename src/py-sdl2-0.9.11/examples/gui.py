"""User interface examples."""
import sys
import sdl2
import sdl2.ext

# Define some global color constants
WHITE = sdl2.ext.Color(255, 255, 255)
GREY = sdl2.ext.Color(200, 200, 200)
RED = sdl2.ext.Color(255, 0, 0)
GREEN = sdl2.ext.Color(0, 255, 0)

# Create a resource, so we have easy access to the example images.
RESOURCES = sdl2.ext.Resources(__file__, "resources")


# A callback for the Button.motion event.
def onmotion(button, event):
    print("Mouse moves over the button!")


# A callback for the Button.click event.
def onclick(button, event):
    print("Button was clicked!")


# A callback for the TextEntry.input event.
def oninput(entry, event):
    print("Input received with text '%s'" % event.text.text)
    print("Text on the entry now is '%s'" % entry.text)


# A callback for the TextEntry.edit event.
def onedit(entry, event):
    print("Edit received with text '%s', start '%d', length '%d'" %
          (event.edit.text, event.edit.start, event.edit.length))


def oncheck(button, event):
    if button.checked:
        color = GREEN
    else:
        color = RED
    if button.factory.sprite_type == sdl2.ext.SOFTWARE:
        sdl2.ext.fill(button.surface, color)
    else:
        # SDL textures do not support color manipulation operation as easy
        # as software surface (since the texture is ideally stored somwhere
        # on the GPU memory in a GPU-specific layout [or not]). To circumvent
        # this, we create a temporary sprite (texture) and exchange the button
        # texture with it.
        tmpsprite = button.factory.from_color(color, button.size)
        button.texture, tmpsprite.texture = tmpsprite.texture, button.texture
        del tmpsprite


def run():
    # You know those from the helloworld.py example.
    # Initialize the video subsystem, create a window and make it visible.
    sdl2.ext.init()
    window = sdl2.ext.Window("UI Elements", size=(800, 600))
    window.show()

    # Create a sprite factory that allows us to create visible 2D elements
    # easily. Depending on what the user chosses, we either create a factory
    # that supports hardware-accelerated sprites or software-based ones.
    # The hardware-accelerated SpriteFactory requres a rendering context
    # (or SDL_Renderer), which will create the underlying textures for us.
    if "-hardware" in sys.argv:
        print("Using hardware acceleration")
        renderer = sdl2.ext.Renderer(window, flags=sdl2.render.SDL_RENDERER_ACCELERATED)
        factory = sdl2.ext.SpriteFactory(sdl2.ext.TEXTURE, renderer=renderer)
    else:
        print("Using software rendering")
        renderer = sdl2.ext.Renderer(window, flags=sdl2.render.SDL_RENDERER_SOFTWARE)
        factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)

    # Create a UI factory, which will handle several defaults for
    # us. Also, the UIFactory can utilises software-based UI elements as
    # well as hardware-accelerated ones; this allows us to keep the UI
    # creation code clean.
    uifactory = sdl2.ext.UIFactory(factory)

    # Create a simple Button sprite, which reacts on mouse movements and
    # button presses and fill it with a white color. All UI elements
    # inherit directly from the TextureSprite (for TEXTURE) or SoftwareSprite
    # (for SOFTWARE), so everything you can do with those classes is also
    # possible for the UI elements.
    button = uifactory.from_image(sdl2.ext.BUTTON,
                                  RESOURCES.get_path("button.bmp"))
    button.position = 50, 50

    # Create a TextEntry sprite, which reacts on keyboard presses and
    # text input.
    entry = uifactory.from_image(sdl2.ext.TEXTENTRY,
                                 RESOURCES.get_path("textentry.bmp"))
    entry.position = 50, 200

    # Create a CheckButton sprite. The CheckButton is a specialised
    # Button, which can switch its state, identified by the 'checked'
    # attribute by clicking.
    checkbutton = uifactory.from_color(sdl2.ext.CHECKBUTTON, RED, size=(50, 50))
    checkbutton.position = 200, 50

    # Bind some actions to the button's event handlers. Whenever a click
    # (combination of a mouse button press and mouse button release), the
    # onclick() function will be called.
    # Whenever the mouse moves around in the area occupied by the button, the
    # onmotion() function will be called.
    # The event handlers receive the issuer of the event as first argument
    # (the button is the issuer of that event) and the SDL event data as second
    # argument for further processing, if necessary.
    button.click += onclick
    button.motion += onmotion

    # Bind some actions to the entry's event handlers. The TextEntry
    # receives input events, once it has been activated by a mouse
    # button press on its designated area. The UIProcessor class takes
    # care of this internally through its activate() method.  If the
    # TextEntry is activated, SDL_TEXTINPUT events are enabled by the
    # relevant SDL2 functions, causing input events to occur, that are
    # handled by the TextEntry.
    entry.input += oninput
    entry.editing += onedit

    checkbutton.click += oncheck
    checkbutton.factory = factory

    # Since all gui elements are sprites, we can use the
    # SpriteRenderSystem class, we learned about in helloworld.py, to
    # draw them on the Window.
    spriterenderer = factory.create_sprite_render_system(window)

    # Create a new UIProcessor, which will handle the user input events
    # and pass them on to the relevant user interface elements.
    uiprocessor = sdl2.ext.UIProcessor()

    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
            # Pass the SDL2 events to the UIProcessor, which takes care of
            # the user interface logic.
            uiprocessor.dispatch([button, checkbutton, entry], event)
        renderer.clear(0)
        # Render all user interface elements on the window.
        spriterenderer.render((button, entry, checkbutton))

    sdl2.ext.quit()
    return 0


if __name__ == "__main__":
    sys.exit(run())
