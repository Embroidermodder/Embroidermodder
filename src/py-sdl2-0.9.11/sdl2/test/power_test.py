import sys
import os
import pytest
from ctypes import c_int, byref
from sdl2 import power


def test_SDL_GetPowerInfo():
    has_battery = [
        power.SDL_POWERSTATE_ON_BATTERY,
        power.SDL_POWERSTATE_CHARGING,
        power.SDL_POWERSTATE_CHARGED
    ]
    remaining, pct = c_int(), c_int()
    state = power.SDL_GetPowerInfo(byref(remaining), byref(pct))
    if state in has_battery:
        assert pct.value <= 100
        assert pct.value > 0
    else:
        assert remaining.value == -1
        assert pct.value == -1
