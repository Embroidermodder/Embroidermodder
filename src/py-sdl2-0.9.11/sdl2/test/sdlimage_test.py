import os
import sys
import ctypes
import pytest

import sdl2
from sdl2 import SDL_Init, SDL_Quit, version, surface, rwops, render

sdlimage = pytest.importorskip("sdl2.sdlimage")


is32bit = sys.maxsize <= 2**32
ismacos = sys.platform == "darwin"
iswindows = "win" in sys.platform
isconda = os.getenv("CONDA_PREFIX") != None

formats = ["bmp",
           "cur",
           "gif",
           "ico",
           "jpg",
           "lbm",
           "pbm",
           "pcx",
           "pgm",
           "png",
           "pnm",
           "ppm",
           "svg",
           "tga",
           "tif",
           "webp",
           "xcf",
           "xpm",
           #"xv",
           ]

# SVG unsupported on SDL2_image < 2.0.2 as well as in Conda's current (2.0.5)
# Windows binaries
if sdlimage.dll.version < 2002 or (isconda and iswindows):
    formats.remove("svg")

# As of SDL2_image 2.0.5, XCF support seems to be broken on 32-bit builds
# XCF support is also broken in official SDL2_image macOS .frameworks and
# Conda's SDL2_image Windows binaries
bad_xcf = False
if is32bit or ismacos or (isconda and iswindows):
    formats.remove("xcf")
    bad_xcf = True

# WEBP support seems to be broken in the 32-bit Windows SDL2_image 2.0.2 binary
bad_webp = is32bit and sdlimage.dll.version == 2002
if bad_webp:
    formats.remove("webp")

# JPG saving requires SDL2_image >= 2.0.2 and isn't in official mac binaries
no_jpeg_save = sdlimage.dll.version < 2002 or ismacos

@pytest.fixture(scope="module")
def with_sdl_image(with_sdl):
    flags = (
        sdlimage.IMG_INIT_JPG | sdlimage.IMG_INIT_PNG |
        sdlimage.IMG_INIT_TIF  | sdlimage.IMG_INIT_WEBP
    )
    sdlimage.IMG_Init(flags)
    yield
    sdlimage.IMG_Quit()

@pytest.fixture
def sw_renderer(with_sdl_image):
    sf = surface.SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0)
    rd = render.SDL_CreateSoftwareRenderer(sf)
    yield rd
    render.SDL_DestroyRenderer(rd)
    surface.SDL_FreeSurface(sf)

def _get_image_path(fmt):
    fname = "surfacetest.{0}".format(fmt)
    testdir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(testdir, "resources", fname)

def _verify_img_load(surf):
    if not surf:
        assert sdlimage.IMG_GetError() == b""
        assert surf
    assert isinstance(surf.contents, surface.SDL_Surface)


def test_IMG_Linked_Version():
    v = sdlimage.IMG_Linked_Version()
    assert isinstance(v.contents, version.SDL_version)
    assert v.contents.major == 2
    assert v.contents.minor == 0
    assert v.contents.patch >= 1

def test_IMG_Init():
    SDL_Init(0)
    supported = []
    libs = {
        'JPEG': sdlimage.IMG_INIT_JPG,
        'PNG': sdlimage.IMG_INIT_PNG,
        'TIFF': sdlimage.IMG_INIT_TIF,
        'WEBP': sdlimage.IMG_INIT_WEBP
    }
    for lib in libs.keys():
        flags = libs[lib]
        ret = sdlimage.IMG_Init(flags)
        err = sdlimage.IMG_GetError()
        if err:
            err = err.decode('utf-8')
            print("Error loading {0} library: {1}".format(lib, err))
            sdl2.SDL_ClearError()
        if ret & flags == flags:
            supported.append(lib)
        sdlimage.IMG_Quit()
    print("Supported image libraries:")
    print(supported)
    assert len(supported) == len(libs.keys())

