import sys
import pytest
import sdl2
from sdl2 import (
    SDL_Init, SDL_Quit, SDL_ClearError, SDL_GetError,
    SDL_FALSE, SDL_TRUE, SDL_IGNORE, SDL_ENABLE, SDL_QUERY
)
from sdl2 import joystick
from sdl2 import gamecontroller as gamepad

# Get status of gamepad support/availability before running tests
SDL_ClearError()
ret = SDL_Init(sdl2.SDL_INIT_GAMECONTROLLER)
gamepad_works = ret == 0 and SDL_GetError() == b""
SDL_Quit()

pytestmark = pytest.mark.skipif(not gamepad_works, reason="system unsupported")

gamepad_types = [
    gamepad.SDL_CONTROLLER_TYPE_UNKNOWN,
    gamepad.SDL_CONTROLLER_TYPE_XBOX360,
    gamepad.SDL_CONTROLLER_TYPE_XBOXONE,
    gamepad.SDL_CONTROLLER_TYPE_PS3,
    gamepad.SDL_CONTROLLER_TYPE_PS4,
    gamepad.SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO,
    gamepad.SDL_CONTROLLER_TYPE_VIRTUAL,
    gamepad.SDL_CONTROLLER_TYPE_PS5,
    gamepad.SDL_CONTROLLER_TYPE_AMAZON_LUNA,
    gamepad.SDL_CONTROLLER_TYPE_GOOGLE_STADIA,
]


# Overrides global fixture with one that initializes the joystick system
@pytest.fixture(scope="module")
def with_sdl():
    sdl2.SDL_ClearError()
    sdl2.SDL_SetHint(b"SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS", b"1")
    ret = sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO | sdl2.SDL_INIT_GAMECONTROLLER)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    # Also initialize a virtual joystick (if supported)
    if sdl2.dll.version >= 2014:
        virt_type = joystick.SDL_JOYSTICK_TYPE_GAMECONTROLLER
        virt_index = joystick.SDL_JoystickAttachVirtual(virt_type, 2, 4, 1)
    yield
    sdl2.SDL_Quit()

@pytest.fixture()
def gamepads(with_sdl):
    devices = []
    count = joystick.SDL_NumJoysticks()
    for i in range(count):
        if gamepad.SDL_IsGameController(i) == SDL_TRUE:
            pad = gamepad.SDL_GameControllerOpen(i)
            assert sdl2.SDL_GetError() == b""
            assert isinstance(pad.contents, gamepad.SDL_GameController)
            devices.append(pad)
    yield devices
    for pad in devices:
        gamepad.SDL_GameControllerClose(pad)

def is_virtual(pad):
    stick = gamepad.SDL_GameControllerGetJoystick(pad)
    return joystick.SDL_JoystickName(stick) == b"Virtual Joystick"


# Test if SDL_GameControllerMappingForGUID is able to be tested
if sys.version_info >= (3, 8, 0) or sdl2.dll.version >= 2006:
    has_mapping_for_guid = True
else:
    has_mapping_for_guid = False


def test_SDL_GameControllerAddMapping(with_sdl):
    newmap = (
        b"030000005e0400002700000006010000,Microsoft SideWinder,"
        b"platform:Mac OS X,a:b0,b:b1,x:b2,y:b3,dpup:-a1,dpdown:+a1,"
        b"dpleft:-a0,dpright:+a0,lefttrigger:b4,righttrigger:b5"
    )
    if sdl2.dll.version >= 2006:
        n1 = gamepad.SDL_GameControllerNumMappings()
        ret = gamepad.SDL_GameControllerAddMapping(newmap)
        assert ret >= 0
        n2 = gamepad.SDL_GameControllerNumMappings()
        assert n2 == n1 + 1
    else:
        # NumMappings not available before 2.0.6
        ret = gamepad.SDL_GameControllerAddMapping(newmap)
        assert ret != -1

