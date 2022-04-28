import ctypes
from .. import SDL_Init, SDL_Quit, SDL_QuitSubSystem, SDL_WasInit, \
    SDL_INIT_VIDEO, error, events, timer

_HASSDLTTF = True
try:
    from .. import sdlttf
except ImportError:
    _HASSDLTTF = False
_HASSDLIMAGE = True
try:
    from .. import sdlimage
except ImportError:
    _HASSDLIMAGE = False

__all__ = ["SDLError", "init", "quit", "get_events", "TestEventProcessor"]


class SDLError(Exception):
    """An SDL2-specific exception class."""

    def __init__(self, msg=None):
        """Creates a new SDLError instance with the specified message.

        If no msg is passed, it will try to get the current SDL2 error via
        :func:`sdl2.SDL_GetError`.

        """
        super(SDLError, self).__init__()
        self.msg = msg
        if not msg:
            self.msg = error.SDL_GetError()
            error.SDL_ClearError()

    def __str__(self):
        return repr(self.msg)


def raise_sdl_err(desc=None):
    # Raises and clears the latest SDL error. For internal use.
    errmsg = error.SDL_GetError().decode('utf-8')
    error.SDL_ClearError()
    e = "Error encountered"
    if desc:
        e += " " + desc
    if len(errmsg):
        e += ": {0}".format(errmsg)
    raise SDLError(e)


def init():
    """Initializes the SDL2 video subsystem.

    See :ref:`pygamers_pygame` for a comparison between this function and
    ``pygame.init()``.

    Raises:
        :exc:`SDLError`: If the SDL2 video subsystem cannot be initialized.

    """
    # TODO: More subsystems?
    if SDL_Init(SDL_INIT_VIDEO) != 0:
        raise SDLError()


def quit():
    """Quits the SDL2 video subysystem.

    If no other subsystems are active, this will also call
    :func:`sdl2.SDL_Quit`, :func:`sdlttf.TTF_Quit` and
    :func:`sdlimage.IMG_Quit`.

    """
    # TODO: More subsystems? Also, is TTF_WasInit always 1?
    SDL_QuitSubSystem(SDL_INIT_VIDEO)
    if SDL_WasInit(0) != 0:
        if _HASSDLTTF and sdlttf.TTF_WasInit() > 0:
            sdlttf.TTF_Quit()
        if _HASSDLIMAGE:
            sdlimage.IMG_Quit()
        SDL_Quit()


def get_events():
    """Gets all SDL events that are currently on the event queue.

    Returns:
        :obj:`List`: A list of all :obj:`~sdl2.SDL_Event` objects currently in
        the event queue.
    
    """
    events.SDL_PumpEvents()

    evlist = []
    SDL_PeepEvents = events.SDL_PeepEvents

    op = events.SDL_GETEVENT
    first = events.SDL_FIRSTEVENT
    last = events.SDL_LASTEVENT

    while True:
        evarray = (events.SDL_Event * 10)()
        ptr = ctypes.cast(evarray, ctypes.POINTER(events.SDL_Event))
        ret = SDL_PeepEvents(ptr, 10, op, first, last)
        if ret <= 0:
            break
        evlist += list(evarray)[:ret]
        if ret < 10:
            break

    return evlist


class TestEventProcessor(object):
    """A simple event processor for testing purposes."""

    def run(self, window):
        """Starts an event loop without actually processing any event.
        
        This method will run endlessly until an ``SDL_QUIT`` event occurs.

        Args:
            window (:obj:`sdl2.ext.Window`): The window within which to run
                the test event loop.
        
        """
        event = events.SDL_Event()
        running = True
        while running:
            ret = events.SDL_PollEvent(ctypes.byref(event), 1)
            if ret == 1:
                if event.type == events.SDL_QUIT:
                    running = False
                    break
            window.refresh()
            timer.SDL_Delay(10)
