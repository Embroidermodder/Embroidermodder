import sys
import pytest
from ctypes import c_char_p, c_void_p, cast
import sdl2
from sdl2 import SDL_Init, SDL_Quit, SDL_QuitSubSystem, SDL_INIT_EVERYTHING
from sdl2 import events

# NOTE: These tests are very incomplete and in need of a rewrite


# Test initializing event structs (is this actually useful?)

def test_SDL_AudioDeviceEvent():
    event = events.SDL_AudioDeviceEvent()
    assert isinstance(event, events.SDL_AudioDeviceEvent)

def test_SDL_DisplayEvent():
    event = events.SDL_DisplayEvent()
    assert isinstance(event, events.SDL_DisplayEvent)

def test_SDL_WindowEvent():
    event = events.SDL_WindowEvent()
    assert isinstance(event, events.SDL_WindowEvent)

def test_SDL_KeyboardEvent():
    event = events.SDL_KeyboardEvent()
    assert isinstance(event, events.SDL_KeyboardEvent)

def test_SDL_TextEditingEvent():
    event = events.SDL_TextEditingEvent()
    assert isinstance(event, events.SDL_TextEditingEvent)

def test_SDL_TextInputEvent():
    event = events.SDL_TextInputEvent()
    assert isinstance(event, events.SDL_TextInputEvent)

def test_SDL_MouseMotionEvent():
    event = events.SDL_MouseMotionEvent()
    assert isinstance(event, events.SDL_MouseMotionEvent)

def test_SDL_MouseButtonEvent():
    event = events.SDL_MouseButtonEvent()
    assert isinstance(event, events.SDL_MouseButtonEvent)

def test_SDL_MouseWheelEvent():
    event = events.SDL_MouseWheelEvent()
    assert isinstance(event, events.SDL_MouseWheelEvent)

def test_SDL_JoyAxisEvent():
    event = events.SDL_JoyAxisEvent()
    assert isinstance(event, events.SDL_JoyAxisEvent)

def test_SDL_JoyBallEvent():
    event = events.SDL_JoyBallEvent()
    assert isinstance(event, events.SDL_JoyBallEvent)

def test_SDL_JoyHatEvent():
    event = events.SDL_JoyHatEvent()
    assert isinstance(event, events.SDL_JoyHatEvent)

def test_SDL_JoyButtonEvent():
    event = events.SDL_JoyButtonEvent()
    assert isinstance(event, events.SDL_JoyButtonEvent)

def test_SDL_JoyDeviceEvent():
    event = events.SDL_JoyDeviceEvent()
    assert isinstance(event, events.SDL_JoyDeviceEvent)

def test_SDL_ControllerAxisEvent():
    event = events.SDL_ControllerAxisEvent()
    assert isinstance(event, events.SDL_ControllerAxisEvent)

def test_SDL_ControllerButtonEvent():
    event = events.SDL_ControllerButtonEvent()
    assert isinstance(event, events.SDL_ControllerButtonEvent)

def test_SDL_ControllerDeviceEvent():
    event = events.SDL_ControllerDeviceEvent()
    assert isinstance(event, events.SDL_ControllerDeviceEvent)

def test_SDL_ControllerTouchpadEvent():
    event = events.SDL_ControllerTouchpadEvent()
    assert isinstance(event, events.SDL_ControllerTouchpadEvent)

def test_SDL_ControllerSensorEvent():
    event = events.SDL_ControllerSensorEvent()
    assert isinstance(event, events.SDL_ControllerSensorEvent)

def test_SDL_TouchFingerEvent():
    event = events.SDL_TouchFingerEvent()
    assert isinstance(event, events.SDL_TouchFingerEvent)

def test_SDL_MultiGestureEvent():
    event = events.SDL_MultiGestureEvent()
    assert isinstance(event, events.SDL_MultiGestureEvent)

def test_SDL_DollarGestureEvent():
    event = events.SDL_DollarGestureEvent()
    assert isinstance(event, events.SDL_DollarGestureEvent)

def test_SDL_DropEvent():
    event = events.SDL_DropEvent()
    assert isinstance(event, events.SDL_DropEvent)

def test_SDL_SensorEvent():
    event = events.SDL_SensorEvent()
    assert isinstance(event, events.SDL_SensorEvent)

def test_SDL_QuitEvent():
    event = events.SDL_QuitEvent()
    assert isinstance(event, events.SDL_QuitEvent)

def test_SDL_UserEvent():
    event = events.SDL_UserEvent()
    assert isinstance(event, events.SDL_UserEvent)

def test_SDL_SysWMEvent():
    event = events.SDL_SysWMEvent()
    assert isinstance(event, events.SDL_SysWMEvent)

def test_SDL_Event():
    event = events.SDL_Event()
    assert isinstance(event, events.SDL_Event)


class TestSDLEvents(object):
    __tags__ = ["sdl"]

    @classmethod
    def setup_class(cls):
        SDL_Init(SDL_INIT_EVERYTHING)

    @classmethod
    def teardown_class(cls):
        SDL_QuitSubSystem(SDL_INIT_EVERYTHING)
        SDL_Quit()

    def test_SDL_AddDelEventWatch(self):
        eventwatch = []

        def watch(data, event):
            eventwatch.append((event.contents, data,))
            return 0
        efilter = events.SDL_EventFilter(watch)
        udata = c_char_p(b"Something random")
        events.SDL_AddEventWatch(efilter, cast(udata, c_void_p))
        ev = events.SDL_Event()
        ev.type = events.SDL_USEREVENT
        ev.user = events.SDL_UserEvent()
        events.SDL_PushEvent(ev)
        assert len(eventwatch) == 1
        # TODO: x
        # self.assertEqual(eventwatch[0][1], udata)

        events.SDL_DelEventWatch(efilter, udata)
        ev = events.SDL_Event()
        events.SDL_PushEvent(ev)
        assert len(eventwatch) == 1
        # TODO: x
        # self.assertEqual(eventwatch[0][1], udata)

    @pytest.mark.skip("not implemented")
    def test_SDL_EventState(self):
        pass
        # state = events.SDL_EventState(events.SDL_USEREVENT, events.SDL_QUERY)
        # self.assertEqual(state, events.SDL_ENABLE)
        # state = events.SDL_EventState(events.SDL_USEREVENT,events.SDL_IGNORE)
        # self.assertEqual(state, events.SDL_ENABLE)
        # state = events.SDL_EventState(events.SDL_USEREVENT, events.SDL_QUERY)
        # self.assertEqual(state, events.SDL_IGNORE)
        # state = events.SDL_EventState(events.SDL_USEREVENT,events.SDL_ENABLE)
        # self.assertEqual(state, events.SDL_IGNORE)
        # state = events.SDL_EventState(events.SDL_USEREVENT, events.SDL_QUERY)
        # self.assertEqual(state, events.SDL_ENABLE)

        # self.assertRaises(TypeError, events.SDL_EventState, None, None)

        # ev = events.SDL_Event()
        # ev.type = events.SDL_USEREVENT
        # ev.user = events.SDL_UserEvent()
        # events.SDL_PushEvent(ev)

    @pytest.mark.skip("not implemented")
    def test_SDL_GetEventState(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_FilterEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_FlushEvent(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_FlushEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_GetSetEventFilter(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_HasEvent(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_HasEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_PeepEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_PollEvent(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_PumpEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_PushEvent(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_RegisterEvents(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_WaitEvent(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_WaitEventTimeout(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_QuitRequested(self):
        pass
