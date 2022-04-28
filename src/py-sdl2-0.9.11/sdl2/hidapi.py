from ctypes import (
    c_char_p, c_wchar_p, c_int, c_ubyte, c_ushort, c_size_t, c_void_p,
    Structure, POINTER
)
from .dll import _bind
from .stdinc import Uint32, SDL_bool


__all__ = [
    # Structs
    "SDL_hid_device", "SDL_hid_device_info",

    # Functions
    "SDL_hid_init", "SDL_hid_exit", "SDL_hid_device_change_count",
    "SDL_hid_enumerate", "SDL_hid_free_enumeration",
    "SDL_hid_open", "SDL_hid_open_path",
    "SDL_hid_write", "SDL_hid_read_timeout", "SDL_hid_read",
    "SDL_hid_set_nonblocking",
    "SDL_hid_send_feature_report", "SDL_hid_get_feature_report",
    "SDL_hid_close",
    "SDL_hid_get_manufacturer_string", "SDL_hid_get_product_string",
    "SDL_hid_get_serial_number_string", "SDL_hid_get_indexed_string",
    "SDL_hid_ble_scan",
]


class SDL_hid_device(c_void_p):
    pass

class SDL_hid_device_info(Structure):
    pass

SDL_hid_device_info._fields_ = [
    ("path", c_char_p),
    ("vendor_id", c_ushort),
    ("product_id", c_ushort),
    ("serial_number", c_wchar_p),
    ("release_number", c_ushort),
    ("manufacturer_string", c_wchar_p),
    ("product_string", c_wchar_p),
    ("usage_page", c_ushort),
    ("usage", c_ushort),
    ("interface_number", c_int),
    ("interface_class", c_int),
    ("interface_subclass", c_int),
    ("interface_protocol", c_int),
    ("next", POINTER(SDL_hid_device_info))
]


SDL_hid_init = _bind("SDL_hid_init", None, c_int, added='2.0.18')
SDL_hid_exit = _bind("SDL_hid_exit", None, c_int, added='2.0.18')
SDL_hid_device_change_count = _bind("SDL_hid_device_change_count", None, Uint32, added='2.0.18')
SDL_hid_enumerate = _bind("SDL_hid_enumerate", [c_ushort, c_ushort], POINTER(SDL_hid_device_info), added='2.0.18')
SDL_hid_free_enumeration = _bind("SDL_hid_free_enumeration", [POINTER(SDL_hid_device_info)], None, added='2.0.18')
SDL_hid_open = _bind("SDL_hid_open", [c_ushort, c_ushort, c_wchar_p], POINTER(SDL_hid_device), added='2.0.18')
SDL_hid_open_path = _bind("SDL_hid_open_path", [c_char_p, c_int], POINTER(SDL_hid_device), added='2.0.18')

# NOTE: not sure if POINTER(c_ubyte) is right for this, need to test
SDL_hid_write = _bind("SDL_hid_write", [POINTER(SDL_hid_device), POINTER(c_ubyte), c_size_t], c_int, added='2.0.18')
SDL_hid_read_timeout = _bind("SDL_hid_read_timeout", [POINTER(SDL_hid_device), POINTER(c_ubyte), c_size_t, c_int], c_int, added='2.0.18')
SDL_hid_read = _bind("SDL_hid_read", [POINTER(SDL_hid_device), POINTER(c_ubyte), c_size_t], c_int, added='2.0.18')

SDL_hid_set_nonblocking = _bind("SDL_hid_set_nonblocking", [POINTER(SDL_hid_device), c_int], c_int, added='2.0.18')
SDL_hid_send_feature_report = _bind("SDL_hid_send_feature_report", [POINTER(SDL_hid_device), POINTER(c_ubyte), c_size_t], c_int, added='2.0.18')
SDL_hid_get_feature_report = _bind("SDL_hid_get_feature_report", [POINTER(SDL_hid_device), POINTER(c_ubyte), c_size_t], c_int, added='2.0.18')
SDL_hid_close = _bind("SDL_hid_close", [POINTER(SDL_hid_device)], None, added='2.0.18')
SDL_hid_get_manufacturer_string = _bind("SDL_hid_get_manufacturer_string", [POINTER(SDL_hid_device), c_wchar_p, c_size_t], c_int, added='2.0.18')
SDL_hid_get_product_string = _bind("SDL_hid_get_product_string", [POINTER(SDL_hid_device), c_wchar_p, c_size_t], c_int, added='2.0.18')
SDL_hid_get_serial_number_string = _bind("SDL_hid_get_serial_number_string", [POINTER(SDL_hid_device), c_wchar_p, c_size_t], c_int, added='2.0.18')
SDL_hid_get_indexed_string = _bind("SDL_hid_get_indexed_string", [POINTER(SDL_hid_device), c_int, c_wchar_p, c_size_t], c_int, added='2.0.18')
SDL_hid_ble_scan = _bind("SDL_hid_ble_scan", [SDL_bool], None, added='2.0.18')
