import sys
import pytest
from ctypes import cast, c_char_p
import sdl2
from sdl2 import hints
from sdl2.stdinc import SDL_TRUE, SDL_FALSE

# Need to override global fixture to init/quit on every test
@pytest.fixture
def with_sdl():
    sdl2.SDL_ClearError()
    ret = sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    yield
    sdl2.SDL_Quit()


def test_SDL_ClearHints(with_sdl):
    assert hints.SDL_SetHint(b"TEST", b"32") == 1
    assert hints.SDL_GetHint(b"TEST") == b"32"
    hints.SDL_ClearHints()
    assert hints.SDL_GetHint(b"TEST") == None

def test_SDL_GetHint(with_sdl):
    assert hints.SDL_SetHint(b"TEST", b"32") == 1
    assert hints.SDL_GetHint(b"TEST") == b"32"
    assert hints.SDL_SetHint(hints.SDL_HINT_ALLOW_TOPMOST, b"true") == 1
    assert hints.SDL_GetHint(hints.SDL_HINT_ALLOW_TOPMOST) == b"true"

def test_SDL_SetHint(with_sdl):
    assert hints.SDL_SetHint(b"TEST", b"32") == 1
    assert hints.SDL_GetHint(b"TEST") == b"32"
    assert hints.SDL_SetHint(b"TEST", b"abcdef") == 1
    assert hints.SDL_GetHint(b"TEST") == b"abcdef"
    assert hints.SDL_SetHint(b"", b"hi") == 1
    assert hints.SDL_GetHint(b"") == b"hi"

def test_SDL_SetHintWithPriority(with_sdl):
    tst_hints = [
        [hints.SDL_HINT_DEFAULT, b"abc", b"abc"],
        [hints.SDL_HINT_NORMAL, b"ABC", b"ABC"],
        [hints.SDL_HINT_OVERRIDE, b"123", b"123"],
        [hints.SDL_HINT_NORMAL, b"xyz", b"123"],
    ]
    for priority, value, expected in tst_hints:
        ret = hints.SDL_SetHintWithPriority(b"TEST", value, priority)
        assert ret == (1 if value == expected else 0)
        assert hints.SDL_GetHint(b"TEST") == expected

def test_SDL_GetHintBoolean(with_sdl):
    assert hints.SDL_SetHint(b"TEST", b"32") == 1
    assert hints.SDL_GetHintBoolean(b"TEST", SDL_TRUE) == SDL_TRUE
    assert hints.SDL_GetHintBoolean(b"TEST", SDL_FALSE) == SDL_TRUE
    assert hints.SDL_GetHintBoolean(b"TEST2", SDL_FALSE) != SDL_TRUE

def test_SDL_AddDelHintCallback(with_sdl):
    topmost_hint = hints.SDL_HINT_ALLOW_TOPMOST
    calls = []
    def callback(userdata, name, oldval, newval):
        data = cast(userdata, c_char_p)
        calls.append((data.value, name, oldval, newval))
    hintcb = hints.SDL_HintCallback(callback)
    udata = c_char_p(b"banana")
    hints.SDL_AddHintCallback(topmost_hint, hintcb, udata)
    # SDL_AddHintCallback invokes the callback once.
    assert len(calls) == 1
    assert calls[0] == (b"banana", topmost_hint, None, None)
    hints.SDL_SetHint(topmost_hint, b"true")
    assert len(calls) == 2
    assert calls[1] == (b"banana", topmost_hint, None, b"true")
    hints.SDL_DelHintCallback(topmost_hint, hintcb, udata)
    hints.SDL_SetHint(topmost_hint, b"false")
    assert len(calls) == 2
