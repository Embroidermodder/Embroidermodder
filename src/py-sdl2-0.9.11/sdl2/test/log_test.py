import sys
import pytest
import ctypes
from sdl2 import log

class TestSDLLog(object):
    __tags__ = ["sdl"]

    def setup_method(self):
        self.logdata = []
        def logfunc(userdata, category, priority, message):
            if userdata:
                userdata = ctypes.cast(userdata, ctypes.c_char_p).value
            self.logdata.append((userdata, category, priority, message,))

        # bind to the TestCase, so we do not loose the reference.
        self.funcptr = log.SDL_LogOutputFunction(logfunc)
        log.SDL_LogSetOutputFunction(self.funcptr, None)
        log.SDL_LogSetAllPriority(log.SDL_LOG_PRIORITY_VERBOSE)

    def teardown_method(self):
        log.SDL_LogSetOutputFunction(log.SDL_LogOutputFunction(), None)
        del self.funcptr

    def test_SDL_LogMessage(self):
        self.logdata = []  # reset the log
        log.SDL_LogMessage(log.SDL_LOG_CATEGORY_APPLICATION,
                           log.SDL_LOG_PRIORITY_VERBOSE, b"test")
        assert self.logdata[0] == (
            None,   log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_VERBOSE, b"test"
        )
        log.SDL_LogMessage(log.SDL_LOG_CATEGORY_CUSTOM,
                           log.SDL_LOG_PRIORITY_CRITICAL, b"test2")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_CUSTOM,
            log.SDL_LOG_PRIORITY_CRITICAL, b"test2"
        )

        #self.assertRaises(TypeError, log.SDL_LogMessage, None, None, None)
        #self.assertRaises(ValueError, log.SDL_LogMessage, 123, None, None)
        #self.assertRaises(ValueError, log.SDL_LogMessage, 123, 456, None)
        #self.assertRaises(ValueError, log.SDL_LogMessage, 123, 456, "Test")
        #self.assertRaises(TypeError, log.SDL_LogMessage,
        #                  log.SDL_LOG_CATEGORY_CUSTOM, None, None)
        #self.assertRaises(ValueError, log.SDL_LogMessage,
        #                  log.SDL_LOG_CATEGORY_CUSTOM, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogMessage,
        #                  log.SDL_LOG_CATEGORY_CUSTOM, 123, "Test")

    def test_SDL_Log(self):
        self.logdata = []  # reset the log
        log.SDL_Log(b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_INFO, b"test"
        )
        log.SDL_Log(b"abcdeghijk")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_INFO, b"abcdeghijk"
        )

    def test_SDL_LogCritical(self):
        self.logdata = []  # reset the log
        log.SDL_LogCritical(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_CRITICAL, b"test"
        )
        log.SDL_LogCritical(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_CRITICAL, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogCritical, None, None)
        #self.assertRaises(ValueError, log.SDL_LogCritical, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogCritical, 123, "Test")

    def test_SDL_LogDebug(self):
        self.logdata = []  # reset the log
        log.SDL_LogDebug(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_DEBUG, b"test"
        )
        log.SDL_LogDebug(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_DEBUG, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogDebug, None, None)
        #self.assertRaises(ValueError, log.SDL_LogDebug, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogDebug, 123, "Test")

    def test_SDL_LogError(self):
        self.logdata = []  # reset the log
        log.SDL_LogError(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_ERROR, b"test"
        )
        log.SDL_LogError(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_ERROR, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogError, None, None)
        #self.assertRaises(ValueError, log.SDL_LogError, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogError, 123, "Test")

    def test_SDL_LogInfo(self):
        self.logdata = []  # reset the log
        log.SDL_LogInfo(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_INFO, b"test"
        )
        log.SDL_LogInfo(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_INFO, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogInfo, None, None)
        #self.assertRaises(ValueError, log.SDL_LogInfo, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogInfo, 123, "Test")

    def test_SDL_LogVerbose(self):
        self.logdata = []  # reset the log
        log.SDL_LogVerbose(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_VERBOSE, b"test"
        )
        log.SDL_LogVerbose(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_VERBOSE, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogVerbose, None, None)
        #self.assertRaises(ValueError, log.SDL_LogVerbose, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogVerbose, 123, "Test")

    def test_SDL_LogWarn(self):
        self.logdata = []  # reset the log
        log.SDL_LogWarn(log.SDL_LOG_CATEGORY_APPLICATION, b"test")
        assert self.logdata[0] == (
            None, log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_WARN, b"test"
        )
        log.SDL_LogWarn(log.SDL_LOG_CATEGORY_SYSTEM, b"test")
        assert self.logdata[1] == (
            None, log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_PRIORITY_WARN, b"test"
        )

        #self.assertRaises(TypeError, log.SDL_LogWarn, None, None)
        #self.assertRaises(ValueError, log.SDL_LogWarn, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogWarn, 123, "Test")

    def test_SDL_LogSetAllPriority(self):
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_APPLICATION) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_SYSTEM) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        log.SDL_LogSetAllPriority(log.SDL_LOG_PRIORITY_WARN)
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_APPLICATION) == \
                                      log.SDL_LOG_PRIORITY_WARN
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_SYSTEM) == \
                                      log.SDL_LOG_PRIORITY_WARN
        # self.assertRaises(ValueError, log.SDL_LogSetAllPriority, 123)
        # self.assertRaises(TypeError, log.SDL_LogSetAllPriority, None)
        # self.assertRaises(TypeError, log.SDL_LogSetAllPriority, "test")

        # Reset to the setUp() value, so other tests do not fail
        log.SDL_LogSetAllPriority(log.SDL_LOG_PRIORITY_VERBOSE)

    def test_SDL_LogGetSetPriority(self):
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_APPLICATION) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_SYSTEM) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_CUSTOM) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        log.SDL_LogSetPriority(log.SDL_LOG_CATEGORY_CUSTOM,
                               log.SDL_LOG_PRIORITY_INFO)
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_APPLICATION) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_SYSTEM) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_CUSTOM) == \
                                      log.SDL_LOG_PRIORITY_INFO
        log.SDL_LogSetPriority(log.SDL_LOG_CATEGORY_SYSTEM,
                               log.SDL_LOG_PRIORITY_ERROR)
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_APPLICATION) == \
                                      log.SDL_LOG_PRIORITY_VERBOSE
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_SYSTEM) == \
                                      log.SDL_LOG_PRIORITY_ERROR
        assert log.SDL_LogGetPriority(log.SDL_LOG_CATEGORY_CUSTOM) == \
                                      log.SDL_LOG_PRIORITY_INFO
        #self.assertRaises(TypeError, log.SDL_LogSetPriority, None, None)
        #self.assertRaises(TypeError, log.SDL_LogSetPriority, "Test", None)
        #self.assertRaises(ValueError, log.SDL_LogSetPriority, 123, None)
        #self.assertRaises(ValueError, log.SDL_LogSetPriority, 123, "Test")
        #self.assertRaises(TypeError, log.SDL_LogSetPriority,
        #                  log.SDL_LOG_CATEGORY_APPLICATION, None)
        #self.assertRaises(TypeError, log.SDL_LogSetPriority,
        #                  log.SDL_LOG_CATEGORY_APPLICATION, "Test")
        #self.assertRaises(ValueError, log.SDL_LogSetPriority,
        #                  log.SDL_LOG_CATEGORY_APPLICATION, 123)

        #self.assertRaises(TypeError, log.SDL_LogGetPriority, None)
        #self.assertRaises(TypeError, log.SDL_LogGetPriority, "Test")
        #self.assertRaises(ValueError, log.SDL_LogGetPriority, 123)

    def test_SDL_LogResetPriorities(self):
        # set in setUp()
        defpriority = log.SDL_LOG_PRIORITY_VERBOSE
        categories = (
            log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_CATEGORY_ERROR,
            log.SDL_LOG_CATEGORY_SYSTEM,
            log.SDL_LOG_CATEGORY_AUDIO,
            log.SDL_LOG_CATEGORY_VIDEO,
            log.SDL_LOG_CATEGORY_RENDER,
            log.SDL_LOG_CATEGORY_INPUT,
            log.SDL_LOG_CATEGORY_CUSTOM
            )
        for cat in categories:
            priority = log.SDL_LogGetPriority(cat)
            assert priority == defpriority

        log.SDL_LogResetPriorities()
        for cat in categories:
            priority = log.SDL_LogGetPriority(cat)
            assert priority != defpriority

        log.SDL_LogSetAllPriority(log.SDL_LOG_PRIORITY_VERBOSE)

    def test_SDL_LogGetSetOutputFunction(self):
        logentries = []

        def __log(userdata, category, priority, message):
            if userdata:
                userdata = ctypes.cast(userdata, ctypes.c_char_p).value
            logentries.append((userdata, category, priority, message,))

        # setUp should have set our output function already.
        origfunc = log.SDL_LogOutputFunction()
        origdata = ctypes.c_void_p(0)
        log.SDL_LogGetOutputFunction(ctypes.byref(origfunc),
                                     ctypes.byref(origdata))
        assert not origdata
        logcount = len(self.logdata)
        origfunc(None, 0, 0, b"test_log_get_set_output_function")
        assert len(self.logdata) == logcount + 1
        assert self.logdata[logcount][3] == b"test_log_get_set_output_function"

        logptr = log.SDL_LogOutputFunction(__log)
        userdata = ctypes.c_char_p(b"Testobject")
        log.SDL_LogSetOutputFunction(logptr, userdata)
        ptr = log.SDL_LogOutputFunction()
        userdata = ctypes.c_void_p(0)
        log.SDL_LogGetOutputFunction(ctypes.byref(ptr), ctypes.byref(userdata))
        userdata = ctypes.cast(userdata, ctypes.c_char_p)
        assert userdata.value == b"Testobject"
        log.SDL_Log(b"output test")
        assert logentries[0] == (
            b"Testobject", log.SDL_LOG_CATEGORY_APPLICATION,
            log.SDL_LOG_PRIORITY_INFO, b"output test"
        )

        log.SDL_LogSetOutputFunction(origfunc, userdata)
