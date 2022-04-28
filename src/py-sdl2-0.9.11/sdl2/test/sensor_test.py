import sys
import pytest
from ctypes import c_float 
import sdl2
from sdl2 import SDL_Init, SDL_Quit, SDL_INIT_SENSOR
from sdl2.events import SDL_QUERY, SDL_ENABLE, SDL_IGNORE
from sdl2.stdinc import SDL_TRUE, SDL_FALSE
from sdl2.error import SDL_GetError, SDL_ClearError
from sdl2 import sensor

# Make sure sensor subsystem is available and works before running tests
available = sdl2.dll.version >= 2009
ret = SDL_Init(SDL_INIT_SENSOR)
SDL_Quit()
skipmsg = 'Sensor subsystem not supported'
pytestmark = pytest.mark.skipif(ret != 0 or not available, reason=skipmsg)


def test_SDL_NumSensors():
    ret = SDL_Init(SDL_INIT_SENSOR)
    assert ret == 0
    retval = sensor.SDL_NumSensors()
    SDL_Quit()
    assert retval >= 0


skipmsg = "sensor subsystem not supported"
@pytest.mark.skipif(SDL_Init(SDL_INIT_SENSOR) != 0, reason=skipmsg)
class TestSDLSensor(object):
    __tags__ = ["sdl"]

    @classmethod
    def setup_class(cls):
        SDL_Init(SDL_INIT_SENSOR)
        num = sensor.SDL_NumSensors()
        if num < 1:
            pytest.skip("no available sensor devices")
        cls.scount = num

    @classmethod
    def teardown_class(cls):
        SDL_Quit()

    def setup_method(self):
        SDL_ClearError()

    def test_SDL_SensorOpenClose(self):
        for index in range(self.scount):
            s = sensor.SDL_SensorOpen(index)
            assert isinstance(s.contents, sensor.SDL_Sensor)
            sensor.SDL_SensorClose(s)

    def test_SDL_SensorFromInstanceID(self):
        for index in range(self.scount):
            iid = sensor.SDL_SensorGetDeviceInstanceID(index)
            s = sensor.SDL_SensorFromInstanceID(iid)
            assert isinstance(s.contents, sensor.SDL_Sensor)
            sensor.SDL_SensorClose(s)

    def test_SDL_SensorUpdate(self):
        sensor.SDL_SensorUpdate()

    def test_SDL_SensorDeviceInfo(self):
        types = {
            sensor.SDL_SENSOR_INVALID: "Invalid",
            sensor.SDL_SENSOR_UNKNOWN: "Unknown",
            sensor.SDL_SENSOR_ACCEL: "Accelerometer",
            sensor.SDL_SENSOR_GYRO: "Gyroscope"
        }
        for index in range(self.scount):
            name = sensor.SDL_SensorGetDeviceName(index)
            assert type(name) in (str, bytes)
            dtype = sensor.SDL_SensorGetDeviceType(index)
            assert dtype in list(types.keys())
            nptype = sensor.SDL_SensorGetDeviceNonPortableType(index)
            assert type(nptype) == int
            iid = sensor.SDL_SensorGetDeviceInstanceID(index)
            assert type(iid) == int

    def test_SDL_SensorInfo(self):
        sensors = []
        types = {
            sensor.SDL_SENSOR_INVALID: "Invalid",
            sensor.SDL_SENSOR_UNKNOWN: "Unknown",
            sensor.SDL_SENSOR_ACCEL: "Accelerometer",
            sensor.SDL_SENSOR_GYRO: "Gyroscope"
        }
        for index in range(self.scount):
            dtype = sensor.SDL_SensorGetDeviceType(index)
            if dtype in [sensor.SDL_SENSOR_INVALID, sensor.SDL_SENSOR_UNKNOWN]:
                continue
            s = sensor.SDL_SensorOpen(index)
            name = sensor.SDL_SensorGetName(s)
            assert type(name) in (str, bytes)
            dtype = sensor.SDL_SensorGetType(s)
            assert dtype in list(types.keys())
            nptype = sensor.SDL_SensorGetNonPortableType(s)
            assert type(nptype) == int
            iid = sensor.SDL_SensorGetInstanceID(s)
            assert type(iid) == int
            sensor.SDL_SensorUpdate()
            data = c_float(-0.00323)  # Set a random initial value
            assert sensor.SDL_SensorGetData(s, data, 1) == 0
            assert data.value != -0.00323
            sensor.SDL_SensorClose(s)
            s_info = {
                'name': name.decode('utf-8'), 'type': types[dtype],
                'np': nptype, 'id': iid, 'data': data.value
            }
            sensors.append(s_info)
        if len(sensors):
            print("Available sensors:")
        for s in sensors:
            print("  '{0}' (ID: {1}):".format(s['name'], s['id']))
            print("   - Sensor Type: {0}".format(s['type']))
            print("   - Non-Portable Type: {0}".format(s['nptype']))
            print("   - Current X Value: {0}".format(s['data']))
            
    @pytest.mark.skipif(sdl2.dll.version < 2014, reason="not available")
    def test_SDL_LockUnlockSensors(self):
        # NOTE: not sure how better to test these, since I don't know if
        # they'd even be useful at all in Python given the GIL
        sensor.SDL_LockSensors()
        sensor.SDL_UnlockSensors()
