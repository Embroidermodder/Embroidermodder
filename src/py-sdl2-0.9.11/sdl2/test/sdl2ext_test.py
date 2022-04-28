import sys
import pytest
from sdl2 import ext as sdl2ext
from sdl2 import SDL_WasInit, SDL_INIT_VIDEO, SDL_FlushEvent, SDL_USEREVENT, \
    SDL_FIRSTEVENT, SDL_LASTEVENT, SDL_Event, SDL_UserEvent, SDL_PushEvent

@pytest.fixture(scope="module")
def with_sdl_ext():
    sdl2ext.init()
    yield
    sdl2ext.quit()


def test_init_quit():
    # NOTE: Currently init only inits the video subsystem, but quit shuts down
    # SDL2 and ttf/image/mixer libraries. This latter function should be tested.
    try:
        sdl2ext.init()
    except sdl2ext.SDLError:
        raise pytest.skip('Video subsystem not supported')
    assert SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO
    sdl2ext.quit()
    assert SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO
    sdl2ext.init()
    sdl2ext.init()
    sdl2ext.init()
    assert SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO
    sdl2ext.quit()
    assert SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO

def test_get_events(with_sdl_ext):
    SDL_FlushEvent(SDL_FIRSTEVENT, SDL_LASTEVENT)
    for x in range(12):
        event = SDL_Event()
        event.type = SDL_USEREVENT + x
        event.user = SDL_UserEvent(
            type=event.type, timestamp=0, windowID=0, code=0
        )
        SDL_PushEvent(event)
    results = sdl2ext.get_events()
    assert len(results) == 12
    for idx, r in enumerate(results):
        assert idx == r.type - SDL_USEREVENT

def test_TestEventProcessor(with_sdl_ext):
    # NOTE: This doesn't really test functionality, but since I don't think
    # it's terribly useful I'm not going to bother expanding it
    proc = sdl2ext.TestEventProcessor()
    assert isinstance(proc, sdl2ext.TestEventProcessor)