def test_IMG_Load(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        sf = sdlimage.IMG_Load(fpath.encode("utf-8"))
        _verify_img_load(sf)
        surface.SDL_FreeSurface(sf)

def test_IMG_Load_RW(with_sdl_image):
    skip = ['tga'] # TGA broken for Load_RW
    for fmt in formats:
        if fmt in skip:
            continue
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            sf = sdlimage.IMG_Load_RW(rwops.rw_from_object(fp), False)
        _verify_img_load(sf)
        surface.SDL_FreeSurface(sf)

def test_IMG_LoadTexture(sw_renderer):
    rd = sw_renderer
    skip = []
    for fmt in formats:
        if fmt in skip:
            continue
        fpath = _get_image_path(fmt)
        tex = sdlimage.IMG_LoadTexture(rd, fpath.encode("utf-8"))
        if not tex:
            assert sdlimage.IMG_GetError() == b""
            assert tex
        assert isinstance(tex.contents, render.SDL_Texture)
        render.SDL_DestroyTexture(tex)

def test_IMG_LoadTexture_RW(sw_renderer):
    rd = sw_renderer
    skip = ['svg', 'tga'] # TGA & SVG broken for LoadTexture_RW
    for fmt in formats:
        if fmt in skip:
            continue
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            tex = sdlimage.IMG_LoadTexture_RW(rd, rwops.rw_from_object(fp), 0)
            if not tex:
                assert sdlimage.IMG_GetError() == b""
                assert tex
            assert isinstance(tex.contents, render.SDL_Texture)
            render.SDL_DestroyTexture(tex)

def test_IMG_LoadTextureTyped_RW(sw_renderer):
    rd = sw_renderer
    skip = ['svg'] # SVG broken for LoadTextureTyped_RW
    for fmt in formats:
        if fmt in skip:
            continue
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            rw = rwops.rw_from_object(fp)
            fmtx = fmt.upper().encode("utf-8")
            tex = sdlimage.IMG_LoadTextureTyped_RW(rd, rw, 0, fmtx)
            if not tex:
                assert sdlimage.IMG_GetError() == b""
                assert tex
            assert isinstance(tex.contents, render.SDL_Texture)
        render.SDL_DestroyTexture(tex)

def test_IMG_LoadTyped_RW(with_sdl_image):
    skip = []
    for fmt in formats:
        if fmt in skip:
            continue
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            sf = sdlimage.IMG_LoadTyped_RW(
                rwops.rw_from_object(fp), False, fmt.upper().encode("utf-8")
            )
            _verify_img_load(sf)
            surface.SDL_FreeSurface(sf)

def test_IMG_LoadBMP_RW(with_sdl_image):
    fp = open(_get_image_path("bmp"), "rb")
    sf = sdlimage.IMG_LoadBMP_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadCUR_RW(with_sdl_image):
    fp = open(_get_image_path("cur"), "rb")
    sf = sdlimage.IMG_LoadCUR_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadGIF_RW(with_sdl_image):
    fp = open(_get_image_path("gif"), "rb")
    sf = sdlimage.IMG_LoadGIF_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadICO_RW(with_sdl_image):
    fp = open(_get_image_path("ico"), "rb")
    sf = sdlimage.IMG_LoadICO_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadJPG_RW(with_sdl_image):
    fp = open(_get_image_path("jpg"), "rb")
    sf = sdlimage.IMG_LoadJPG_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadLBM_RW(with_sdl_image):
    fp = open(_get_image_path("lbm"), "rb")
    sf = sdlimage.IMG_LoadLBM_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadPCX_RW(with_sdl_image):
    fp = open(_get_image_path("pcx"), "rb")
    sf = sdlimage.IMG_LoadPCX_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadPNG_RW(with_sdl_image):
    fp = open(_get_image_path("png"), "rb")
    sf = sdlimage.IMG_LoadPNG_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadPNM_RW(with_sdl_image):
    fp = open(_get_image_path("pnm"), "rb")
    sf = sdlimage.IMG_LoadPNM_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

@pytest.mark.skipif(sdlimage.dll.version < 2002, reason="Added in 2.0.2")
@pytest.mark.xfail(isconda and iswindows, reason="Broken w/ win64 Conda")
def test_IMG_LoadSVG_RW(with_sdl_image):
    fp = open(_get_image_path("svg"), "rb")
    sf = sdlimage.IMG_LoadSVG_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadTGA_RW(with_sdl_image):
    fp = open(_get_image_path("tga"), "rb")
    sf = sdlimage.IMG_LoadTGA_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadTIF_RW(with_sdl_image):
    fp = open(_get_image_path("tif"), "rb")
    sf = sdlimage.IMG_LoadTIF_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

@pytest.mark.xfail(bad_webp, reason="WEBP broken in 2.0.2 binaries for 32-bit Windows")
def test_IMG_LoadWEBP_RW(with_sdl_image):
    fp = open(_get_image_path("webp"), "rb")
    sf = sdlimage.IMG_LoadWEBP_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

@pytest.mark.xfail(bad_xcf, reason="XCF currently broken on some platforms")
def test_IMG_LoadXCF_RW(with_sdl_image):
    fp = open(_get_image_path("xcf"), "rb")
    sf = sdlimage.IMG_LoadXCF_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_LoadXPM_RW(with_sdl_image):
    fp = open(_get_image_path("xpm"), "rb")
    sf = sdlimage.IMG_LoadXPM_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

@pytest.mark.skip("not implemented")
def test_IMG_LoadXV_RW(with_sdl_image):
    fp = open(_get_image_path("xv"), "rb")
    sf = sdlimage.IMG_LoadXV_RW(rwops.rw_from_object(fp))
    fp.close()
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_isBMP(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "bmp":
                assert sdlimage.IMG_isBMP(imgrw)
            else:
                assert not sdlimage.IMG_isBMP(imgrw)

def test_IMG_isCUR(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "cur":
                assert sdlimage.IMG_isCUR(imgrw)
            else:
                assert not sdlimage.IMG_isCUR(imgrw)

def test_IMG_isGIF(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "gif":
                assert sdlimage.IMG_isGIF(imgrw)
            else:
                assert not sdlimage.IMG_isGIF(imgrw)

def test_IMG_isICO(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "ico":
                assert sdlimage.IMG_isICO(imgrw)
            else:
                assert not sdlimage.IMG_isICO(imgrw)

def test_IMG_isJPG(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "jpg":
                assert sdlimage.IMG_isJPG(imgrw)
            else:
                assert not sdlimage.IMG_isJPG(imgrw)

def test_IMG_isLBM(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "lbm":
                assert sdlimage.IMG_isLBM(imgrw)
            else:
                assert not sdlimage.IMG_isLBM(imgrw)

def test_IMG_isPCX(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "pcx":
                assert sdlimage.IMG_isPCX(imgrw)
            else:
                assert not sdlimage.IMG_isPCX(imgrw)

def test_IMG_isPNG(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "png":
                assert sdlimage.IMG_isPNG(imgrw)
            else:
                assert not sdlimage.IMG_isPNG(imgrw)

def test_IMG_isPNM(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt in ("pnm", "pbm", "ppm", "pgm"):
                assert sdlimage.IMG_isPNM(imgrw)
            else:
                assert not sdlimage.IMG_isPNM(imgrw)

@pytest.mark.skipif(sdlimage.dll.version < 2002, reason="Added in 2.0.2")
@pytest.mark.xfail(isconda and iswindows, reason="Broken w/ win64 Conda")
def test_IMG_isSVG(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "svg":
                assert sdlimage.IMG_isSVG(imgrw)
            else:
                assert not sdlimage.IMG_isSVG(imgrw)

def test_IMG_isTIF(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "tif":
                assert sdlimage.IMG_isTIF(imgrw)
            else:
                assert not sdlimage.IMG_isTIF(imgrw)

def test_IMG_isWEBP(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "webp":
                assert sdlimage.IMG_isWEBP(imgrw)
            else:
                assert not sdlimage.IMG_isWEBP(imgrw)

@pytest.mark.xfail(bad_xcf, reason="XCF currently broken on some platforms")
def test_IMG_isXCF(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "xcf":
                assert sdlimage.IMG_isXCF(imgrw)
            else:
                assert not sdlimage.IMG_isXCF(imgrw)

def test_IMG_isXPM(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "xpm":
                assert sdlimage.IMG_isXPM(imgrw)
            else:
                assert not sdlimage.IMG_isXPM(imgrw)

@pytest.mark.skip("not implemented")
def test_IMG_isXV(with_sdl_image):
    for fmt in formats:
        fpath = _get_image_path(fmt)
        with open(fpath, "rb") as fp:
            imgrw = rwops.rw_from_object(fp)
            if fmt == "xv":
                assert sdlimage.IMG_isXV(imgrw)
            else:
                assert not sdlimage.IMG_isXV(imgrw)

@pytest.mark.skipif(hasattr(sys, "pypy_version_info"),
    reason="PyPy's ctypes fails to pass a correct string array")
def test_IMG_ReadXPMFromArray(with_sdl_image):
    fp = open(_get_image_path("xpm"), "rb")
    xpm = b""
    fp.readline()  # /* XPM */
    fp.readline()  # static char * surfacetest_xpm[] = {
    lbuf = fp.readlines()
    fp.close()
    for line in lbuf:
        if line.endswith(b"};"):
            xpm += line[1:-4]
        else:
            xpm += line[1:-3]
    pxpm = ctypes.c_char_p(xpm)
    sf = sdlimage.IMG_ReadXPMFromArray(ctypes.byref(pxpm))
    _verify_img_load(sf)
    surface.SDL_FreeSurface(sf)

def test_IMG_SavePNG(tmpdir):
    # Open a PNG that we can re-save
    fpath = _get_image_path("png")
    sf = sdlimage.IMG_Load(fpath.encode("utf-8"))
    assert isinstance(sf.contents, surface.SDL_Surface)

    # Try saving the PNG to a new folder
    outpath = os.path.join(str(tmpdir), "save_test.png")
    ret = sdlimage.IMG_SavePNG(sf, outpath.encode("utf-8"))
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    assert os.path.exists(outpath)
    surface.SDL_FreeSurface(sf)

@pytest.mark.skip("not working yet")
def test_IMG_SavePNG_RW(tmpdir):
    # Open a PNG that we can re-save
    fpath = _get_image_path("png")
    sf = sdlimage.IMG_Load(fpath.encode("utf-8"))
    assert isinstance(sf.contents, surface.SDL_Surface)

    # Try saving the PNG to a new folder
    outpath = os.path.join(str(tmpdir), "save_test.png")
    rw = rwops.SDL_RWFromFile(outpath.encode("utf-8"), b"wb")
    ret = sdlimage.IMG_SavePNG_RW(sf, rw, 0)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    assert os.path.exists(outpath)

    # Try reopening the RW as a PNG
    sf2 = sdlimage.IMG_LoadPNG_RW(rw)
    assert sdl2.SDL_GetError() == b""
    assert isinstance(sf2.contents, surface.SDL_Surface)
    surface.SDL_FreeSurface(sf)
    surface.SDL_FreeSurface(sf2)

@pytest.mark.skipif(no_jpeg_save, reason="Added in 2.0.2, not in macOS bnaries")
def test_IMG_SaveJPG(tmpdir):
    # Open a PNG that we can save to JPG
    fpath = _get_image_path("png")
    sf = sdlimage.IMG_Load(fpath.encode("utf-8"))
    assert isinstance(sf.contents, surface.SDL_Surface)

    # Try saving as JPG to a new folder
    outpath = os.path.join(str(tmpdir), "save_test.jpg")
    ret = sdlimage.IMG_SaveJPG(sf, outpath.encode("utf-8"), 90)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    assert os.path.exists(outpath)
    surface.SDL_FreeSurface(sf)

@pytest.mark.skip("not working yet")
@pytest.mark.skipif(no_jpeg_save, reason="Added in 2.0.2, not in macOS bnaries")
def test_IMG_SaveJPG_RW(tmpdir):
    # Open a PNG that we can save to JPG
    fpath = _get_image_path("png")
    sf = sdlimage.IMG_Load(fpath.encode("utf-8"))
    assert isinstance(sf.contents, surface.SDL_Surface)

    # Try saving as JPG to a new folder
    outpath = os.path.join(str(tmpdir), "save_test.jpg")
    rw = rwops.SDL_RWFromFile(outpath.encode("utf-8"), b"wb")
    ret = sdlimage.IMG_SaveJPG_RW(sf, rw, 0, 90)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    assert os.path.exists(outpath)

    # Try reopening the RW as a JPG
    sf2 = sdlimage.IMG_LoadJPG_RW(rw)
    assert sdl2.SDL_GetError() == b""
    assert isinstance(sf2.contents, surface.SDL_Surface)
    surface.SDL_FreeSurface(sf)
    surface.SDL_FreeSurface(sf2)
