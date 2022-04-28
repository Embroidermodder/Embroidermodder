import os
import sys
import array
import pytest
from ctypes import c_int, byref, cast, POINTER, c_void_p
import sdl2
from sdl2.ext import CTypesView
from sdl2 import SDL_Init, SDL_Quit, SDL_INIT_EVERYTHING
from sdl2.stdinc import Uint8, Uint32, SDL_TRUE, SDL_FALSE
from sdl2 import blendmode, pixels, rect, surface, rwops, error

to_ctypes = lambda seq, dtype: (dtype * len(seq))(*seq)

alldepths = (1, 4, 8, 12, 15, 16, 24, 32)
indexdepths = (1, 4, 8)
rgbdepths = (8, 12, 15, 16)
rgbadepths = (16, 24, 32)

rgba_pixelations_16x16 = (
    # 32-bit 16x16 RGBA surface
    ([x << 24 for x in range(16 * 16)], 32, 16,
     (0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF),
     pixels.SDL_PIXELFORMAT_RGBA8888),
    # 16-bit 16x16 RGBA surface
    ([x << 8 for x in range(16 * 16)], 16, 16,
     (0xF000, 0x0F00, 0x00F0, 0x000F),
     pixels.SDL_PIXELFORMAT_RGB444),
    )

blitsizes = ((2, 2), (5, 5), (10, 10), (20, 20),
             (2, 4), (5, 3), (8, 12), (27, 9),
             )

blitpositions = (
    rect.SDL_Rect(0, 0),
    rect.SDL_Rect(4, 4),
    rect.SDL_Rect(10, 10),
    rect.SDL_Rect(15, 15),
    rect.SDL_Rect(-2, 1),
    rect.SDL_Rect(3, -4),
    rect.SDL_Rect(0, 3),
    rect.SDL_Rect(4, 0),
    rect.SDL_Rect(12, 6),
    rect.SDL_Rect(13, 22),
    )


