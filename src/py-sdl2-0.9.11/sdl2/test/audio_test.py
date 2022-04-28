import os
import sys
import ctypes
import sdl2
from sdl2 import SDL_Init, SDL_Quit, SDL_InitSubSystem, SDL_QuitSubSystem, \
    SDL_INIT_AUDIO
from sdl2.error import SDL_GetError, SDL_ClearError
from sdl2 import audio
import pytest

# NOTE: This module is missing a lot of tests, but is also going to be tricky
# to write more tests for.

# Get original audio driver, if one was set in the environment
original_driver = os.getenv("SDL_AUDIODRIVER", None)

@pytest.fixture
def with_sdl_audio():
    # Reset original audio driver in the environment (if there was one)
    if original_driver:
        os.environ["SDL_AUDIODRIVER"] = original_driver
    # Initialize SDL2 with video and audio subsystems
    sdl2.SDL_ClearError()
    ret = sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO | sdl2.SDL_INIT_AUDIO)
    assert sdl2.SDL_GetError() == b""
    assert ret == 0
    yield
    sdl2.SDL_Quit()
    # Reset original audio driver in environment
    os.environ.pop("SDL_AUDIODRIVER", None)
    if original_driver:
        os.environ["SDL_AUDIODRIVER"] = original_driver


# Test macro functions

def test_SDL_AUDIO_BITSIZE():
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_U8) == 8
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S8) == 8
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_U16LSB) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S16LSB) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_U16MSB) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S16MSB) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_U16) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S16) == 16
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S32LSB) == 32
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S32MSB) == 32
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_S32) == 32
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_F32LSB) == 32
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_F32MSB) == 32
    assert audio.SDL_AUDIO_BITSIZE(audio.AUDIO_F32) == 32

def test_SDL_AUDIO_ISFLOAT():
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_U8)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S8)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_U16LSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S16LSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_U16MSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S16MSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_U16)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S16)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S32LSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S32MSB)
    assert not audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_S32)
    assert audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_F32LSB)
    assert audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_F32MSB)
    assert audio.SDL_AUDIO_ISFLOAT(audio.AUDIO_F32)

def test_SDL_AUDIO_ISBIGENDIAN():
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_U8)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S8)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_U16LSB)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S16LSB)
    assert audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_U16MSB)
    assert audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S16MSB)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_U16)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S16)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S32LSB)
    assert audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S32MSB)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_S32)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_F32LSB)
    assert audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_F32MSB)
    assert not audio.SDL_AUDIO_ISBIGENDIAN(audio.AUDIO_F32)

def test_SDL_AUDIO_ISSIGNED():
    assert not audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_U8)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S8)
    assert not audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_U16LSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S16LSB)
    assert not audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_U16MSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S16MSB)
    assert not audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_U16)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S16)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S32LSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S32MSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_S32)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_F32LSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_F32MSB)
    assert audio.SDL_AUDIO_ISSIGNED(audio.AUDIO_F32)

def test_SDL_AUDIO_ISINT():
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_U8)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S8)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_U16LSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S16LSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_U16MSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S16MSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_U16)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S16)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S32LSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S32MSB)
    assert audio.SDL_AUDIO_ISINT(audio.AUDIO_S32)
    assert not audio.SDL_AUDIO_ISINT(audio.AUDIO_F32LSB)
    assert not audio.SDL_AUDIO_ISINT(audio.AUDIO_F32MSB)
    assert not audio.SDL_AUDIO_ISINT(audio.AUDIO_F32)

def test_SDL_AUDIO_ISLITTLEENDIAN():
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_U8)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S8)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_U16LSB)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S16LSB)
    assert not audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_U16MSB)
    assert not audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S16MSB)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_U16)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S16)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S32LSB)
    assert not audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S32MSB)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_S32)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_F32LSB)
    assert not audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_F32MSB)
    assert audio.SDL_AUDIO_ISLITTLEENDIAN(audio.AUDIO_F32)

def test_SDL_AUDIO_ISUNSIGNED():
    assert audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_U8)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S8)
    assert audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_U16LSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S16LSB)
    assert audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_U16MSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S16MSB)
    assert audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_U16)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S16)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S32LSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S32MSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_S32)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_F32LSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_F32MSB)
    assert not audio.SDL_AUDIO_ISUNSIGNED(audio.AUDIO_F32)


