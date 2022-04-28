.. _pong-tutorial:

The Pong Game
=============
The following tutorial will show you some capabilities of the component-based
approach, PySDL2 features. We will create the basics of a simple Pong game
implementation here. The basics of creating a event loop, dealing with
user input, moving images around and creating a rendering function are
covered in this tutorial.

Getting started
---------------
We start with creating the window and add a small event loop, so we are able
to close the window and exit the game. ::

    import sys
    import sdl2
    import sdl2.ext


    def run():
        sdl2.ext.init()
        window = sdl2.ext.Window("The Pong Game", size=(800, 600))
        window.show()
        running = True
        while running:
            events = sdl2.ext.get_events()
            for event in events:
                if event.type == sdl2.SDL_QUIT:
                    running = False
                    break
            window.refresh()
        return 0

    if __name__ == "__main__":
        sys.exit(run())

The import statements, video initialisation and window creation were
discussed previously in the :ref:`hello_world` tutorial. We import everything
from the :mod:`sdl2` package here, too, to have all SDL2 functions available.

Instead of some integrated event processor, a new code fragment is
introduced, though. ::

    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
        window.refresh()

The while loop above is the main event loop of our application. It deals with
all kinds of input events that can occur when working with the window, such as
mouse movements, key strokes, resizing operations and so on. SDL handles a lot
for us when it comes to events, so all we need to do is to check, if there are
any events, retrieve each event one by one, and handle it, if necessary. For
now, we will just handle the ``sdl2.SDL_QUIT`` event, which is raised when the
window is about to be closed.

In any other case we will just refresh the window's graphics buffer, so
it is updated and visible on-screen.

Adding the game world
---------------------
The window is available and working. Now let's take care of creating the
game world, which will manage the player paddles, ball, visible elements
and everything else. We are going to use an implementation layout loosely
based on a COP [#f1]_ pattern, which separates data structures and
functionality from each other. This allows us to change or enhance functional
parts easily without having to refactor all classes we are implementing.

We start with creating the two player paddles and the rendering engine
that will display them. ::

    [...]

    WHITE = sdl2.ext.Color(255, 255, 255)

    class SoftwareRenderer(sdl2.ext.SoftwareSpriteRenderSystem):
        def __init__(self, window):
            super(SoftwareRenderer, self).__init__(window)

        def render(self, components):
            sdl2.ext.fill(self.surface, sdl2.ext.Color(0, 0, 0))
            super(SoftwareRenderer, self).render(components)


    class Player(sdl2.ext.Entity):
        def __init__(self, world, sprite, posx=0, posy=0):
            self.sprite = sprite
            self.sprite.position = posx, posy


    def run():
        ...

        world = sdl2.ext.World()

        spriterenderer = SoftwareRenderer(window)
        world.add_system(spriterenderer)

        factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)
        sp_paddle1 = factory.from_color(WHITE, size=(20, 100))
        sp_paddle2 = factory.from_color(WHITE, size=(20, 100))

        player1 = Player(world, sp_paddle1, 0, 250)
        player2 = Player(world, sp_paddle2, 780, 250)

        running = True
        while running:
            events = sdl2.ext.get_events()
            for event in events:
                if event.type == sdl2.SDL_QUIT:
                    running = False
                    break
            world.process()

    if __name__ == "__main__":
        sys.exit(run())

The first thing to do is to enhance the
:class:`sdl2.ext.SoftwareSpriteRenderSystem` so that it will paint
the whole window screen black on every drawing cycle, before drawing all
sprites on the window.

Afterwards, the player paddles will be implemented, based on an
:class:`sdl2.ext.Entity` data container. The player paddles are
simple rectangular sprites that can be positioned anywhere on the
window.

In the main program function, we put those things together by creating a
:class:`sdl2.ext.World`, in which the player paddles and the renderer
can live and operate.

Within the main event loop, we allow the world to process all attached
systems, which causes it to invoke the ``process()`` methods for all
:class:`sdl2.ext.System` instances added to it.