# TODO: mostly covers positive tests right now - fix this!
class TestSDLSurface(object):
    __tags__ = ["sdl"]

    @classmethod
    def setup_class(cls):
        cls.testfile = os.path.join(os.path.dirname
                                     (os.path.abspath(__file__)),
                                     "resources", "surfacetest.bmp")
        SDL_Init(SDL_INIT_EVERYTHING)

    @classmethod
    def teardown_class(cls):
        SDL_Quit()

    def test_SDL_Surface(self):
        sf = surface.SDL_Surface()
        assert isinstance(sf, surface.SDL_Surface)
        # TODO: property checks

    def test_SDL_ConvertPixels(self):
        for buf, bpp, pitch, masks, fmt in rgba_pixelations_16x16:
            bytecount = bpp // 8
            arflag = None
            if bpp == 32:
                arflag = "I"
            elif bpp == 16:
                arflag = "H"
            src = CTypesView(array.array(arflag, buf), bytecount)
            srcp = cast(src.to_bytes(), POINTER(Uint8))
            dst = (Uint8 * len(src))()
            dstp = cast(dst, POINTER(Uint8))
            ret = surface.SDL_ConvertPixels(16, 16, fmt, srcp, 16 * bytecount,
                                            fmt, dstp, 16 * bytecount)
            assert ret == 0
            for index, val in enumerate(dst):
                assert val == src.view[index]

    def test_SDL_ConvertSurface(self):
        tenbit = [pixels.SDL_PACKEDLAYOUT_2101010, pixels.SDL_PACKEDLAYOUT_1010102]
        for idx in pixels.ALL_PIXELFORMATS:
            pfmt = pixels.SDL_AllocFormat(idx)
            for fmt in pixels.ALL_PIXELFORMATS:
                idx_name = pixels.SDL_GetPixelFormatName(idx).decode('utf-8') # for debug
                fmt_name = pixels.SDL_GetPixelFormatName(fmt).decode('utf-8') # for debug
                # SDL2 doesn't support converting fancier formats (e.g YUV, 10bit)
                if pixels.SDL_ISPIXELFORMAT_FOURCC(idx) or pixels.SDL_PIXELLAYOUT(idx) in tenbit:
                    continue
                if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt) or pixels.SDL_PIXELLAYOUT(fmt) in tenbit:
                    continue
                # SDL2 doesn't support converting to formats w/ less than 8 bpp
                if pixels.SDL_BITSPERPIXEL(idx) < 8:
                    continue
                # SDL2 doesn't support converting from indexed formats w/ 4 bpp
                if pixels.SDL_PIXELTYPE(fmt) == pixels.SDL_PIXELTYPE_INDEX4:
                    continue
                bpp = c_int()
                rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
                ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                        byref(rmask), byref(gmask),
                                                        byref(bmask), byref(amask))
                assert ret == SDL_TRUE
                sf = surface.SDL_CreateRGBSurface(0, 10, 20, bpp, rmask, gmask,
                                                  bmask, amask)
                assert isinstance(sf.contents, surface.SDL_Surface)
                csf = surface.SDL_ConvertSurface(sf, pfmt, 0)
                assert csf, error.SDL_GetError()
                assert isinstance(csf.contents, surface.SDL_Surface)
                surface.SDL_FreeSurface(sf)
                surface.SDL_FreeSurface(csf)
            pixels.SDL_FreeFormat(pfmt)

        #######################################################################
        # sf = surface.create_rgb_surface(10, 10, 32, 0, 0, 0)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, None, None, None)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, sf, None, None)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, sf, "Test", 0)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, sf, 12345, 0)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, None, pfmt, 0)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, "Test", pfmt, 0)
        # self.assertRaises((AttributeError, TypeError),
        #                   surface.convert_surface, 12345, pfmt, 0)
        # surface.free_surface(sf)
        #######################################################################

    def test_SDL_ConvertSurfaceFormat(self):
        tenbit = [pixels.SDL_PACKEDLAYOUT_2101010, pixels.SDL_PACKEDLAYOUT_1010102]
        for pfmt in pixels.ALL_PIXELFORMATS:
            for fmt in pixels.ALL_PIXELFORMATS:
                # SDL2 doesn't support converting fancier formats (e.g YUV, 10bit)
                if pixels.SDL_ISPIXELFORMAT_FOURCC(pfmt) or pixels.SDL_PIXELLAYOUT(pfmt) in tenbit:
                    continue
                if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt) or pixels.SDL_PIXELLAYOUT(fmt) in tenbit:
                    continue
                # SDL2 doesn't support converting to formats w/ less than 8 bpp
                if pixels.SDL_BITSPERPIXEL(pfmt) < 8:
                    continue
                # SDL2 doesn't support converting from indexed formats w/ 4 bpp
                if pixels.SDL_PIXELTYPE(fmt) == pixels.SDL_PIXELTYPE_INDEX4:
                    continue
                bpp = c_int()
                rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
                ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                        byref(rmask), byref(gmask),
                                                        byref(bmask), byref(amask))
                assert ret == SDL_TRUE
                sf = surface.SDL_CreateRGBSurface(0, 10, 20, bpp, rmask, gmask,
                                                  bmask, amask)
                assert isinstance(sf.contents, surface.SDL_Surface)
                csf = surface.SDL_ConvertSurfaceFormat(sf, pfmt, 0)
                assert csf, error.SDL_GetError()
                assert isinstance(csf.contents, surface.SDL_Surface)
                surface.SDL_FreeSurface(sf)
                surface.SDL_FreeSurface(csf)

    def test_SDL_CreateRGBSurface(self):
        for w in range(1, 100, 5):
            for h in range(1, 100, 5):
                for bpp in alldepths:
                    sf = surface.SDL_CreateRGBSurface(0, w, h, bpp, 0, 0, 0, 0)
                    assert isinstance(sf.contents, surface.SDL_Surface)
                    surface.SDL_FreeSurface(sf)

        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            for w in range(1, 100, 5):
                for h in range(1, 100, 5):
                    ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                            byref(rmask),
                                                            byref(gmask),
                                                            byref(bmask),
                                                            byref(amask))
                    assert ret == SDL_TRUE
                    sf = surface.SDL_CreateRGBSurface(0, 10, 20, bpp, rmask,
                                                      gmask, bmask, amask)
                    assert isinstance(sf.contents, surface.SDL_Surface)
                    surface.SDL_FreeSurface(sf)

        # Broken ones
        # sizes = [(0, 0), (65536, 65536)]
        # for (w, h) in sizes:
        #    for bpp in alldepths:
        #        print w, h, bpp
        #        sf = surface.create_rgb_surface(w, h, bpp)
        #        self.assertIsInstance(sf, surface.SDL_Surface)
        #        surface.free_surface(sf)

        # TODO: raises a SDL Out of memory error, but 65536 works?!
        #
        # sf = surface.create_rgb_surface(65535, 65535, 32)
        # self.assertIsInstance(sf, surface.SDL_Surface)

        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 66)
        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 0)
        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 8,
        #                  0xf0, 0x0f, 0x01, 0x02)
        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 16,
        #                  0xf0, 0x0f, 0x01, 0x02)
        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 24,
        #                  0xf0, 0x0f, 0x01, 0x02)
        #self.assertRaises(sdl.SDLError, surface.create_rgb_surface, 1, 1, 32,
        #                  0xf0, 0x0f, 0x01, 0x02)

    def test_SDL_CreateRGBSurfaceWithFormat(self):
        for w in range(1, 100, 5):
            for h in range(1, 100, 5):
                for bpp in alldepths:
                    for fmt in pixels.ALL_PIXELFORMATS:
                        if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                            continue
                        sf = surface.SDL_CreateRGBSurfaceWithFormat(0, w, h,
                                                                    bpp, fmt)
                        assert isinstance(sf.contents, surface.SDL_Surface)
                        surface.SDL_FreeSurface(sf)

    def test_SDL_CreateRGBSurfaceWithFormatFrom(self):
        for buf, bpp, pitch, masks, fmt in rgba_pixelations_16x16:
            if bpp == 32:
                arflag = "I"
            elif bpp == 16:
                arflag = "H"
            bytebuf = CTypesView(array.array(arflag, buf))
            bufptr = cast(bytebuf.to_bytes(), POINTER(Uint8))
            sf = surface.SDL_CreateRGBSurfaceWithFormatFrom(bufptr, 16, 16,
                    bpp, pitch, fmt)
            assert isinstance(sf.contents, surface.SDL_Surface)
            surface.SDL_FreeSurface(sf)

    def test_SDL_CreateRGBSurfaceFrom(self):
        for buf, bpp, pitch, masks, fmt in rgba_pixelations_16x16:
            if bpp == 32:
                arflag = "I"
            elif bpp == 16:
                arflag = "H"
            bytebuf = CTypesView(array.array(arflag, buf))
            bufptr = cast(bytebuf.to_bytes(), POINTER(Uint8))
            sf = surface.SDL_CreateRGBSurfaceFrom(bufptr, 16, 16, bpp,
                                                  pitch, masks[0], masks[1],
                                                  masks[2], masks[3])
            assert isinstance(sf.contents, surface.SDL_Surface)
            surface.SDL_FreeSurface(sf)

    @pytest.mark.skipif(sdl2.dll.version < 2018, reason="not available")
    def test_SDL_PremultiplyAlpha(self):
        # TODO: Add a test for this after the ext updates, when we have
        # _create_surface to help
        pass

    def test_SDL_FillRect(self):
        rectlist = (rect.SDL_Rect(0, 0, 0, 0),
                    rect.SDL_Rect(0, 0, 10, 10),
                    rect.SDL_Rect(0, 0, -10, 10),
                    rect.SDL_Rect(0, 0, -10, -10),
                    rect.SDL_Rect(-10, -10, 10, 10),
                    rect.SDL_Rect(10, -10, 10, 10),
                    rect.SDL_Rect(10, 10, 10, 10),
                    )

        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            if pixels.SDL_BITSPERPIXEL(fmt) < 8:
                continue  # Skip < 8bpp, SDL_FillRect does not work on those

            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            for w in range(1, 100, 5):
                for h in range(1, 100, 5):
                    sf = surface.SDL_CreateRGBSurface(0, w, h, bpp, rmask,
                                                      gmask, bmask, amask)
                    for r in rectlist:
                        # TODO: check for changed pixels
                        ret = surface.SDL_FillRect(sf, r, 0xff00ff00)
                        assert ret == 0
                    surface.SDL_FreeSurface(sf)

    def test_SDL_FillRects(self):
        rectlist = to_ctypes([rect.SDL_Rect(0, 0, 0, 0),
                              rect.SDL_Rect(0, 0, 10, 10),
                              rect.SDL_Rect(0, 0, -10, 10),
                              rect.SDL_Rect(0, 0, -10, -10),
                              rect.SDL_Rect(-10, -10, 10, 10),
                              rect.SDL_Rect(10, -10, 10, 10),
                              rect.SDL_Rect(10, 10, 10, 10)], rect.SDL_Rect)

        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            if pixels.SDL_BITSPERPIXEL(fmt) < 8:
                continue  # Skip < 8bpp, SDL_FillRect does not work on those
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            for w in range(1, 100, 5):
                for h in range(1, 100, 5):
                    sf = surface.SDL_CreateRGBSurface(0, w, h, bpp, rmask,
                                                      gmask, bmask, amask)
                    # TODO: check for changed pixels
                    surface.SDL_FillRects(sf, rectlist, 7, 0xff00ff00)
                    surface.SDL_FreeSurface(sf)

    def test_SDL_FreeSurface(self):
        formats = (pixels.SDL_PIXELFORMAT_INDEX1LSB,
                   pixels.SDL_PIXELFORMAT_RGB332,
                   pixels.SDL_PIXELFORMAT_RGBA4444,
                   pixels.SDL_PIXELFORMAT_BGR888,
                   pixels.SDL_PIXELFORMAT_ARGB8888,
                   pixels.SDL_PIXELFORMAT_ARGB2101010
                   )
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in formats:
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 5, 5, bpp, rmask, gmask,
                                              bmask, amask)
            surface.SDL_FreeSurface(sf)

    def test_SDL_GetSetClipRect(self):
        rectlist = ((rect.SDL_Rect(0, 0, 0, 0), SDL_FALSE, True),
                    (rect.SDL_Rect(2, 2, 0, 0), SDL_FALSE, False),
                    (rect.SDL_Rect(2, 2, 5, 1), SDL_TRUE, True),
                    (rect.SDL_Rect(6, 5, 10, 3), SDL_TRUE, False),
                    (rect.SDL_Rect(0, 0, 10, 10), SDL_TRUE, True),
                    (rect.SDL_Rect(0, 0, -10, 10), SDL_FALSE, False),
                    (rect.SDL_Rect(0, 0, -10, -10), SDL_FALSE, False),
                    (rect.SDL_Rect(-10, -10, 10, 10), SDL_FALSE, False),
                    (rect.SDL_Rect(10, -10, 10, 10), SDL_FALSE, False),
                    (rect.SDL_Rect(10, 10, 10, 10), SDL_TRUE, False)
                    )

        clip = rect.SDL_Rect()
        sf = surface.SDL_CreateRGBSurface(0, 15, 15, 32, 0, 0, 0, 0)
        assert isinstance(sf.contents, surface.SDL_Surface)
        surface.SDL_GetClipRect(sf, byref(clip))
        assert clip == rect.SDL_Rect(0, 0, 15, 15)

        for r, clipsetval, cmpval in rectlist:
            retval = surface.SDL_SetClipRect(sf, r)
            surface.SDL_GetClipRect(sf, byref(clip))
            err = "Could not set clip rect %s" % r
            assert retval == clipsetval, "retval: " + err
            assert (clip == r) == cmpval, "clip: " + err
        surface.SDL_FreeSurface(sf)

    @pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
    def test_SDL_HasColorKey(self):
        sf = surface.SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0)
        assert surface.SDL_HasColorKey(sf) == SDL_FALSE
        key = pixels.SDL_MapRGB(sf.contents.format, 255, 255, 255)
        surface.SDL_SetColorKey(sf, 1, key)
        assert surface.SDL_HasColorKey(sf) == SDL_TRUE
        surface.SDL_FreeSurface(sf)

    def test_SDL_GetSetColorKey(self):
        colorkeys = ((0, 0, 0),
                     (32, 64, 128),
                     (10, 20, 30),
                     (1, 2, 4),
                     (255, 255, 255),
                     (128, 128, 128),
                     (127, 127, 127),
                     )
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            pformat = pixels.SDL_AllocFormat(fmt)
            assert isinstance(pformat.contents, pixels.SDL_PixelFormat)
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 10, 10, bpp, rmask, gmask,
                                              bmask, amask)
            for r, g, b in colorkeys:
                key = pixels.SDL_MapRGB(pformat, r, g, b)
                surface.SDL_SetColorKey(sf, 1, key)
                skey = Uint32()
                ret = surface.SDL_GetColorKey(sf, byref(skey))
                errmsg = "Could not set color key (%d, %d, %d)" % (r, g, b)
                assert ret == 0
                assert skey.value == key, errmsg
                    
            pixels.SDL_FreeFormat(pformat)
            surface.SDL_FreeSurface(sf)

    def test_SDL_GetSetSurfaceAlphaMod(self):
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 10, 10, bpp, rmask, gmask,
                                              bmask, amask)
            salpha = Uint8()
            ret = surface.SDL_GetSurfaceAlphaMod(sf, byref(salpha))
            assert ret == 0
            assert salpha.value == 255
            for alpha in range(0, 255):
                ret = surface.SDL_SetSurfaceAlphaMod(sf, alpha)
                assert ret == 0
                ret = surface.SDL_GetSurfaceAlphaMod(sf, byref(salpha))
                assert ret == 0
                assert salpha.value == alpha
            surface.SDL_FreeSurface(sf)

    def test_SDL_GetSetSurfaceBlendMode(self):
        modes = (blendmode.SDL_BLENDMODE_NONE,
                 blendmode.SDL_BLENDMODE_BLEND,
                 blendmode.SDL_BLENDMODE_ADD,
                 blendmode.SDL_BLENDMODE_MOD
                 )
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 10, 10, bpp, rmask, gmask,
                                              bmask, amask)
            for mode in modes:
                smode = blendmode.SDL_BlendMode()
                ret = surface.SDL_SetSurfaceBlendMode(sf, mode)
                assert ret == 0
                surface.SDL_GetSurfaceBlendMode(sf, byref(smode))
                assert smode.value == mode
            surface.SDL_FreeSurface(sf)

    def test_SDL_GetSetSurfaceColorMod(self):
        colormods = ((0, 0, 0),
                     (32, 64, 128),
                     (10, 20, 30),
                     (1, 2, 4),
                     (255, 255, 255),
                     (128, 128, 128),
                     (127, 127, 127),
                     )
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 10, 10, bpp, rmask, gmask,
                                              bmask, amask)
            for r, g, b in colormods:
                sr, sg, sb = Uint8(), Uint8(), Uint8()
                surface.SDL_SetSurfaceColorMod(sf, r, g, b)
                ret = surface.SDL_GetSurfaceColorMod(sf, byref(sr), byref(sg),
                                                     byref(sb))
                assert ret == 0
                assert (sr.value, sg.value, sb.value) == (r, g, b)
            surface.SDL_FreeSurface(sf)

    def test_SDL_LockUnlockMUSTLOCKSurface(self):
        bpp = c_int()
        rmask, gmask, bmask, amask = Uint32(), Uint32(), Uint32(), Uint32()
        for fmt in pixels.ALL_PIXELFORMATS:
            if pixels.SDL_ISPIXELFORMAT_FOURCC(fmt):
                continue
            ret = pixels.SDL_PixelFormatEnumToMasks(fmt, byref(bpp),
                                                    byref(rmask), byref(gmask),
                                                    byref(bmask), byref(amask))
            assert ret == SDL_TRUE
            sf = surface.SDL_CreateRGBSurface(0, 10, 10, bpp, rmask, gmask,
                                              bmask, amask)
            # TODO: locking seems to be only necessary for RLE surfaces...
            if surface.SDL_MUSTLOCK(sf.contents):
                assert not sf.contents.locked
                surface.SDL_LockSurface(sf)
                assert sf.contents.locked
                surface.SDL_LockSurface(sf)
                assert sf.contents.locked
                surface.SDL_LockSurface(sf)
                assert sf.contents.locked
                surface.SDL_LockSurface(sf)
                assert sf.contents.locked
                surface.SDL_LockSurface(sf)
                assert sf.contents.locked
                surface.SDL_UnlockSurface(sf)
                assert not sf.contents.locked
            surface.SDL_FreeSurface(sf)

    def test_SDL_LowerBlit(self):
        bpp = 32
        w, h = 10, 10
        # no alpha to prevent blending
        masks = (0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000)
        dest = surface.SDL_CreateRGBSurface(0, w, h, bpp, masks[0], masks[1],
                                            masks[2], masks[3])
        pixelsize = h * dest.contents.pitch
        rowlen = dest.contents.pitch // 4

        sources = []
        for width, height in blitsizes:
            src = surface.SDL_CreateRGBSurface(0, width, height, bpp, masks[0],
                                               masks[1], masks[2], masks[3])
            surface.SDL_FillRect(src, None, 0xFFFFFFFF)  # fill with white
            sources.append(src)

        for src in sources:
            for pos in blitpositions:
                drect = pos.__copy__()
                surface.SDL_FillRect(dest, None, 0x0)  # fill with black
                surface.SDL_LowerBlit(src, src.contents.clip_rect, dest, drect)
                buf = cast(dest.contents.pixels, POINTER(Uint8 * pixelsize))
                pbuf = CTypesView(buf.contents, itemsize=1, objsize=pixelsize)
                iview = pbuf.to_uint32()
                pw = drect.x + drect.w
                ph = drect.y + drect.h
                e = "color mismatch at %d,%d for %s: %d != %d"
                for y in range(dest.contents.h):
                    for x in range(dest.contents.w):
                        col = iview[y * rowlen + x]
                        if y >= drect.y and y < ph and x >= drect.x and x < pw:
                            assert col == 0xFFFFFFFF, \
                                   e % (y, x, pos, col, 0xFFFFFFFF)
                        else:
                            assert col == 0x0, e % (y, x, pos, col, 0x0)

        while len(sources) > 0:
            sf = sources.pop()
            surface.SDL_FreeSurface(sf)
        surface.SDL_FreeSurface(dest)

    @pytest.mark.skip("not implemented")
    def test_SDL_LowerBlitScaled(self):
        pass

    def test_SDL_UpperBlit(self):
        bpp = 32
        w, h = 10, 10
        # no alpha to prevent blending
        masks = (0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000)
        dest = surface.SDL_CreateRGBSurface(0, w, h, bpp, masks[0], masks[1],
                                            masks[2], masks[3])
        pixelsize = h * dest.contents.pitch
        rowlen = dest.contents.pitch // 4

        sources = []
        for width, height in blitsizes:
            src = surface.SDL_CreateRGBSurface(0, width, height, bpp, masks[0],
                                               masks[1], masks[2], masks[3])
            surface.SDL_FillRect(src, None, 0xFFFFFFFF)  # fill with white
            sources.append(src)

        for src in sources:
            for pos in blitpositions:
                drect = pos.__copy__()
                surface.SDL_FillRect(dest, None, 0x0)  # fill with black
                surface.SDL_UpperBlit(src, None, dest, drect)
                buf = cast(dest.contents.pixels, POINTER(Uint8 * pixelsize))
                pbuf = CTypesView(buf.contents, itemsize=1, objsize=pixelsize)
                iview = pbuf.to_uint32()
                pw = drect.x + drect.w
                ph = drect.y + drect.h
                e = "color mismatch at %d,%d for %s: %d != %d"
                for y in range(dest.contents.h):
                    for x in range(dest.contents.w):
                        col = iview[y * rowlen + x]
                        if y >= drect.y and y < ph and x >= drect.x and x < pw:
                            assert col == 0xFFFFFFFF, \
                                   e % (x, y, pos, col, 0xFFFFFFFF)
                        else:
                            assert col == 0x0, e % (x, y, pos, col, 0x0)

        while len(sources) > 0:
            sf = sources.pop()
            surface.SDL_FreeSurface(sf)
        surface.SDL_FreeSurface(dest)

    def test_SDL_BlitSurface(self):
        bpp = 32
        w, h = 10, 10
        # no alpha to prevent blending
        masks = (0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000)
        dest = surface.SDL_CreateRGBSurface(0, w, h, bpp, masks[0], masks[1],
                                            masks[2], masks[3])
        pixelsize = h * dest.contents.pitch
        rowlen = dest.contents.pitch // 4

        sources = []
        for width, height in blitsizes:
            src = surface.SDL_CreateRGBSurface(0, width, height, bpp, masks[0],
                                               masks[1], masks[2], masks[3])
            surface.SDL_FillRect(src, None, 0xFFFFFFFF)  # fill with white
            sources.append(src)

        for src in sources:
            for pos in blitpositions:
                drect = pos.__copy__()
                surface.SDL_FillRect(dest, None, 0x0)  # fill with black
                surface.SDL_BlitSurface(src, None, dest, drect)
                buf = cast(dest.contents.pixels, POINTER(Uint8 * pixelsize))
                pbuf = CTypesView(buf.contents, itemsize=1, objsize=pixelsize)
                iview = pbuf.to_uint32()
                pw = drect.x + drect.w
                ph = drect.y + drect.h
                e = "color mismatch at %d,%d for %s: %d != %d"
                for y in range(dest.contents.h):
                    for x in range(dest.contents.w):
                        col = iview[y * rowlen + x]
                        if y >= drect.y and y < ph and x >= drect.x and x < pw:
                            assert col == 0xFFFFFFFF, \
                                   e % (y, x, pos, col, 0xFFFFFFFF)
                        else:
                            assert col == 0x0, e % (y, x, pos, col, 0x0)

        while len(sources) > 0:
            sf = sources.pop()
            surface.SDL_FreeSurface(sf)
        surface.SDL_FreeSurface(dest)

    @pytest.mark.skip("not implemented")
    def test_SDL_UpperBlitScaled(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_SoftStretch(self):
        pass

    @pytest.mark.skip("not implemented")
    @pytest.mark.skipif(sdl2.dll.version < 2016, reason="not available")
    def test_SDL_SoftStretchLinear(self):
        pass

    def test_SDL_SetSurfacePalette(self):
        invpalette = pixels.SDL_AllocPalette(10)
        palette = pixels.SDL_AllocPalette(1 << 16)
        sf = surface.SDL_CreateRGBSurface(0, 10, 10, 16, 0, 0, 0, 0)

        assert not sf.contents.format.contents.palette
        surface.SDL_SetSurfacePalette(sf, palette)
        assert sf.contents.format.contents.palette.contents is not None
        assert sf.contents.format.contents.palette.contents is not None
        surface.SDL_SetSurfacePalette(sf, None)
        assert not sf.contents.format.contents.palette

        surface.SDL_FreeSurface(sf)
        pixels.SDL_FreePalette(invpalette)
        pixels.SDL_FreePalette(palette)

    def test_SDL_SetSurfaceRLE(self):
        masks = (0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000)
        tst = surface.SDL_CreateRGBSurface(
            0, 16, 16, 32, masks[0], masks[1], masks[2], masks[3]
        )
        assert isinstance(tst.contents, surface.SDL_Surface)
        # Test for success when setting RLE after locking
        surface.SDL_LockSurface(tst)
        ret = surface.SDL_SetSurfaceRLE(tst, 1)
        assert ret == 0
        surface.SDL_FreeSurface(tst)

    @pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
    def test_SDL_HasSurfaceRLE(self):
        masks = (0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000)
        tst = surface.SDL_CreateRGBSurface(
            0, 16, 16, 32, masks[0], masks[1], masks[2], masks[3]
        )
        assert isinstance(tst.contents, surface.SDL_Surface)
        # Test for success before and after setting RLE
        assert surface.SDL_HasSurfaceRLE(tst) == SDL_FALSE
        surface.SDL_LockSurface(tst)
        ret = surface.SDL_SetSurfaceRLE(tst, 1)
        assert ret == 0
        assert surface.SDL_HasSurfaceRLE(tst) == SDL_TRUE
        surface.SDL_FreeSurface(tst)

    def test_SDL_LoadBMP_RW(self):
        fp = open(self.testfile, "rb")
        imgrw = rwops.rw_from_object(fp)
        imgsurface = surface.SDL_LoadBMP_RW(imgrw, 0)
        assert isinstance(imgsurface.contents, surface.SDL_Surface)
        surface.SDL_FreeSurface(imgsurface)
        imgrw.close(imgrw)

    def test_SDL_LoadBMP(self):
        imgsurface = surface.SDL_LoadBMP(self.testfile.encode("utf-8"))
        assert isinstance(imgsurface.contents, surface.SDL_Surface)
        surface.SDL_FreeSurface(imgsurface)

    @pytest.mark.skip("not implemented")
    def test_SDL_SaveBMP_RW(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_SaveBMP(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_GetSetYUVConversionMode(self):
        pass

    @pytest.mark.skip("not implemented")
    def test_SDL_GetYUVConversionModeForResolution(self):
        pass
