import sys
import pytest
import sdl2
from ctypes import create_string_buffer, byref
from sdl2 import error

# Override global cleanup fixture since it calls SDL_ClearError
@pytest.fixture(autouse=True)
def sdl_cleanup():
    yield


def test_SDL_GetSetClearError():
    error.SDL_SetError(b"Hi there!")
    assert error.SDL_GetError() == b"Hi there!"
    error.SDL_SetError(b"Error 2");
    assert error.SDL_GetError() == b"Error 2"
    error.SDL_ClearError();
    assert error.SDL_GetError() == b""

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GetErrorMsg():
    error.SDL_SetError(b"123456789")
    assert error.SDL_GetError() == b"123456789"
    buf = create_string_buffer(10)
    assert error.SDL_GetErrorMsg(buf, 10) == b"123456789"
    assert buf.value == b"123456789"
    buf2 = create_string_buffer(5)
    assert error.SDL_GetErrorMsg(buf2, 5) == b"1234"
    assert buf2.value == b"1234"
    error.SDL_ClearError()