Moving the ball
---------------
We have two static paddles centred vertically on the left and right of
our window. The next thing to do is to add a ball that can move around
within the window boundaries. ::

    [...]
    class MovementSystem(sdl2.ext.Applicator):
        def __init__(self, minx, miny, maxx, maxy):
            super(MovementSystem, self).__init__()
            self.componenttypes = Velocity, sdl2.ext.Sprite
            self.minx = minx
            self.miny = miny
            self.maxx = maxx
            self.maxy = maxy

        def process(self, world, componentsets):
            for velocity, sprite in componentsets:
                swidth, sheight = sprite.size
                sprite.x += velocity.vx
                sprite.y += velocity.vy

                sprite.x = max(self.minx, sprite.x)
                sprite.y = max(self.miny, sprite.y)

                pmaxx = sprite.x + swidth
                pmaxy = sprite.y + sheight
                if pmaxx > self.maxx:
                    sprite.x = self.maxx - swidth
                if pmaxy > self.maxy:
                    sprite.y = self.maxy - sheight


    class Velocity(object):
        def __init__(self):
            super(Velocity, self).__init__()
            self.vx = 0
            self.vy = 0


    class Player(sdl2.ext.Entity):
        def __init__(self, world, posx=0, posy=0):
            [...]
            self.velocity = Velocity()


    class Ball(sdl2.ext.Entity):
        def __init__(self, world, sprite, posx=0, posy=0):
            self.sprite = sprite
            self.sprite.position = posx, posy
            self.velocity = Velocity()


    def run():
        [...]
        sp_ball = factory.from_color(WHITE, size=(20, 20))
        [...]
        movement = MovementSystem(0, 0, 800, 600)
        spriterenderer = SoftwareRenderer(window)

        world.add_system(movement)
        world.add_system(spriterenderer)

        [...]

        ball = Ball(world, sp_ball, 390, 290)
        ball.velocity.vx = -3

        [...]

Two new classes are introduced here, ``Velocity`` and
``MovementSystem``. The ``Velocity`` class is a simple data bag. It
does not contain any application logic, but consists of the relevant
information to represent the movement in a certain direction. This
allows us to mark in-game items as being able to move around.

The ``MovementSystem`` in turn takes care of moving the in-game items around
by applying the velocity to their current position. Thus, we can simply enable
any ``Player`` instance to be movable or not by adding or removing a
velocity attribute to them, which is a ``Velocity`` component instance.

.. note::

   The naming is important here. The EBS implementation as described in
   :ref:`ref-ebs` requires every in-application or in-game item attribute
   bound to a :class:`sdl2.ext.Entity` to be the lowercase class name of its
   related component. ::

      Player.vel = Velocity(10, 10)

   for example would raise an exception, since the system expects
   ``Player.vel`` to be an instance of a ``Vel`` component.

The ``MovementSystem`` is a specialised :class:`sdl2.ext.System`, a
:class:`sdl2.ext.Applicator`, which can operate on combined sets of
data. When the :meth:`sdl2.ext.Applicator.process()` method is
called, the passed ``componentsets`` iterable will contain tuples of
objects that belong to an instance and feature a certain type. The
``MovementSystem``'s ``process()`` implementation hence will loop over
sets of ``Velocity`` and ``Sprite`` instances that belong to the same
:class:`sdl2.ext.Entity`. Since we have a ball and two players
currently available, it typically would loop over three tuples, two for
the individual players and one for the ball.

The :class:`sdl2.ext.Applicator` thus enables us to process combined
data of our in-game items, without creating complex data structures.

.. note::

   Only entities that contain *all* attributes (components) are taken
   into account. If e.g. the ``Ball`` class would not contain a
   ``Velocity`` component, it would not be processed by the
   ``MovementSystem``.

Why do we use this approach? The :class:`sdl2.ext.Sprite` objects carry a
position, which defines the location at which they should be rendered, when
processed by the ``SoftwareRenderer``. If they should move around (which is
a change in the position), we need to apply the velocity to them.

We also define some more things within the ``MovementSystem``, such as a
simple boundary check, so that the players and ball cannot leave the
visible window area on moving around.

Bouncing
--------
We have a ball that can move around as well as the general game logic
for moving things around. In contrast to a classic OO approach we do not
need to implement the movement logic within the ``Ball`` and ``Player``
class individually, since the basic movement is the same for all (yes,
you could have solved that with inheriting ``Ball`` and ``Player`` from
a ``MovableObject`` class in OO).

The ball now moves and stays within the bounds, but once it hits the
left side, it will stay there. To make it *bouncy*, we need to add a
simple collision system, which causes the ball to change its direction
on colliding with the walls or the player paddles. ::

    [...]
    class CollisionSystem(sdl2.ext.Applicator):
        def __init__(self, minx, miny, maxx, maxy):
            super(CollisionSystem, self).__init__()
            self.componenttypes = Velocity, sdl2.ext.Sprite
            self.ball = None
            self.minx = minx
            self.miny = miny
            self.maxx = maxx
            self.maxy = maxy

        def _overlap(self, item):
            pos, sprite = item
            if sprite == self.ball.sprite:
                return False

            left, top, right, bottom = sprite.area
            bleft, btop, bright, bbottom = self.ball.sprite.area

            return (bleft < right and bright > left and
                    btop < bottom and bbottom > top)

        def process(self, world, componentsets):
            collitems = [comp for comp in componentsets if self._overlap(comp)]
            if collitems:
                self.ball.velocity.vx = -self.ball.velocity.vx


    def run():
        [...]
        world = World()

        movement = MovementSystem(0, 0, 800, 600)
        collision = CollisionSystem(0, 0, 800, 600)
        spriterenderer = SoftwareRenderer(window)

        world.add_system(movement)
        world.add_system(collision)
        world.add_system(spriterenderer)

        [...]
        collision.ball = ball

        running = True
        while running:
            events = sdl2.ext.get_events()
            for event in events:
                if event.type == sdl2.SDL_QUIT:
                    running = False
                    break
            sdl2.SDL_Delay(10)
            world.process()

    if __name__ == "__main__":
        sys.exit(run())