@pytest.mark.skipif(not has_mapping_for_guid, reason="not available")
def test_SDL_GameControllerMappingForGUID(with_sdl):
    newmap = (
        b"030000005e0400002700000006010000,Microsoft SideWinder,"
        b"platform:Mac OS X,a:b0,b:b1,x:b2,y:b3,dpup:-a1,dpdown:+a1,"
        b"dpleft:-a0,dpright:+a0,lefttrigger:b4,righttrigger:b5"
    )
    ret = gamepad.SDL_GameControllerAddMapping(newmap)
    assert ret >= 0
    # Get GUID for new mapping
    guid_str = newmap.split(b",")[0]
    guid = joystick.SDL_JoystickGetGUIDFromString(guid_str)
    # Get mapping for GUID
    retmap = gamepad.SDL_GameControllerMappingForGUID(guid)
    assert retmap == newmap

def test_SDL_IsGameController(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        ret = gamepad.SDL_IsGameController(index)
        assert sdl2.SDL_GetError() == b""
        assert ret in [SDL_TRUE, SDL_FALSE]

def test_SDL_GameControllerNameForIndex(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        name = gamepad.SDL_GameControllerNameForIndex(index)
        assert sdl2.SDL_GetError() == b""
        assert name == None or type(name) in (str, bytes)

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_GameControllerTypeForIndex(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        padtype = gamepad.SDL_GameControllerTypeForIndex(index)
        assert padtype in gamepad_types

def test_SDL_GameControllerOpenClose(with_sdl):
    count = joystick.SDL_NumJoysticks()
    for index in range(count):
        if gamepad.SDL_IsGameController(index) == SDL_TRUE:
            pad = gamepad.SDL_GameControllerOpen(index)
            assert sdl2.SDL_GetError() == b""
            assert isinstance(pad.contents, gamepad.SDL_GameController)
            gamepad.SDL_GameControllerClose(pad)

def test_SDL_GameControllerMapping(gamepads):
    for pad in gamepads:
        mapping = gamepad.SDL_GameControllerMapping(pad)
        assert SDL_GetError() == b""
        assert mapping == None or type(mapping) in (str, bytes)

def test_SDL_GameControllerName(gamepads):
    names = []
    for pad in gamepads:
        name = gamepad.SDL_GameControllerName(pad)
        assert type(name) in (str, bytes)
        names.append(name.decode('utf-8'))
    print(names)

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_GameControllerGetType(gamepads):
    for pad in gamepads:
        padtype = gamepad.SDL_GameControllerGetType(pad)
        assert SDL_GetError() == b""
        assert padtype in gamepad_types
        if is_virtual(pad):
            assert padtype == gamepad.SDL_CONTROLLER_TYPE_VIRTUAL

def test_SDL_GameControllerGetAttached(gamepads):
    for pad in gamepads:
        ret = gamepad.SDL_GameControllerGetAttached(pad)
        assert ret in [SDL_FALSE, SDL_TRUE]

def test_SDL_GameControllerGetJoystick(gamepads):
    for pad in gamepads:
        stick = gamepad.SDL_GameControllerGetJoystick(pad)
        assert SDL_GetError() == b""
        assert isinstance(stick.contents, joystick.SDL_Joystick)

def test_SDL_GameControllerEventState(with_sdl):
    for state in (SDL_IGNORE, SDL_ENABLE):
        news = gamepad.SDL_GameControllerEventState(state)
        assert news == state
        query = gamepad.SDL_GameControllerEventState(SDL_QUERY)
        assert query == state

def test_SDL_GameControllerUpdate(with_sdl):
    # NOTE: Returns void, can't really test anything else
    gamepad.SDL_GameControllerUpdate()
    assert SDL_GetError() == b""

def test_SDL_GameControllerGetAxisFromString(with_sdl):
    expected = {
        b'lefty': gamepad.SDL_CONTROLLER_AXIS_LEFTY,
        b'lefttrigger': gamepad.SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        b'notanaxis': gamepad.SDL_CONTROLLER_AXIS_INVALID
    }
    for string in expected.keys():
        a = gamepad.SDL_GameControllerGetAxisFromString(string)
        assert a == expected[string]

def test_SDL_GameControllerGetStringForAxis(with_sdl):
    expected = {
        gamepad.SDL_CONTROLLER_AXIS_LEFTY: b'lefty',
        gamepad.SDL_CONTROLLER_AXIS_TRIGGERLEFT: b'lefttrigger',
        gamepad.SDL_CONTROLLER_AXIS_INVALID: None
    }
    for axis in expected.keys():
        s = gamepad.SDL_GameControllerGetStringForAxis(axis)
        assert s == expected[axis]

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerGetBindForAxis(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerHasAxis(gamepads):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerGetAxis(gamepads):
    pass

def test_SDL_GameControllerGetButtonFromString(with_sdl):
    expected = {
        b'x': gamepad.SDL_CONTROLLER_BUTTON_X,
        b'dpup': gamepad.SDL_CONTROLLER_BUTTON_DPAD_UP,
        b'notabutton': gamepad.SDL_CONTROLLER_BUTTON_INVALID
    }
    for string in expected.keys():
        b = gamepad.SDL_GameControllerGetButtonFromString(string)
        assert b == expected[string]

def test_SDL_GameControllerGetStringForButton(with_sdl):
    expected = {
        gamepad.SDL_CONTROLLER_BUTTON_X: b'x',
        gamepad.SDL_CONTROLLER_BUTTON_DPAD_UP: b'dpup',
        gamepad.SDL_CONTROLLER_BUTTON_INVALID: None
    }
    for button in expected.keys():
        s = gamepad.SDL_GameControllerGetStringForButton(button)
        assert s == expected[button]

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerGetBindForButton(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerHasButton(gamepads):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerGetButton(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerGetNumTouchpads(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerGetNumTouchpadFingers(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerGetTouchpadFinger(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerHasSensor(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerSetSensorEnabled(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerIsSensorEnabled(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2016, reason="not available")
def test_SDL_GameControllerGetSensorDataRate(gamepads):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerGetSensorData(gamepads):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerAddMappingsFromRW(gamepads):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GameControllerAddMappingsFromFile(gamepads):
    pass

def test_SDL_GameControllerFromInstanceID(gamepads):
    for pad in gamepads:
        stick = gamepad.SDL_GameControllerGetJoystick(pad)
        iid = joystick.SDL_JoystickInstanceID(stick)
        assert iid >= 0
        pad2 = gamepad.SDL_GameControllerFromInstanceID(iid)
        name = gamepad.SDL_GameControllerName(pad)
        assert gamepad.SDL_GameControllerName(pad2) == name

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_GameControllerFromPlayerIndex(gamepads):
    i = 0
    for pad in gamepads:
        gamepad.SDL_GameControllerSetPlayerIndex(pad, i)
        pad2 = gamepad.SDL_GameControllerFromPlayerIndex(i)
        name = gamepad.SDL_GameControllerName(pad)
        assert gamepad.SDL_GameControllerName(pad2) == name
        i += 1

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_GameControllerGetPlayerIndex(gamepads):
    for pad in gamepads:
        player = gamepad.SDL_GameControllerGetPlayerIndex(pad)
        assert player in [-1, 0, 1, 2, 3]

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_GameControllerSetPlayerIndex(gamepads):
    i = 0
    for pad in gamepads:
        gamepad.SDL_GameControllerSetPlayerIndex(pad, i)
        player = gamepad.SDL_GameControllerGetPlayerIndex(pad)
        assert player == i
        i += 1

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_GameControllerGetVendor(gamepads):
    for pad in gamepads:
        vid = gamepad.SDL_GameControllerGetVendor(pad)
        assert SDL_GetError() == b""
        if not is_virtual(pad):
            assert vid > 0

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_GameControllerGetProduct(gamepads):
    for pad in gamepads:
        pid = gamepad.SDL_GameControllerGetProduct(pad)
        assert SDL_GetError() == b""
        if not is_virtual(pad):
            assert pid > 0

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_GameControllerGetProductVersion(gamepads):
    for pad in gamepads:
        pver = gamepad.SDL_GameControllerGetProductVersion(pad)
        assert SDL_GetError() == b""
        assert pver >= 0

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerGetSerial(gamepads):
    for pad in gamepads:
        serial = gamepad.SDL_GameControllerGetSerial(pad)
        assert SDL_GetError() == b""
        assert serial == None or type(serial) in (str, bytes)

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_GameControllerNumMappings(with_sdl):
    num = gamepad.SDL_GameControllerNumMappings()
    assert num > 0

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_GameControllerMappingForIndex(with_sdl):
    newmap = (
        b"030000005e0400002700000006010000,Microsoft SideWinder,"
        b"platform:Mac OS X,a:b0,b:b1,x:b2,y:b3,dpup:-a1,dpdown:+a1,"
        b"dpleft:-a0,dpright:+a0,lefttrigger:b4,righttrigger:b5"
    )
    ret = gamepad.SDL_GameControllerAddMapping(newmap)
    assert ret >= 0
    num = gamepad.SDL_GameControllerNumMappings()
    retmap = gamepad.SDL_GameControllerMappingForIndex(num - 1)
    assert newmap == retmap

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_GameControllerMappingForDeviceIndex(with_sdl):
    pass

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_GameControllerRumble(gamepads):
    # If we ever add an interactive test suite, this should be moved there
    for pad in gamepads:
        # 50% strength low-frequency, 25% high-frequency rumble for 500ms
        ret = gamepad.SDL_GameControllerRumble(pad, 32767, 16384, 500)
        assert ret in [-1, 0]

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerRumbleTriggers(gamepads):
    # If we ever add an interactive test suite, this should be moved there
    for pad in gamepads:
        # 50% strength left trigger, 25% right trigger rumble for 500ms
        ret = gamepad.SDL_GameControllerRumbleTriggers(pad, 32767, 16384, 500)
        assert ret in [-1, 0]

@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GameControllerHasSetLED(gamepads):
    # If we ever add an interactive test suite, this should be moved there
    for pad in gamepads:
        has_led = gamepad.SDL_GameControllerHasLED(pad)
        assert has_led in [SDL_FALSE, SDL_TRUE]
        expected = -1 if has_led == SDL_FALSE else 0
        cols = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]
        for r, g, b in cols:
            ret = gamepad.SDL_GameControllerSetLED(pad, r, g, b)
            assert ret == expected

@pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
def test_SDL_GameControllerHasRumble(gamepads):
    # If we ever add an interactive test suite, this should be moved there
    for pad in gamepads:
        has_rumble = gamepad.SDL_GameControllerHasRumble(pad)
        assert has_rumble in [SDL_FALSE, SDL_TRUE]

@pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
def test_SDL_GameControllerHasRumbleTriggers(gamepads):
    # If we ever add an interactive test suite, this should be moved there
    for pad in gamepads:
        has_rumble_triggers = gamepad.SDL_GameControllerHasRumbleTriggers(pad)
        assert has_rumble_triggers in [SDL_FALSE, SDL_TRUE]

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2016, reason="not available")
def test_SDL_GameControllerSendEffect(gamepads):
    # Probably impossible to test since effect data would be specific
    # to each controller type?
    pass

@pytest.mark.skip("Only relevant on iOS, not testable by PySDL2")
@pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
def test_SDL_GameControllerGetAppleSFSymbolsNameForButtonAxis(gamepads):
    # The following two functions are deliberatly ignored:
    # - SDL_GameControllerGetAppleSFSymbolsNameForButton
    # - SDL_GameControllerGetAppleSFSymbolsNameForAxis
    pass
