import sys
import pytest
import sdl2
from sdl2 import cpuinfo

_HASMP = True
try:
    import multiprocessing
except:
    _HASMP = False


def test_SDL_GetCPUCacheLineSize():
    ret = cpuinfo.SDL_GetCPUCacheLineSize()
    assert isinstance(ret, int)

def test_SDL_GetCPUCount():
    if _HASMP:
        assert multiprocessing.cpu_count() == cpuinfo.SDL_GetCPUCount()
    else:
        assert cpuinfo.SDL_GetCPUCount() >= 1

def test_SDL_Has3DNow():
    ret = cpuinfo.SDL_Has3DNow()
    assert ret in (0, 1)

def test_SDL_HasAltiVec():
    ret = cpuinfo.SDL_HasAltiVec()
    assert ret in (0, 1)

def test_SDL_HasMMX():
    ret = cpuinfo.SDL_HasMMX()
    assert ret in (0, 1)

def test_SDL_HasRDTSC():
    ret = cpuinfo.SDL_HasRDTSC()
    assert ret in (0, 1)

def test_SDL_HasSSE():
    ret = cpuinfo.SDL_HasSSE()
    assert ret in (0, 1)

def test_SDL_HasSSE2():
    ret = cpuinfo.SDL_HasSSE2()
    assert ret in (0, 1)

def test_SDL_HasSSE3():
    ret = cpuinfo.SDL_HasSSE3()
    assert ret in (0, 1)

def test_SDL_HasSSE41():
    ret = cpuinfo.SDL_HasSSE41()
    assert ret in (0, 1)

def test_SDL_HasSSE42():
    ret = cpuinfo.SDL_HasSSE42()
    assert ret in (0, 1)

def test_SDL_HasAVX():
    ret = cpuinfo.SDL_HasAVX()
    assert ret in (0, 1)

def test_SDL_HasAVX2():
    ret = cpuinfo.SDL_HasAVX2()
    assert ret in (0, 1)

def test_SDL_GetSystemRAM():
    ret = cpuinfo.SDL_GetSystemRAM()
    assert ret > 0

@pytest.mark.skipif(sdl2.dll.version < 2009, reason="not available")
def test_SDL_HasAVX512F():
    ret = cpuinfo.SDL_HasAVX512F()
    assert ret in (0, 1)

@pytest.mark.skipif(sdl2.dll.version < 2012, reason="not available")
def test_SDL_HasARMSIMD():
    ret = cpuinfo.SDL_HasARMSIMD()
    assert ret in (0, 1)

@pytest.mark.skipif(sdl2.dll.version < 2006, reason="not available")
def test_SDL_HasNEON():
    ret = cpuinfo.SDL_HasNEON()
    assert ret in (0, 1)

@pytest.mark.skipif(sdl2.dll.version < 2010, reason="not available")
def test_SDL_SIMDGetAlignment():
    ret = cpuinfo.SDL_SIMDGetAlignment()
    assert ret % 8 == 0 # Should be multiple of 8

@pytest.mark.skip("not implemented (no clue how)")
@pytest.mark.skipif(sdl2.dll.version < 2010, reason="not available")
def test_SDL_SIMDAllocFree():
    # Should test both SDL_SIMDAlloc and SDL_SIMDFree
    pass

@pytest.mark.skip("not implemented (no clue how)")
@pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
def test_SDL_SIMDRealloc():
    pass
