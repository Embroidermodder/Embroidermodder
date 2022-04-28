import sys
import pytest
from ctypes import c_int, cast, byref, POINTER
from sdl2 import SDL_TRUE, SDL_FALSE, SDL_GetError
from sdl2 import rect, keyboard, scancode, keycode, video

byteify = lambda x: x.encode("utf-8")

@pytest.fixture
def window(with_sdl):
    flag = video.SDL_WINDOW_INPUT_FOCUS
    w = video.SDL_CreateWindow(b"Test", 10, 40, 32, 24, flag)
    assert SDL_GetError() == b""
    assert isinstance(w.contents, video.SDL_Window)
    yield w
    video.SDL_DestroyWindow(w)


def test_SDL_Keysym():
    keysym = keyboard.SDL_Keysym()
    assert keysym.scancode == 0
    assert keysym.sym == 0
    assert keysym.mod == 0

    keysym = keyboard.SDL_Keysym(1, 2, 3, ord("b"))
    assert keysym.scancode == 1
    assert keysym.sym == 2
    assert keysym.mod == 3

    uval = "\u0220"
    if sys.version_info[0] < 3:
        uval = unichr(8224)
    keysym = keyboard.SDL_Keysym(17, 32, 88, ord(uval))
    assert keysym.scancode == 17
    assert keysym.sym == 32
    assert keysym.mod == 88


def test_SDL_GetKeyboardFocus(window):
    # NOTE: Would need to actually set keyboard focus for window to test,
    # which I can't seem to get working in pytest
    video.SDL_ShowWindow(window)
    focused_win = keyboard.SDL_GetKeyboardFocus()
    if focused_win:
        focused_id = video.SDL_GetWindowID(focused_win)
        assert video.SDL_GetWindowID(window) == focused_id

def test_SDL_GetKeyboardState(with_sdl):
    # NOTE: This function returns a pointer to an array, which we can use
    # directly as an array in Python since we know how long it is from numkeys
    states = (c_int * scancode.SDL_NUM_SCANCODES)()
    numkeys = c_int(0)
    keystates = keyboard.SDL_GetKeyboardState(byref(numkeys))
    assert numkeys.value > 0
    for key in keystates[:numkeys.value]:
        assert key in [0, 1]

def test_SDL_GetSetModState(with_sdl):
    test_states = [
        keycode.KMOD_NUM | keycode.KMOD_CAPS | keycode.KMOD_MODE,
        keycode.KMOD_LCTRL | keycode.KMOD_LSHIFT,
        keycode.KMOD_CAPS,
    ]
    initial = keyboard.SDL_GetModState()
    for state in test_states:
        keyboard.SDL_SetModState(state)
        assert keyboard.SDL_GetModState() == state
    # Try resetting the modifier state to the initial value
    keyboard.SDL_SetModState(initial)
    assert keyboard.SDL_GetModState() == initial

def test_SDL_GetKeyFromScancode(with_sdl):
    # Test with letter keys
    scan_key_offset = 93
    for scan in range(scancode.SDL_SCANCODE_A, scancode.SDL_SCANCODE_Z + 1):
        key = keyboard.SDL_GetKeyFromScancode(scan)
        assert key == scan + scan_key_offset
    # Test with number keys
    scan_key_offset = 19
    for scan in range(scancode.SDL_SCANCODE_1, scancode.SDL_SCANCODE_9 + 1):
        key = keyboard.SDL_GetKeyFromScancode(scan)
        assert key == scan + scan_key_offset

def test_SDL_GetScancodeFromKey(with_sdl):
    # Test with letter keys
    scan_key_offset = 93
    for scan in range(scancode.SDL_SCANCODE_A, scancode.SDL_SCANCODE_Z + 1):
        key = scan + scan_key_offset
        assert keyboard.SDL_GetScancodeFromKey(key) == scan
    # Test with number keys
    scan_key_offset = 19
    for scan in range(scancode.SDL_SCANCODE_1, scancode.SDL_SCANCODE_9 + 1):
        key = scan + scan_key_offset
        assert keyboard.SDL_GetScancodeFromKey(key) == scan
    # Test with unknown key
    key = keyboard.SDL_GetScancodeFromKey(477)
    assert key == scancode.SDL_SCANCODE_UNKNOWN

def test_SDL_GetScancodeName(with_sdl):
    names = range(ord('A'), ord('Z'))
    xoff = 0
    for code in range(scancode.SDL_SCANCODE_A, scancode.SDL_SCANCODE_Z):
        name = keyboard.SDL_GetScancodeName(code)
        assert name == byteify(chr(names[xoff]))
        xoff += 1
    # Test with unknown scancode
    name = keyboard.SDL_GetScancodeName(0)
    assert name == b""

def test_SDL_GetScancodeFromName(with_sdl):
    codes = range(scancode.SDL_SCANCODE_A, scancode.SDL_SCANCODE_Z)
    xoff = 0
    for key in range(ord('a'), ord('z')):
        ch = chr(key)
        code = keyboard.SDL_GetScancodeFromName(byteify(ch))
        assert code == codes[xoff]
        xoff += 1
    # Test with invalid key name
    key = keyboard.SDL_GetScancodeFromName(b"")
    assert key == scancode.SDL_SCANCODE_UNKNOWN

def test_SDL_GetKeyName(with_sdl):
    x = 65  # SDL maps everything against upper-case letters
    for key in range(ord('a'), ord('z')):
        ch = chr(x)
        name = keyboard.SDL_GetKeyName(key)
        assert name == byteify(ch)
        x += 1

def test_SDL_GetKeyFromName(with_sdl):
    # Test with lower-case ASCII characters
    for x in range(26):
        key = keyboard.SDL_GetKeyFromName(byteify(chr(x + 97)))
        assert key == x + 97
    # Test with ASCII numbers
    for x in range(10):
        key = keyboard.SDL_GetKeyFromName(("%d" % x).encode("utf-8"))
        assert key == 48 + x
    # Test handling of unknown values
    val = keyboard.SDL_GetKeyFromName(b"not a key")
    assert val == keycode.SDLK_UNKNOWN

def test_SDL_StartStopTextInput(with_sdl):
    keyboard.SDL_StopTextInput()
    assert SDL_GetError() == b""
    assert keyboard.SDL_IsTextInputActive() == SDL_FALSE
    keyboard.SDL_StartTextInput()
    assert SDL_GetError() == b""
    assert keyboard.SDL_IsTextInputActive() == SDL_TRUE

def test_SDL_SetTextInputRect(with_sdl):
    keyboard.SDL_StartTextInput()
    coords = [(0, 0, 0, 0), (-10, -70, 3, 6), (10, 10, 10, 10)]
    for x, y, w, h in coords:
        r = rect.SDL_Rect(x, y, w, h)
        keyboard.SDL_SetTextInputRect(r)
        assert SDL_GetError() == b""

def test_SDL_HasScreenKeyboardSupport(with_sdl):
    ret = keyboard.SDL_HasScreenKeyboardSupport()
    assert ret in [SDL_TRUE, SDL_FALSE]

def test_SDL_IsScreenKeyboardShown(window):
    ret = keyboard.SDL_IsScreenKeyboardShown(window)
    assert ret in [SDL_TRUE, SDL_FALSE]
