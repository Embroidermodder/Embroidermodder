.. _hello_world:

Hello World
===========
Ahhh, the great tradition of saying "Hello World" in a programming
language. To whet your appetite, we will do this with a most simple
application, which will display an image. It is not important to understand
everything at once, which will be used by the example. Nearly all parts used
now are explained in later chapters, so do not hesitate, if the one or other
explanation is missing.

Importing
---------
Let's start with importing some basic modules, which are necessary to
display a small nice window and to do some basic drawing within that
window. ::

    import sys
    import sdl2.ext

    RESOURCES = sdl2.ext.Resources(__file__, "resources")

We need some resources from the ``resources`` folder, so that we have a test
image around to display on the window later on. In your own applications, it is
unlikely that you will ever need to import them, but we need them here, so we
use the :mod:`sdl2.ext.Resources` class to have them available.

Window creation and image loading
---------------------------------
Any graphical application requires access to the screen, mostly in form
of a window, which basically represents a portion of the screen, the
application has access to and the application can manipulate. In most cases
that portion has a border and title bar around it, allowing the user to move
it around on the screen and reorganise everything in a way to fit his needs.

Once we have imported all necessary parts, let's create a window to have
access to the screen, so we can display the logo and thus represent it
to the user. ::

    sdl2.ext.init()

    window = sdl2.ext.Window("Hello World!", size=(640, 480))
    window.show()

    factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)
    sprite = factory.from_image(RESOURCES.get_path("hello.bmp"))

    spriterenderer = factory.create_sprite_render_system(window)
    spriterenderer.render(sprite)

First, we initialise the :mod:`sdl2.ext` internals to gain access to the
screen and to be able to create windows on top of it. Once done with that,
:class:`sdl2.ext.Window` will create the window for us and we
supply a title to be shown on the window's border along with its initial size.
Since :class:`sdl2.ext.Window` instances are not shown by default,
we have to tell the operating system and window manager that there is a new
window to display by calling :meth:`sdl2.ext.Window.show()`.

Afterwards, we get an image from the resources folder and create a
:class:`sdl2.ext.Sprite` from it, which can be easily shown later
on. This is done via a :class:`sdl2.ext.SpriteFactory`, since the
factory allows us to switch between texture-based, hardware-accelerated, and
software-based sprites easily.

To display the image, we will use a :class:`sdl2.ext.SpriteRenderSystem`,
which supports the sprite type (texture- or software-based) and can copy the
image to the window for display. The :class:`sdl2.ext.SpriteRenderSystem`
needs to know, where to copy to, thus we have to supply the window as target
for copy and display operations.

All left to do is to initiate the copy process by calling
:class:`sdl2.ext.SpriteRenderSystem.render()` with the image we
created earlier.

.. tip::

   You will notice that the sprite used above will always be drawn at the
   top-left corner of the :class:`sdl2.ext.Window`. You can change
   the position of where to draw it by changing its
   :attr:`sdl2.ext.Sprite.position` value. ::

        # will cause the renderer to draw the sprite 10px to the right and
        # 20 px to the bottom
        sprite.position = 10, 20

        # will cause the renderer to draw the sprite 55px to the right and
        # 10 px to the bottom
        sprite.position = 55, 10

   Experiment with different values to see their effect. Do not forget to do
   this *before* ``spriterenderer.render(sprite)`` is called.

Making the application responsive
---------------------------------
We are nearly done now. We have an image to display, we have a window, where
the image should be displayed on, so we can execute the written code, not?

Well, yes, but the only thing that will happen is that we will notice a
short flickering before the application exits. Maybe we can even see
the window with the image for a short moment, but that's not what we
want, do we?

To keep the window on the screen and to make it responsive to user
input, such as closing the window, react upon the mouse cursor or key
presses, we have to add a so-called event loop. The event loop will deal
with certain types of actions happening on the window or while the
window is focused by the user and - as long as the event loop is
running - will keep the window shown on the screen. ::

    processor = sdl2.ext.TestEventProcessor()
    processor.run(window)

Since this is a very first tutorial, we keep things simple here and use a
dummy class for testing without actually dealing with the event loop magic.
By calling :meth:`sdl2.ext.TestEventProcessor.run()`, we implicitly start an
event loop, which takes care of the most important parts for us.

And here it ends...
-------------------
The window is shown, the image is shown, great! All left to do is to clean up
everything, once the application finishes. Luckily the
:class:`sdl2.ext.TestEventProcessor` knows when the window is closed, so
it will exit from the event loop. Once it exits, we should clean up the
video internals, we initialised at the beginning. Thus, a final call to ::

    sdl2.ext.quit()

should be made.
