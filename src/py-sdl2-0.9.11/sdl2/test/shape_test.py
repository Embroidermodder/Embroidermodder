import sys
import pytest
import ctypes
from sdl2 import SDL_Init, SDL_Quit, SDL_QuitSubSystem, SDL_INIT_EVERYTHING
from sdl2 import video, shape, surface


class TestSDLShape(object):
    __tags__ = ["sdl"]

    @classmethod
    def setup_class(cls):
        SDL_Init(SDL_INIT_EVERYTHING)
        if video.SDL_GetCurrentVideoDriver() is None:
            raise pytest.skip('Video subsystem not supported')

    @classmethod
    def teardown_class(cls):
        SDL_QuitSubSystem(SDL_INIT_EVERYTHING)
        SDL_Quit()

    def test_SDL_CreateShapedWindow(self):
        if video.SDL_GetCurrentVideoDriver() == b"dummy":
            pytest.skip("dummy video driver does not support shaped windows")
        flags = (video.SDL_WINDOW_HIDDEN,)
        for flag in flags:
            window = shape.SDL_CreateShapedWindow(b"Test", 10, 10, 10, 10,
                                                  flag)
            assert isinstance(window.contents, video.SDL_Window)
            video.SDL_DestroyWindow(window)

    def test_SDL_IsShapedWindow(self):
        if video.SDL_GetCurrentVideoDriver() == b"dummy":
            pytest.skip("dummy video driver does not support shaped windows")
        flags = (video.SDL_WINDOW_HIDDEN,)
        for flag in flags:
            window = shape.SDL_CreateShapedWindow(b"Test", 10, 10, 10, 10,
                                                  flag)
            assert isinstance(window.contents, video.SDL_Window)
            val = shape.SDL_IsShapedWindow(window)
            assert val
            video.SDL_DestroyWindow(window)

            window = video.SDL_CreateWindow(b"Test", 10, 10, 10, 10, flag)
            assert isinstance(window.contents, video.SDL_Window)
            val = shape.SDL_IsShapedWindow(window)
            assert not val
            video.SDL_DestroyWindow(window)

    def test_SDL_SetWindowShape(self):
        if video.SDL_GetCurrentVideoDriver() == b"dummy":
            pytest.skip("dummy video driver does not support shaped windows")
        sf = surface.SDL_CreateRGBSurface(0, 10, 10, 32,
                                          0xFF000000,
                                          0x00FF0000,
                                          0x0000FF00,
                                          0x000000FF)
        mode = shape.SDL_WindowShapeMode()
        mode.mode = shape.ShapeModeDefault
        mode.parameters = shape.SDL_WindowShapeParams()
        mode.parameters.binarizationCutoff = 1

        flags = (video.SDL_WINDOW_HIDDEN,)
        for flag in flags:
            # TODO: find out, how shaping is supposed to work :-)
            # window = shape.SDL_CreateShapedWindow(b"Test", 10, 10, 10, 10,
            #                                       flag)
            # self.assertIsInstance(window, video.SDL_Window)
            # self.assertTrue(shape.SDL_IsShapedWindow(window))

            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   None, None, None)
            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   window, None, None)
            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   window, sf, None)
            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   "Test", sf, mode)
            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   window, 12345, mode)
            # self.assertRaises(TypeError, shape.SDL_SetWindowShape,
            #                   window, sf, "Test")

            # shape.SDL_SetWindowShape(window, sf, mode)
            # wmode = shape.SDL_GetShapedWindowMode(window)
            # self.assertEqual(wmode.mode, mode.mode)
            # self.assertEqual(wmode.parameters.binarizationCutoff,
            #                  mode.parameters.binarizationCutoff)
            # video.SDL_DestroyWindow(window)

            window = video.SDL_CreateWindow(b"Test", 10, 10, 10, 10, flag)
            assert isinstance(window.contents, video.SDL_Window)
            #self.assertRaises(sdl.SDLError, shape.SDL_SetWindowShape,
            #                  window, sf, mode)
            video.SDL_DestroyWindow(window)
        surface.SDL_FreeSurface(sf)

    def test_SDL_GetShapedWindowMode(self):
        if video.SDL_GetCurrentVideoDriver() == b"dummy":
            pytest.skip("dummy video driver does not support shaped windows")
        flags = (video.SDL_WINDOW_HIDDEN,)
        for flag in flags:
            window = shape.SDL_CreateShapedWindow(b"Test", 10, 10, 10, 10,
                                                  flag)
            assert isinstance(window.contents, video.SDL_Window)
            mode = shape.SDL_WindowShapeMode()
            ret = shape.SDL_GetShapedWindowMode(window, ctypes.byref(mode))
            assert ret == 0
            video.SDL_DestroyWindow(window)
