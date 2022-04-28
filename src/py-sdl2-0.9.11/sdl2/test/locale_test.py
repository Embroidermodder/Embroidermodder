import pytest
import sdl2
from sdl2 import locale


@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_GetPreferredLocales():
    locales = locale.SDL_GetPreferredLocales()
    assert type(locales) == list
    if len(locales):
        assert all([type(i) == locale.SDL_Locale for i in locales])
        assert len(locales[0].language)
