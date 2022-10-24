import sys
import pytest
from ctypes import create_string_buffer, byref, c_int, c_int16
import sdl2
from sdl2 import SDL_Init, SDL_Quit, SDL_INIT_JOYSTICK
from sdl2.events import SDL_QUERY, SDL_ENABLE, SDL_IGNORE
from sdl2.stdinc import SDL_TRUE, SDL_FALSE
from sdl2.error import SDL_GetError, SDL_ClearError
from sdl2 import joystick

# Get status of joystick support/availability before running tests
any_joysticks = False
SDL_ClearError()
ret = SDL_Init(SDL_INIT_JOYSTICK)
joystick_works = ret == 0 and SDL_GetError() == b""
if joystick_works:
    devices = joystick.SDL_NumJoysticks()
    if sdl2.dll.version >= 2014:
        # On 2.0.14 and above, we can test with a virtual joystick
        devices += 1
    any_joysticks = devices > 0
SDL_Quit()

pytestmark = pytest.mark.skipif(not joystick_works, reason="system unsupported")

joystick_types = [
    joystick.SDL_JOYSTICK_TYPE_UNKNOWN,
    joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER,
    joystick.SDL_JOYSTICK_TYPE_WHEEL,
    joystick.SDL_JOYSTICK_TYPE_ARCADE_STICK,
    joystick.SDL_JOYSTICK_TYPE_FLIGHT_STICK,
    joystick.SDL_JOYSTICK_TYPE_DANCE_PAD,
    joystick.SDL_JOYSTICK_TYPE_GUITAR,
    joystick.SDL_JOYSTICK_TYPE_DRUM_KIT,
    joystick.SDL_JOYSTICK_TYPE_ARCADE_PAD,
    joystick.SDL_JOYSTICK_TYPE_THROTTLE,
]


# Overrides global fixture with one that initializes the joystick system
@pytest.fixture(scope="module")
def with_sdl():
    sdl2.SDL_ClearError()
    sdl2.SDL_SetHint(b"SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", b"1")
    ret = sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO | sdl2.SDL_INIT_JOYSTICK)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    # Also initialize a virtual joystick (if supported)
    if sdl2.dll.version >= 2014:
        virt_type = joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER
        virt_index = joystick.SDL_JoystickAttachVirtual(virt_type, 2, 4, 1)
    yield
    sdl2.SDL_Quit()

@pytest.fixture()
def joysticks(with_sdl):
    devices = []
    count = joystick.SDL_NumJoysticks()
    for i in range(count):
        stick = joystick.SDL_JoystickOpen(i)
        assert sdl2.SDL_GetError() == b""
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        devices.append(stick)
    yield devices
    for stick in devices:
        joystick.SDL_JoystickClose(stick)

def is_virtual(stick):
    virtual = False
    if isinstance(stick, int):
        if sdl2.dll.version >= 2014:
            virtual = joystick.SDL_JoystickIsVirtual(stick) == SDL_TRUE
    elif isinstance(stick.contents, joystick.SDL_Joystick):
        name = joystick.SDL_JoystickName(stick)
        virtual = name == b"Virtual Joystick"
    return virtual


# TODO: Make one of the tests gather/print out current joystick info

def test_SDL_JoystickGetGUIDFromString():
    guid_str = b'030000007e050000060300001c3a0000' # Wiimote on macOS
    expected = [3, 0, 0, 0, 126, 5, 0, 0, 6, 3, 0, 0, 28, 58, 0, 0]
    guid = joystick.SDL_JoystickGetGUIDFromString(guid_str)
    assert list(guid.data) == expected

def test_SDL_JoystickGetGUIDString():
    guid_str = b'030000007e050000060300001c3a0000' # Wiimote on macOS
    guid = joystick.SDL_JoystickGetGUIDFromString(guid_str)
    buff = create_string_buffer(33)
    joystick.SDL_JoystickGetGUIDString(guid, buff, 33) # Get GUID string
    assert guid_str == buff.value

def test_SDL_NumJoysticks():
    if SDL_Init(SDL_INIT_JOYSTICK) != 0:
        pytest.skip("joystick subsystem not supported")
    retval = joystick.SDL_NumJoysticks()
    SDL_Quit()
    assert retval >= 0