The ``CollisionSystem`` only needs to take care of the ball and objects
it collides with, since the ball is the only unpredictable object within our
game world. The player paddles will only be able to move up and down
within the visible window area and we already dealt with that within the
``MovementSystem`` code.

Whenever the ball collides with one of the paddles, its movement
direction (velocity) should be inverted, so that it *bounces* back.

Additionally, we won't run at the full processor speed anymore in the
main loop, but instead add a short delay, using the
:func:`sdl2.SDL_Delay` function. This reduces the overall load on the
CPU and makes the game a bit slower.

Reacting on player input
------------------------
We have a moving ball that bounces from side to side. The next step
would be to allow moving one of the paddles around, if the player presses a
key. The SDL event routines allow us to deal with a huge variety of user and
system events that could occur for our application, but right now we are only
interested in key strokes for the Up and Down keys to move one of the player
paddles up or down. ::

    [...]
    def run():
        [...]
        running = True
        while running:
            events = sdl2.ext.get_events()
            for event in events:
                if event.type == sdl2.SDL_QUIT:
                    running = False
                    break
                if event.type == sdl2.SDL_KEYDOWN:
                    if event.key.keysym.sym == sdl2.SDLK_UP:
                        player1.velocity.vy = -3
                    elif event.key.keysym.sym == sdl2.SDLK_DOWN:
                        player1.velocity.vy = 3
                elif event.type == sdl2.SDL_KEYUP:
                    if event.key.keysym.sym in (sdl2.SDLK_UP, sdl2.SDLK_DOWN):
                        player1.velocity.vy = 0
            sdl2.SDL_Delay(10)
            world.process()

    if __name__ == "__main__":
        sys.exit(run())

Every event that can occur and that is supported by SDL2 can be identified by a
static event type code. This allows us to check for a key stroke, mouse button
press, and so on. First, we have to check for ``sdl2.SDL_KEYDOWN`` and
``sdl2.SDL_KEYUP`` events, so we can start and stop the paddle movement on
demand.  Once we identified such events, we need to check, whether the pressed
or released key is actually the Up or Down key, so that we do not start or stop
moving the paddle, if the user presses R or G or whatever.

Whenever the Up or Down key are pressed down, we allow the left player
paddle to move by changing its velocity information for the vertical
direction. Likewise, if either of those keys is released, we stop moving
the paddle.

Improved bouncing
-----------------
We have a moving paddle and we have a ball that bounces from one side to
another, which makes the game ... quite boring. If you played Pong before,
you know that most variations of it will cause the ball to bounce in a
certain angle, if it collides with a paddle. Most of those
implementations achieve this by implementing the paddle collision as if
the ball collides with a rounded surface. If it collides with the center
of the paddle, it will bounce back straight, if it hits the paddle near
the center, it will bounce back with a pointed angle and on the corners
of the paddle it will bounce back with some angle close to 90 degrees to
its initial movement direction. ::

    class CollisionSystem(sdl2.ext.Applicator):
        [...]

        def process(self, world, componentsets):
            collitems = [comp for comp in componentsets if self._overlap(comp)]
            if collitems:
                self.ball.velocity.vx = -self.ball.velocity.vx

                sprite = collitems[0][1]
                ballcentery = self.ball.sprite.y + self.ball.sprite.size[1] // 2
                halfheight = sprite.size[1] // 2
                stepsize = halfheight // 10
                degrees = 0.7
                paddlecentery = sprite.y + halfheight
                if ballcentery < paddlecentery:
                    factor = (paddlecentery - ballcentery) // stepsize
                    self.ball.velocity.vy = -int(round(factor * degrees))
                elif ballcentery > paddlecentery:
                    factor = (ballcentery - paddlecentery) // stepsize
                    self.ball.velocity.vy = int(round(factor * degrees))
                else:
                    self.ball.velocity.vy = - self.ball.velocity.vy

The reworked processing code above simulates a curved paddle by
creating segmented areas, which cause the ball to be reflected in
different angles. Instead of doing some complex trigonometry to
calculate an accurate angle and transform it on a x/y plane, we simply
check, where the ball collided with the paddle and adjust the vertical
velocity.

