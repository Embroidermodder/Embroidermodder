from ctypes import Structure, POINTER, c_char_p, c_void_p, cast, sizeof
from .dll import _bind
from .dll import version as sdl_version

__all__ = [
    # Structs
    "SDL_Locale",

    # Functions
    "SDL_GetPreferredLocales"
]


class SDL_Locale(Structure):
    _fields_ = [("language", c_char_p),
                ("country", c_char_p)
                ]

    def __repr__(self):
        lang, country = (self.language, self.country)
        if type(lang) == bytes and type(lang) != str:
            lang = lang.decode('utf-8')
            country = country.decode('utf-8') if country else ''
        country_str = ", country={0}".format(country) if country else ''
        return "SDL_Locale(lang={0}{1})".format(lang, country_str)


_SDL_GetPreferredLocales = _bind("SDL_GetPreferredLocales", None, c_void_p, added='2.0.14')

def SDL_GetPreferredLocales():
    """The base SDL2 function returns a pointer to an array of locales with an
    arbitrary length, which is just plain awful to deal with in Python. This
    wrapper returns a Python list of locales instead.
    """
    locales = []
    loc_size = sizeof(SDL_Locale)
    p = _SDL_GetPreferredLocales()  # Get void pointer to first locale in array
    while True:
        loc = cast(p, POINTER(SDL_Locale))
        if not loc:
            # Break if loc is a null pointer
            break
        if loc.contents.language == None:
            # Array of locales is terminated by locale with null language
            break
        locales.append(loc.contents)
        p = p + loc_size  # Increment pointer to next locale in array
    return locales