def test_SDL_JoystickNameForIndex(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        name = joystick.SDL_JoystickNameForIndex(index)
        assert type(name) in (str, bytes)

def test_SDL_JoystickOpenClose(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        stick = joystick.SDL_JoystickOpen(index)
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        joystick.SDL_JoystickClose(stick)

def test_SDL_JoystickName(joysticks):
    names = []
    for stick in joysticks:
        name = joystick.SDL_JoystickName(stick)
        assert type(name) in (str, bytes)
        names.append(name.decode('utf-8'))
    print(names)

def test_SDL_JoystickGetDeviceGUID(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        guid = joystick.SDL_JoystickGetDeviceGUID(index)
        assert isinstance(guid, joystick.SDL_JoystickGUID)
        guidlist = list(guid.data)
        assert isinstance(guidlist[0], int)

def test_SDL_JoystickGetGUID(joysticks):
    for stick in joysticks:
        guid = joystick.SDL_JoystickGetGUID(stick)
        assert isinstance(guid, joystick.SDL_JoystickGUID)
        guidlist = list(guid.data)
        assert isinstance(guidlist[0], int)

def test_SDL_JoystickGetAttached(joysticks):
    for stick in joysticks:
        ret = joystick.SDL_JoystickGetAttached(stick)
        assert ret in [SDL_FALSE, SDL_TRUE]

def test_SDL_JoystickInstanceID(joysticks):
    for stick in joysticks:
        ret = joystick.SDL_JoystickInstanceID(stick)
        assert ret >= 0

def test_SDL_JoystickNumAxes(joysticks):
    for stick in joysticks:
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        axes = joystick.SDL_JoystickNumAxes(stick)
        assert axes >= 0
        if is_virtual(stick):
            assert axes == 2

def test_SDL_JoystickNumBalls(joysticks):
    for stick in joysticks:
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        balls = joystick.SDL_JoystickNumBalls(stick)
        assert balls >= 0

def test_SDL_JoystickNumHats(joysticks):
    for stick in joysticks:
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        hats = joystick.SDL_JoystickNumHats(stick)
        assert hats >= 0
        if is_virtual(stick):
            assert hats == 1

def test_SDL_JoystickNumButtons(joysticks):
    for stick in joysticks:
        assert isinstance(stick.contents, joystick.SDL_Joystick)
        buttons = joystick.SDL_JoystickNumButtons(stick)
        assert buttons >= 0
        if is_virtual(stick):
            assert buttons == 4

def test_SDL_JoystickUpdate(with_sdl):
    # NOTE: Returns void, can't really test anything else
    joystick.SDL_JoystickUpdate()
    assert SDL_GetError() == b""

def test_SDL_JoystickEventState(with_sdl):
    for state in (SDL_IGNORE, SDL_ENABLE):
        news = joystick.SDL_JoystickEventState(state)
        assert news == state
        query = joystick.SDL_JoystickEventState(SDL_QUERY)
        assert query == state

def test_SDL_JoystickGetAxis(joysticks):
    for stick in joysticks:
        for axis in range(joystick.SDL_JoystickNumAxes(stick)):
            val = joystick.SDL_JoystickGetAxis(stick, axis)
            assert -32768 <= val <= 32767

def test_SDL_JoystickGetBall(joysticks):
    numball = [joystick.SDL_JoystickNumBalls(s) for s in joysticks]
    if not any(numball):
        pytest.skip("no trackball on any connected controller")
    dx, dy = c_int(0), c_int(0)
    get_ball = joystick.SDL_JoystickGetBall
    for stick in sticks:
        for ball in range(joystick.SDL_JoystickNumBalls(stick)):
            ret = get_ball(stick, ball, byref(dx), byref(dy))
            assert SDL_GetError() == b""
            assert ret == 0

def test_SDL_JoystickGetHat(joysticks):
    hatvals = [
        joystick.SDL_HAT_UP, joystick.SDL_HAT_DOWN, joystick.SDL_HAT_LEFT,
        joystick.SDL_HAT_RIGHT, joystick.SDL_HAT_CENTERED,
        joystick.SDL_HAT_LEFTUP, joystick.SDL_HAT_LEFTDOWN,
        joystick.SDL_HAT_RIGHTUP, joystick.SDL_HAT_RIGHTDOWN
    ]
    for stick in joysticks:
        for hat in range(joystick.SDL_JoystickNumHats(stick)):
            val = joystick.SDL_JoystickGetHat(stick, hat)
            assert val in hatvals

def test_SDL_JoystickGetButton(joysticks):
    for stick in joysticks:
        for button in range(joystick.SDL_JoystickNumButtons(stick)):
            val = joystick.SDL_JoystickGetButton(stick, button)
            assert val in [0, 1]

def test_SDL_JoystickCurrentPowerLevel(joysticks):
    levels = [
        joystick.SDL_JOYSTICK_POWER_UNKNOWN,
        joystick.SDL_JOYSTICK_POWER_EMPTY,
        joystick.SDL_JOYSTICK_POWER_LOW,
        joystick.SDL_JOYSTICK_POWER_MEDIUM,
        joystick.SDL_JOYSTICK_POWER_FULL,
        joystick.SDL_JOYSTICK_POWER_WIRED,
        joystick.SDL_JOYSTICK_POWER_MAX,
    ]
    for stick in joysticks:
        pwr = joystick.SDL_JoystickCurrentPowerLevel(stick)
        assert SDL_GetError() == b""
        assert pwr in levels

def test_SDL_JoystickFromInstanceID(joysticks):
    for stick in joysticks:
        iid = joystick.SDL_JoystickInstanceID(stick)
        assert iid >= 0
        stick2 = joystick.SDL_JoystickFromInstanceID(iid)
        name = joystick.SDL_JoystickName(stick)
        assert joystick.SDL_JoystickName(stick2) == name

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_JoystickFromPlayerIndex(joysticks):
    i = 0
    for stick in joysticks:
        joystick.SDL_JoystickSetPlayerIndex(stick, i)
        stick2 = joystick.SDL_JoystickFromPlayerIndex(i)
        name = joystick.SDL_JoystickName(stick)
        assert joystick.SDL_JoystickName(stick2) == name
        i += 1

def test_SDL_JoystickGetVendor(joysticks):
    for stick in joysticks:
        vid = joystick.SDL_JoystickGetVendor(stick)
        assert SDL_GetError() == b""
        if not is_virtual(stick):
            assert vid > 0

def test_SDL_JoystickGetProduct(joysticks):
    for stick in joysticks:
        pid = joystick.SDL_JoystickGetProduct(stick)
        assert SDL_GetError() == b""
        if not is_virtual(stick):
            assert pid > 0

def test_SDL_JoystickGetProductVersion(joysticks):
    for stick in joysticks:
        pver = joystick.SDL_JoystickGetProductVersion(stick)
        assert SDL_GetError() == b""
        assert pver >= 0

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_JoystickGetSerial(joysticks):
    for stick in joysticks:
        serial = joystick.SDL_JoystickGetSerial(stick)
        assert SDL_GetError() == b""
        assert serial == None or type(serial) in (str, bytes)

def test_SDL_JoystickGetAxisInitialState(joysticks):
    init_state = c_int16(0)
    for stick in joysticks:
        for axis in range(joystick.SDL_JoystickNumAxes(stick)):
            ret = joystick.SDL_JoystickGetAxisInitialState(
                stick, axis, byref(init_state)
            )
            assert SDL_GetError() == b""
            assert -32768 <= init_state.value <= 32767
            assert ret in [SDL_TRUE, SDL_FALSE]

def test_SDL_JoystickGetType(joysticks):
    for stick in joysticks:
        jtype = joystick.SDL_JoystickGetType(stick)
        assert SDL_GetError() == b""
        assert jtype in joystick_types
        if is_virtual(stick):
            assert jtype == joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER

def test_SDL_JoystickGetDeviceVendor(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        vid = joystick.SDL_JoystickGetDeviceVendor(index)
        assert SDL_GetError() == b""
        if not is_virtual(index):
            assert vid > 0

def test_SDL_JoystickGetDeviceProduct(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        pid = joystick.SDL_JoystickGetDeviceProduct(index)
        assert SDL_GetError() == b""
        if not is_virtual(index):
            assert pid > 0

def test_SDL_JoystickGetDeviceProductVersion(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        pver = joystick.SDL_JoystickGetDeviceProductVersion(index)
        assert SDL_GetError() == b""
        assert pver >= 0

def test_SDL_JoystickGetDeviceType(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        jtype = joystick.SDL_JoystickGetDeviceType(index)
        assert SDL_GetError() == b""
        assert jtype in joystick_types
        if is_virtual(index):
            assert jtype == joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_JoystickGetDeviceInstanceID(joysticks):
    for index in range(len(joysticks)):
        iid = joystick.SDL_JoystickGetDeviceInstanceID(index)
        assert SDL_GetError() == b""
        stick = joysticks[index]
        assert iid == joystick.SDL_JoystickInstanceID(stick)

@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_LockUnlockJoysticks(with_sdl):
    # NOTE: Not sure how to test these more comprehensively
    joystick.SDL_LockJoysticks()
    joystick.SDL_UnlockJoysticks()

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_JoystickGetPlayerIndex(joysticks):
    for stick in joysticks:
        player = joystick.SDL_JoystickGetPlayerIndex(stick)
        assert player in [-1, 0, 1, 2, 3]

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_JoystickSetPlayerIndex(joysticks):
    i = 0
    for stick in joysticks:
        joystick.SDL_JoystickSetPlayerIndex(stick, i)
        player = joystick.SDL_JoystickGetPlayerIndex(stick)
        assert player == i
        i += 1

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_JoystickGetDevicePlayerIndex(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        player = joystick.SDL_JoystickGetDevicePlayerIndex(index)
        assert player in [-1, 0, 1, 2, 3]

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_JoystickRumble(joysticks):
    # If we ever add an interactive test suite, this should be moved there
    for stick in joysticks:
        # 50% strength low-frequency, 25% high-frequency rumble for 500ms
        ret = joystick.SDL_JoystickRumble(stick, 32767, 16384, 500)
        assert ret in [-1, 0]

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_JoystickRumbleTriggers(joysticks):
    # If we ever add an interactive test suite, this should be moved there
    for stick in joysticks:
        # 50% strength left trigger, 25% right trigger rumble for 500ms
        ret = joystick.SDL_JoystickRumbleTriggers(stick, 32767, 16384, 500)
        assert ret in [-1, 0]

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_JoystickHasSetLED(joysticks):
    # If we ever add an interactive test suite, this should be moved there
    for stick in joysticks:
        has_led = joystick.SDL_JoystickHasLED(stick)
        assert has_led in [SDL_FALSE, SDL_TRUE]
        expected = -1 if has_led == SDL_FALSE else 0
        cols = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]
        for r, g, b in cols:
            ret = joystick.SDL_JoystickSetLED(stick, r, g, b)
            assert ret == expected

@pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
def test_SDL_JoystickHasRumble(joysticks):
    # If we ever add an interactive test suite, this should be moved there
    for stick in joysticks:
        has_rumble = joystick.SDL_JoystickHasRumble(stick)
        assert has_rumble in [SDL_FALSE, SDL_TRUE]

@pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
def test_SDL_JoystickHasRumbleTriggers(joysticks):
    # If we ever add an interactive test suite, this should be moved there
    for stick in joysticks:
        has_rumble_triggers = joystick.SDL_JoystickHasRumbleTriggers(stick)
        assert has_rumble_triggers in [SDL_FALSE, SDL_TRUE]

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2016, reason="not available")
def test_SDL_JoystickSendEffect(joysticks):
    # NOTE: Not supported on macOS or Linux, and effect data would be specific
    # to each controller type, so can't easily test this.
    pass

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_JoystickVirtual(with_sdl):
    jcount = joystick.SDL_NumJoysticks()
    jtype = joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER
    index = joystick.SDL_JoystickAttachVirtual(jtype, 1, 2, 1)
    assert index >= 0
    assert joystick.SDL_JoystickIsVirtual(index) == SDL_TRUE
    assert joystick.SDL_NumJoysticks() == (jcount + 1)
    stick = joystick.SDL_JoystickOpen(index)
    # Test joystick configuration
    assert joystick.SDL_JoystickNumAxes(stick) == 1
    assert joystick.SDL_JoystickNumButtons(stick) == 2
    assert joystick.SDL_JoystickNumHats(stick) == 1
    # Try setting and checking for some virtual values
    assert joystick.SDL_JoystickSetVirtualAxis(stick, 0, -30) == 0
    assert joystick.SDL_JoystickSetVirtualButton(stick, 0, 255) == 0
    assert joystick.SDL_JoystickSetVirtualButton(stick, 1, 128) == 0
    assert joystick.SDL_JoystickSetVirtualHat(stick, 0, 36) == 0
    joystick.SDL_JoystickUpdate()
    # NOTE: SDL2 doesn't update joystick values unless it has a window that
    # has input focus. There's a hint to disable that but it doesn't seem to
    # work, so for now these tests are disabled.
    #assert joystick.SDL_JoystickGetAxis(stick, 0) == -30
    #assert joystick.SDL_JoystickGetButton(stick, 0) == 255
    #assert joystick.SDL_JoystickGetButton(stick, 1) == 128
    #assert joystick.SDL_JoystickGetHat(stick, 0) == 36
    # Check that removing the virtual joystick works properly
    joystick.SDL_JoystickClose(stick)
    jcount = joystick.SDL_NumJoysticks()
    assert joystick.SDL_JoystickDetachVirtual(index) == 0
    assert joystick.SDL_NumJoysticks() == (jcount - 1)