# Test structs and objects

@pytest.mark.skip("not implemented")
def test_SDL_AudioSpec():
    pass

@pytest.mark.skip("not implemented")
def test_SDL_AudioCVT():
    pass

@pytest.mark.skip("not implemented")
def test_SDL_AudioStream():
    pass


# Test actual function bindings

@pytest.mark.skip("not implemented")
def test_SDL_AudioInitQuit():
    pass

def test_SDL_GetNumAudioDrivers(with_sdl_audio):
    count = audio.SDL_GetNumAudioDrivers()
    assert count >= 1

def test_SDL_GetAudioDriver(with_sdl_audio):
    founddummy = False
    drivercount = audio.SDL_GetNumAudioDrivers()
    for index in range(drivercount):
        drivername = audio.SDL_GetAudioDriver(index)
        assert isinstance(drivername, (str, bytes))
        if drivername == b"dummy":
            founddummy = True
    assert founddummy

def test_SDL_GetCurrentAudioDriver(with_sdl_audio):
    success = 0
    # Reset audio subsystem
    SDL_Quit()
    SDL_Init(0)
    for index in range(audio.SDL_GetNumAudioDrivers()):
        drivername = audio.SDL_GetAudioDriver(index)
        os.environ["SDL_AUDIODRIVER"] = drivername.decode("utf-8")
        # Certain drivers fail without bringing up the correct
        # return value, such as the esd, if it is not running.
        SDL_InitSubSystem(SDL_INIT_AUDIO)
        driver = audio.SDL_GetCurrentAudioDriver()
        SDL_QuitSubSystem(SDL_INIT_AUDIO)
        # Do not handle wrong return values.
        if driver is not None:
            assert drivername == driver
            success += 1
    assert success >= 1

def test_SDL_OpenCloseAudio(with_sdl_audio):
    # TODO: Add test that checks which audio formats are supported for each
    # audio device?
    fmt = audio.AUDIO_F32 if sys.platform == "darwin" else audio.AUDIO_U16
    reqspec = audio.SDL_AudioSpec(44100, fmt, 2, 1024)
    spec = audio.SDL_AudioSpec(0, 0, 0, 0)
    ret = audio.SDL_OpenAudio(reqspec, ctypes.byref(spec))
    assert ret == 0
    assert spec.format > 0  # Can't guarantee we'll get requested format
    assert spec.freq == reqspec.freq
    assert spec.channels == reqspec.channels
    audio.SDL_CloseAudio()

def test_SDL_GetNumAudioDevices(with_sdl_audio):
    outnum = audio.SDL_GetNumAudioDevices(False)
    assert outnum >= 1
    innum = audio.SDL_GetNumAudioDevices(True)
    assert innum >= 0

def test_SDL_GetAudioDeviceName(with_sdl_audio):
    # NOTE: Check & print errors for drivers that failed to load?
    backends = []
    devices = {}
    # Reset audio subsystem
    SDL_Quit()
    SDL_Init(0)
    for index in range(audio.SDL_GetNumAudioDrivers()):
        # Get input/output device names for each audio driver
        drivername = audio.SDL_GetAudioDriver(index)
        backends.append(drivername.decode("utf-8"))
        os.environ["SDL_AUDIODRIVER"] = drivername.decode("utf-8")
        # Need to reinitialize subsystem for each driver
        SDL_InitSubSystem(SDL_INIT_AUDIO)
        driver = audio.SDL_GetCurrentAudioDriver()
        if driver is not None:
            driver = driver.decode("utf-8")
            devices[driver] = {'input': [], 'output': []}
            outnum = audio.SDL_GetNumAudioDevices(False)
            innum = audio.SDL_GetNumAudioDevices(True)
            for x in range(outnum):
                name = audio.SDL_GetAudioDeviceName(x, False)
                assert name is not None
                devices[driver]['output'].append(name.decode('utf-8'))
            for x in range(innum):
                name = audio.SDL_GetAudioDeviceName(x, True)
                assert name is not None
                devices[driver]['input'].append(name.decode('utf-8'))
        SDL_QuitSubSystem(SDL_INIT_AUDIO)
    print("Audio backends supported by current SDL2 binary:")
    print(backends)
    print("\nAvailable audio drivers and devices:")
    for driver in devices.keys():
        print(driver)
        print(" - input: {0}".format(str(devices[driver]['input'])))
        print(" - output: {0}".format(str(devices[driver]['output'])))