If the ball now hits a paddle, it can be reflected at different angles,
hitting the top and bottom window boundaries... and will stay there. If it
hits the window boundaries, it should be reflected, too, but not with a
varying angle, but with the exact angle, it hit the boundary with.
This means that we just need to invert the vertical velocity, once the
ball hits the top or bottom. ::

    class CollisionSystem(sdl2.ext.Applicator):
        [...]

        def process(self, world, componentsets):
            [...]

            if (self.ball.sprite.y <= self.miny or
                self.ball.sprite.y + self.ball.sprite.size[1] >= self.maxy):
                self.ball.velocity.vy = - self.ball.velocity.vy

            if (self.ball.sprite.x <= self.minx or
                self.ball.sprite.x + self.ball.sprite.size[0] >= self.maxx):
                self.ball.velocity.vx = - self.ball.velocity.vx

Creating an enemy
-----------------
Now that we can shoot back the ball in different ways, it would be nice
to have an opponent to play against. We could enhance the main event
loop to recognise two different keys and manipulate the second paddle's
velocity for two people playing against each other. We also could
create a simple computer-controlled player that tries to hit the ball
back to us, which sounds more interesting. ::

    class TrackingAIController(sdl2.ext.Applicator):
        def __init__(self, miny, maxy):
            super(TrackingAIController, self).__init__()
            self.componenttypes = PlayerData, Velocity, sdl2.ext.Sprite
            self.miny = miny
            self.maxy = maxy
            self.ball = None

        def process(self, world, componentsets):
            for pdata, vel, sprite in componentsets:
                if not pdata.ai:
                    continue

                centery = sprite.y + sprite.size[1] // 2
                if self.ball.velocity.vx < 0:
                    # ball is moving away from the AI
                    if centery < self.maxy // 2:
                        vel.vy = 3
                    elif centery > self.maxy // 2:
                        vel.vy = -3
                    else:
                        vel.vy = 0
                else:
                    bcentery = self.ball.sprite.y + self.ball.sprite.size[1] // 2
                    if bcentery < centery:
                        vel.vy = -3
                    elif bcentery > centery:
                        vel.vy = 3
                    else:
                        vel.vy = 0


    class PlayerData(object):
        def __init__(self):
            super(PlayerData, self).__init__()
            self.ai = False


    class Player(sdl2.ext.Entity):
        def __init__(self, world, sprite, posx=0, posy=0, ai=False):
            self.sprite = sprite
            self.sprite.position = posx, posy
            self.velocity = Velocity()
            self.playerdata = PlayerData()
            self.playerdata.ai = ai


    def run():
        [...]
        aicontroller = TrackingAIController(0, 600)

        world.add_system(aicontroller)
        world.add_system(movement)
        world.add_system(collision)
        world.add_system(spriterenderer)

        player1 = Player(world, sp_paddle1, 0, 250)
        player2 = Player(world, sp_paddle2, 780, 250, True)
        [...]
        aicontroller.ball = ball

        [...]

We start by creating a component ``PlayerData`` that flags a player as
being AI controlled or not. Afterwards, a ``TrackingAIController`` is
implemented, which, depending on the information of the ``PlayerData``
component, will move the specific player paddle around by manipulating
its velocity information.

The AI is pretty simple, just following the ball's vertical movement,
trying to hit it at its center, if the ball moves into the direction of
the AI-controlled paddle. As soon as the ball moves away from the
paddle, the paddle will move back to the vertical center.

.. tip::

   Add ``True`` as last parameter to the first ``Player()`` constructor to
   see two AIs playing against each other.

Next steps
----------
We created the basics of a Pong game, which can be found in the
examples folder. However, there are some more things to do, such as

  * resetting the ball to the center with a random vertical velocity, if
    it hits either the left or right window bounds

  * adding the ability to track the points made by either player, if the
    ball hit the left or right side

  * drawing a dashed line in the middle to make the game field look
    nicer

  * displaying the points made by each player

It is your turn now to implement these features. Go ahead, it is not as
complex as it sounds.

  * you can reset the ball's position in the ``CollisionSystem`` code,
    by changing the code for the ``minx`` and ``maxx`` test

  * you could enhance the ``CollisionSystem`` to process ``PlayerData``
    components and add the functionality to add points there (or write a
    small processor that keeps track of the ball only and processes only
    the ``PlayerData`` and ``video.SoftSprite`` objects of each player for
    adding points). Alternatively, you could use the
    :class:`sdl2.ext.EventHandler` class to raise a score count
    function within the ``CollisionSystem``, if the ball collides with
    one of the paddles.

  * write an own render sytem, based on :class:`sdl2.ext.Applicator`,
    which takes care of position and sprite sets ::

       StaticRepeatingSprite(Entity):
           ...
           self.positions = Positions((400, 0), (400, 60), (400, 120), ...)
           ...

  * draw some simple images for 0-9 and render them as sprites,
    depending on the points a player made.

.. rubric:: Footnotes

.. [#f1]  Component-Oriented Programming