@pytest.mark.skipif(sdl2.dll.version < 2016, reason="not available")
def test_SDL_GetAudioDeviceSpec(with_sdl_audio):
    # Reset audio subsystem
    SDL_Quit()
    SDL_Init(0)
    # Find an audio driver with at least one output
    SDL_InitSubSystem(SDL_INIT_AUDIO)
    driver = audio.SDL_GetCurrentAudioDriver()
    if driver == None or audio.SDL_GetNumAudioDevices(False) == 0:
        SDL_QuitSubSystem(SDL_INIT_AUDIO)
        os.environ["SDL_AUDIODRIVER"] = b'dummy'
        SDL_InitSubSystem(SDL_INIT_AUDIO)
        driver = audio.SDL_GetCurrentAudioDriver()
    drivername = driver.decode('utf-8')
    # Get name and spec of first output device
    outspec = audio.SDL_AudioSpec(0, 0, 0, 0)
    outname = audio.SDL_GetAudioDeviceName(0, False).decode('utf-8')
    ret = audio.SDL_GetAudioDeviceSpec(0, False, ctypes.byref(outspec))
    SDL_QuitSubSystem(SDL_INIT_AUDIO)
    assert ret == 0
    # Validate frequency and channel count were set
    hz = outspec.freq
    fmt = audio.FORMAT_NAME_MAP[outspec.format] if outspec.format > 0 else 'unknown'
    chans = outspec.channels
    bufsize = outspec.samples if outspec.samples > 0 else 'unknown'
    if driver != b"dummy":
        assert hz > 0
        assert chans > 0
        # Print out device spec info
        msg = "Audio device spec for {0} with '{1}' driver:"
        msg2 = "{0} Hz, {1} channels, {2} format, {3} sample buffer size"
        print(msg.format(outname, drivername))
        print(msg2.format(hz, chans, fmt, bufsize))

def test_SDL_OpenCloseAudioDevice(with_sdl_audio):
    #TODO: Add tests for callback
    fmt = audio.AUDIO_F32 if sys.platform == "darwin" else audio.AUDIO_U16
    reqspec = audio.SDL_AudioSpec(44100, fmt, 2, 1024)
    outnum = audio.SDL_GetNumAudioDevices(0)
    for x in range(outnum):
        spec = audio.SDL_AudioSpec(0, 0, 0, 0)
        name = audio.SDL_GetAudioDeviceName(x, 0)
        assert name is not None
        deviceid = audio.SDL_OpenAudioDevice(
            name, 0, reqspec, ctypes.byref(spec),
            audio.SDL_AUDIO_ALLOW_ANY_CHANGE
        )
        err = SDL_GetError()
        assert deviceid >= 2
        assert isinstance(spec, audio.SDL_AudioSpec)
        assert spec.format in audio.AUDIO_FORMATS
        assert spec.freq > 0
        assert spec.channels > 0
        assert spec.samples > 0
        audio.SDL_CloseAudioDevice(deviceid)

@pytest.mark.skip("not implemented")
def test_SDL_GetAudioStatus(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GetAudioDeviceStatus(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_PauseAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_PauseAudioDevice(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_LoadWAV_RW(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_LoadWAV(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_FreeWAV(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_BuildAudioCVT(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_ConvertAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_MixAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_MixAudioFormat(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_LockUnlockAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_LockUnlockAudioDevice(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_QueueAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_GetQueuedAudioSize(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_ClearQueuedAudio(self):
    pass

@pytest.mark.skip("not implemented")
def test_SDL_DequeueAudio(self):
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_NewAudioStream():
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_AudioStreamPut():
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_AudioStreamGet():
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_AudioStreamAvailable():
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_AudioStreamClear():
    pass

@pytest.mark.skip("not implemented")
@pytest.mark.skipif(sdl2.dll.version < 2007, reason="not available")
def test_SDL_FreeAudioStream():
    pass
