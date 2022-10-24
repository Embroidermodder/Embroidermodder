import os
from ctypes import POINTER, c_int, c_char_p
from ctypes import POINTER as _P
from .dll import DLL, SDLFunc
from .version import SDL_version, SDL_VERSIONNUM
from .surface import SDL_Surface
from .rwops import SDL_RWops
from .render import SDL_Texture, SDL_Renderer
from .error import SDL_SetError, SDL_GetError

__all__ = [
    # Defines
    "SDL_IMAGE_MAJOR_VERSION", "SDL_IMAGE_MINOR_VERSION", "SDL_IMAGE_PATCHLEVEL",

    # Enums
    "IMG_InitFlags",
    "IMG_INIT_JPG", "IMG_INIT_PNG", "IMG_INIT_TIF", "IMG_INIT_WEBP",
    
    # Macro Functions
    "SDL_IMAGE_VERSION", "SDL_IMAGE_COMPILEDVERSION",
    "SDL_IMAGE_VERSION_ATLEAST", 
    
    # Functions
    "IMG_Linked_Version", "IMG_Init", "IMG_Quit", "IMG_LoadTyped_RW",
    "IMG_Load", "IMG_Load_RW", "IMG_LoadTexture", "IMG_LoadTexture_RW",
    "IMG_LoadTextureTyped_RW", "IMG_isICO", "IMG_isCUR", "IMG_isBMP",
    "IMG_isGIF", "IMG_isJPG", "IMG_isLBM", "IMG_isPNG", "IMG_isPNM",
    "IMG_isPCX", "IMG_isTIF", "IMG_isXCF", "IMG_isXV", "IMG_isWEBP",
    "IMG_LoadBMP_RW", "IMG_LoadCUR_RW", "IMG_LoadCUR_RW",
    "IMG_LoadGIF_RW", "IMG_LoadICO_RW", "IMG_LoadJPG_RW",
    "IMG_LoadLBM_RW", "IMG_LoadPCX_RW", "IMG_LoadPNM_RW",
    "IMG_LoadPNG_RW", "IMG_LoadTGA_RW", "IMG_LoadTIF_RW",
    "IMG_LoadXCF_RW", "IMG_LoadWEBP_RW", "IMG_LoadXPM_RW",
    "IMG_LoadXV_RW", "IMG_ReadXPMFromArray",
    "IMG_GetError", "IMG_SetError", "IMG_SaveJPG", "IMG_SaveJPG_RW",
    
    # Python Functions
    "get_dll_file"
]


try:
    dll = DLL("SDL2_image", ["SDL2_image", "SDL2_image-2.0"],
              os.getenv("PYSDL2_DLL_PATH"))
except RuntimeError as exc:
    raise ImportError(exc)


def get_dll_file():
    """Gets the file name of the loaded SDL2_image library."""
    return dll.libfile

_bind = dll.bind_function


# Constants, enums, type definitions, and macros

SDL_IMAGE_MAJOR_VERSION = 2
SDL_IMAGE_MINOR_VERSION = 0
SDL_IMAGE_PATCHLEVEL = 5

def SDL_IMAGE_VERSION(x):
    x.major = SDL_IMAGE_MAJOR_VERSION
    x.minor = SDL_IMAGE_MINOR_VERSION
    x.patch = SDL_IMAGE_PATCHLEVEL

SDL_IMAGE_COMPILEDVERSION = SDL_VERSIONNUM(SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL)
SDL_IMAGE_VERSION_ATLEAST = lambda x, y, z: (SDL_IMAGE_COMPILEDVERSION >= SDL_VERSIONNUM(x, y, z))

IMG_InitFlags = c_int
IMG_INIT_JPG = 0x00000001
IMG_INIT_PNG = 0x00000002
IMG_INIT_TIF = 0x00000004
IMG_INIT_WEBP = 0x00000008


# Raw ctypes function definitions

_funcdefs = [
    SDLFunc("IMG_Linked_Version", None, _P(SDL_version)),
    SDLFunc("IMG_Init", [c_int], c_int),
    SDLFunc("IMG_Quit"),
    SDLFunc("IMG_LoadTyped_RW", [_P(SDL_RWops), c_int, c_char_p], _P(SDL_Surface)),
    SDLFunc("IMG_Load", [c_char_p], _P(SDL_Surface)),
    SDLFunc("IMG_Load_RW", [_P(SDL_RWops), c_int], _P(SDL_Surface)),
    SDLFunc("IMG_LoadTexture", [_P(SDL_Renderer), c_char_p], _P(SDL_Texture)),
    SDLFunc("IMG_LoadTexture_RW", [_P(SDL_Renderer), _P(SDL_RWops), c_int], _P(SDL_Texture)),
    SDLFunc("IMG_LoadTextureTyped_RW", [_P(SDL_Renderer), _P(SDL_RWops), c_int, c_char_p], _P(SDL_Texture)),
    SDLFunc("IMG_isICO", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isCUR", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isBMP", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isGIF", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isJPG", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isLBM", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isPCX", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isPNG", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isPNM", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isSVG", [_P(SDL_RWops)], c_int, added='2.0.2'),
    SDLFunc("IMG_isTIF", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isXCF", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isXPM", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isXV", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_isWEBP", [_P(SDL_RWops)], c_int),
    SDLFunc("IMG_LoadICO_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadCUR_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadBMP_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadGIF_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadJPG_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadLBM_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadPCX_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadPNG_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadPNM_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadSVG_RW", [_P(SDL_RWops)], _P(SDL_Surface), added='2.0.2'),
    SDLFunc("IMG_LoadTGA_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadTIF_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadXCF_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadXPM_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadXV_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_LoadWEBP_RW", [_P(SDL_RWops)], _P(SDL_Surface)),
    SDLFunc("IMG_ReadXPMFromArray", [_P(c_char_p)], _P(SDL_Surface)),
    SDLFunc("IMG_SaveJPG", [_P(SDL_Surface), c_char_p, c_int], c_int, added='2.0.2'),
    SDLFunc("IMG_SaveJPG_RW", [_P(SDL_Surface), _P(SDL_RWops), c_int, c_int], c_int, added='2.0.2'),
    SDLFunc("IMG_SavePNG", [_P(SDL_Surface), c_char_p], c_int),
    SDLFunc("IMG_SavePNG_RW", [_P(SDL_Surface), _P(SDL_RWops), c_int], c_int),
]
_funcs = {}
for f in _funcdefs:
    _funcs[f.name] = _bind(f.name, f.args, f.returns, f.added)


# Python wrapper functions

def IMG_Linked_Version():
    """Gets the version of the dynamically-linked **SDL2_image** library.

    Returns:
        POINTER(:obj:`SDL_version`): A pointer to a structure containing the
        version of the SDL2_image library currently in use.

    """
    return _funcs["IMG_Linked_Version"]()

def IMG_Init(flags):
    """Initializes the SDL2_image library.
    
    Calling this function enables support for the JPEG, PNG, TIF, and/or WebP
    image formats as requested by the init flags. All other image formats can
    be loaded or used regardless of whether this has been called.

    The following init flags are supported:

    ====== =================
    Format Init flag
    ====== =================
    JPEG   ``IMG_INIT_JPG``
    PNG    ``IMG_INIT_PNG``
    TIFF   ``IMG_INIT_TIF``
    WebP   ``IMG_INIT_WEBP``
    ====== =================

    This can be called multiple times to enable support for these formats
    separately, or can initialize multiple formats at once by passing a set of
    flags as a bitwise OR. You can also call this function with 0 as a flag
    to check which image libraries have already been loaded, or to test
    whether a given image format is available on the current system::

       # Initialize JPEG and PNG support separately
       for flag in [IMG_INIT_JPG, IMG_INIT_PNG]:
           IMG_Init(flag)
           err = IMG_GetError() # check for any errors loading library
           if len(err):
               print(err)

       # Initialize JPEG and PNG support at the same time
       flags = IMG_INIT_JPG | IMG_INIT_PNG
       IMG_Init(flags)
       if IMG_Init(0) != flags: # verify both libraries loaded properly
           print(IMG_GetError())

    Args:
        flags (int): A bitwise OR'd set of the flags of the image formats to
            load support for.

    Returns:
        int: A bitmask of all the currently initialized image loaders.

    """
    return _funcs["IMG_Init"](flags)

def IMG_Quit():
    """De-initializes the SDL2_image library.
    
    Calling this function disables JPEG, PNG, TIF, and WEBP support and frees
    all associated memory. Once this has been called, you can re-initialize
    support for those image formats using :func:`IMG_Init` and the corresponding
    init flags.

    You only need to call this function once, no matter how many times
    :func:`IMG_Init` was called.

    """
    return _funcs["IMG_Quit"]()

def IMG_LoadTyped_RW(src, freesrc, type):
    """Loads an image file from an SDL2 file object as a specific format.
    
    This function allows you to explicitly specify the format type of the image
    to load. Here are the different possible image format strings:

    =============  ===========================
    Format String  Format Type
    =============  ===========================
    b"TGA"         TrueVision Targa
    b"CUR"         Windows Cursor
    b"ICO"         Windows Icon
    b"BMP"         Windows Bitmap
    b"GIF"         Graphics Interchange Format
    b"JPG"         JPEG
    b"LBM"         Interleaved Bitmap
    b"PCX"         IBM PC Paintbrush
    b"PNG"         Portable Network Graphics
    b"PNM"         Portable Anymap
    b"SVG"         Scalable Vector Graphics
    b"TIF"         Tagged Image File Format
    b"XCF"         GIMP native format
    b"XPM"         X11 Pixmap
    b"XV"          XV Thumbnail
    b"WEBP"        WebP
    =============  ===========================

    See :func:`IMG_Load` for more information.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the image.
        freesrc (int): If non-zero, the input file object will be closed and
            freed after it has been read.
        type (bytes): A bytestring indicating the image format with which the
            file object should be loaded.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to the new surface containing the
        image, or a null pointer if there was an error.

    """
    return _funcs["IMG_LoadTyped_RW"](src, freesrc, type)

def IMG_Load(file):
    """Loads an image file to a new surface.

    This can load all supported image files, including TGA as long as the
    filename ends with ".tga".

    It is best to call this outside of event loops and keep the loaded
    images around until you are really done with them, as disk speed and
    image conversion to a surface can be slow.

    Once you are done with a loaded image, you can call :func:`SDL_FreeSurface`
    on the returned surface pointer to free up the memory associated with it.

    If the image format supports a transparent pixel, SDL_image will set the
    colorkey for the surface. You can enable RLE acceleration on the surface
    afterwards by calling::

       SDL_SetColorKey(image, SDL_RLEACCEL, image.contents.format.colorkey)

    .. note::
       If the loader for the image's format requires initialization (e.g. PNG)
       and it is not already initialized, this function will attempt to load
       it automatically.

    Args:
        file (bytes): A UTF8-encoded bytestring containing the path to the 
            image file to load.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to the new surface containing the
        image, or a null pointer if there was an error.

    """
    return _funcs["IMG_Load"](file)

def IMG_Load_RW(src, freesrc):
    """Loads an image file from an SDL2 file object to a new surface.

    This can load all supported formats, *except* TGA. See :func:`IMG_Load` 
    for more information.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the image.
        freesrc (int): If non-zero, the input file object will be closed and
            freed after it has been read.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to the new surface containing the
        image, or a null pointer if there was an error.

    """
    return _funcs["IMG_Load_RW"](src, freesrc)

def IMG_LoadTexture(renderer, file):
    """Loads an image file to a new texture using a given renderer.
    
    This function can load all supported image files, including TGA as long as
    the filename ends with ".tga".

    It is best to call this outside of event loops and keep the loaded
    images around until you are really done with them, as disk speed and
    image conversion to a texture can be slow.

    Once you are done with a loaded image, you can call
    :func:`SDL_DestroyTexture` on the returned texture pointer to free up the
    memory associated with it.

    .. note::
       If the image loader for the format of the given image requires
       initialization (e.g. PNG) and it is not already initialized, this
       function will attempt to load it automatically.

    Args:
        renderer (:obj:`SDL_Renderer`): The SDL rendering context with which to
            create the texture.
        file (bytes): A UTF8-encoded bytestring containing the path to the 
            image file to load.

    Returns:
        POINTER(:obj:`SDL_Texture`): A pointer to the new texture containing
        the image, or a null pointer if there was an error.

    """
    return _funcs["IMG_LoadTexture"](renderer, file)

def IMG_LoadTexture_RW(renderer, src, freesrc):
    """Loads an image from a file object to a texture using a given renderer.
    
    This function can load all supported formats, *except* TGA. See
    :func:`IMG_LoadTexture` for more information.

    Args:
        renderer (:obj:`SDL_Renderer`): The SDL rendering context with which to
            create the texture.
        src (:obj:`SDL_RWops`): The file object from which to load the image.
        freesrc (int): If non-zero, the input file object will be closed and
            freed after it has been read.

    Returns:
        POINTER(:obj:`SDL_Texture`): A pointer to the new texture containing
        the image, or a null pointer if there was an error.

    """
    return _funcs["IMG_LoadTexture_RW"](renderer, src, freesrc)

def IMG_LoadTextureTyped_RW(renderer, src, freesrc, type):
    """Loads an image file from a file object to a texture as a specific format.

    This function allows you to explicitly specify the format type of the image
    to load. The different possible format strings are listed in the
    documentation for :func:`IMG_LoadTyped_RW`.

    See :func:`IMG_LoadTexture` for more information.

    Args:
        renderer (:obj:`SDL_Renderer`): The SDL rendering context with which to
            create the texture.
        src (:obj:`SDL_RWops`): The file object from which to load the image.
        freesrc (int): If non-zero, the input file object will be closed and
            freed after it has been read.
        type (bytes): A bytestring indicating the image format with which the
            file object should be loaded.

    Returns:
        POINTER(:obj:`SDL_Texture`): A pointer to the new texture containing
        the image, or a null pointer if there was an error.

    """
    return _funcs["IMG_LoadTextureTyped_RW"](renderer, src, freesrc, type)


def IMG_isICO(src):
    """Tests whether a file object contains an ICO (Windows icon) image.

    The ICO format 

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if BMPs are supported and file is a valid ICO, otherwise 0.

    """
    return _funcs["IMG_isICO"](src)

def IMG_isCUR(src):
    """Tests whether a file object contains a CUR (Windows cursor) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if BMPs are supported and file is a valid CUR, otherwise 0.

    """
    return _funcs["IMG_isCUR"](src)

def IMG_isBMP(src):
    """Tests whether a file object contains a BMP (Windows bitmap) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if BMPs are supported and file is a valid BMP, otherwise 0.

    """
    return _funcs["IMG_isBMP"](src)

def IMG_isGIF(src):
    """Tests whether a file object contains a GIF image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if GIFs are supported and file is a valid GIF, otherwise 0.

    """
    return _funcs["IMG_isGIF"](src)

def IMG_isJPG(src):
    """Tests whether a file object contains a JPEG image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if JPEGs are supported and file is a valid JPEG, otherwise 0.

    """
    return _funcs["IMG_isJPG"](src)

def IMG_isLBM(src):
    """Tests whether a file object contains an LBM (Interleaved Bitmap) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if LBMs are supported and file is a valid LBM, otherwise 0.

    """
    return _funcs["IMG_isLBM"](src)

def IMG_isPCX(src):
    """Tests whether a file object contains a PCX (IBM PC Paintbrush) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if PCXs are supported and file is a valid PCX, otherwise 0.

    """
    return _funcs["IMG_isPCX"](src)

def IMG_isPNG(src):
    """Tests whether a file object contains a PNG image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if PNGs are supported and file is a valid PNG, otherwise 0.

    """
    return _funcs["IMG_isPNG"](src)

def IMG_isPNM(src):
    """Tests whether a file object contains a PNM (Portable Anymap) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if PNMs are supported and file is a valid PNM, otherwise 0.

    """
    return _funcs["IMG_isPNM"](src)

def IMG_isSVG(src):
    """Tests whether a file object contains an SVG image.

    .. note:: This function is only available in SDL_image 2.0.2 or later.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if SVGs are supported and file is a valid SVG, otherwise 0.

    """
    return _funcs["IMG_isSVG"](src)

def IMG_isTIF(src):
    """Tests whether a file object contains a TIFF image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if TIFFs are supported and file is a valid TIFF, otherwise 0.

    """
    return _funcs["IMG_isTIF"](src)

def IMG_isXCF(src):
    """Tests whether a file object contains an XCF (native GIMP format) image.

    .. note:: XCF support is currently missing in official macOS binaries

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if XCFs are supported and file is a valid XCF, otherwise 0.

    """
    return _funcs["IMG_isXCF"](src)

def IMG_isXPM(src):
    """Tests whether a file object contains an XPM (X11 Pixmap) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if XPMs are supported and file is a valid XPM, otherwise 0.

    """
    return _funcs["IMG_isXPM"](src)

def IMG_isXV(src):
    """Tests whether a file object contains an XV (XV Visual Schnauzer) image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if XVs are supported and file is a valid XV, otherwise 0.

    """
    return _funcs["IMG_isXV"](src)

def IMG_isWEBP(src):
    """Tests whether a file object contains a WebP image.

    Args:
        src (:obj:`SDL_RWops`): The file object to check.

    Returns:
        int: 1 if WebPs are supported and file is a valid WebP, otherwise 0.

    """
    return _funcs["IMG_isWEBP"](src)


def IMG_LoadICO_RW(src):
    """Loads an ICO (Windows icon) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the ICO.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadICO_RW"](src)

def IMG_LoadCUR_RW(src):
    """Loads a CUR (Windows cursor) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the CUR.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadCUR_RW"](src)

def IMG_LoadBMP_RW(src):
    """Loads a BMP (Windows bitmap) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the BMP.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadBMP_RW"](src)

def IMG_LoadGIF_RW(src):
    """Loads a GIF (Graphics Interchange Format) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the GIF.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadGIF_RW"](src)

def IMG_LoadJPG_RW(src):
    """Loads a JPEG image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the JPEG.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadJPG_RW"](src)

def IMG_LoadLBM_RW(src):
    """Loads an LBM (Interleaved Bitmap) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the LBM.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadLBM_RW"](src)

def IMG_LoadPCX_RW(src):
    """Loads a PCX (IBM PC Paintbrush) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the PCX.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadPCX_RW"](src)

def IMG_LoadPNG_RW(src):
    """Loads a PNG image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the PNG.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadPNG_RW"](src)

def IMG_LoadPNM_RW(src):
    """Loads a PNM (Portable Anymap) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the PNM.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadPNM_RW"](src)

def IMG_LoadSVG_RW(src):
    """Loads an SVG image from an SDL file object.

    .. note:: This function is only available in SDL_image 2.0.2 or later.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the SVG.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadSVG_RW"](src)

def IMG_LoadTGA_RW(src):
    """Loads a TGA (TrueVision Targa) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the TGA.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadTGA_RW"](src)

def IMG_LoadTIF_RW(src):
    """Loads a TIFF (Tagged Image File Format) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the TIFF.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadTIF_RW"](src)

def IMG_LoadXCF_RW(src):
    """Loads an TIFF (native GIMP format) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the XCF.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadXCF_RW"](src)

def IMG_LoadXPM_RW(src):
    """Loads an XPM (X11 Pixmap) image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the XPM.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadXPM_RW"](src)

def IMG_LoadXV_RW(src):
    """Loads an XV thumbnail (XV Visual Schnauzer) from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the XV.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadXV_RW"](src)

def IMG_LoadWEBP_RW(src):
    """Loads a WebP image from an SDL file object.

    Use the :func:`IMG_GetError` function to check for any errors.

    Args:
        src (:obj:`SDL_RWops`): The file object from which to load the WebP.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_LoadWEBP_RW"](src)


def IMG_ReadXPMFromArray(xpm):
    """Loads an X11 Pixmap from an array to a new surface.
    
    The XPM format consists of a C header with an array of strings defining the
    dimensions, colors, and pixels of a pixel art image: this is the data format
    that this function expects to be passed. 

    The input string for this function must be cast to a ``ctypes.c_char_p``
    (pointer to a character array) and passed by reference using
    :func:`ctypes.byref`.

    Args:
        xpm (byref(:obj:`ctypes.c_char_p`)): A pointer to a ctypes string
            defining an XPM image.

    Returns:
        POINTER(:obj:`SDL_Surface`): A pointer to a new surface containing the
        image, or ``None`` if there was an error.

    """
    return _funcs["IMG_ReadXPMFromArray"](xpm)


def IMG_SavePNG(surface, file):
    """Saves an SDL surface to a PNG file.

    .. note::
       This should work regardless of whether PNG support was
       successfully initialized with :func:`IMG_Init`, but the full set
       of PNG features may not be available.

    Args:
        surface (:obj:`SDL_Surface`): The surface to be saved to PNG.
        file (bytes): A UTF-8 encoded bytestring containing the path at which
            to save the PNG.

    Returns:
        int: 0 on success or a negative error code on failure, can call
        :func:`IMG_GetError` for more information.

    """
    return _funcs["IMG_SavePNG"](surface, file)

def IMG_SavePNG_RW(surface, dst, freedst):
    """Saves an SDL surface to a file object containing a PNG.

    See :func:`IMG_SavePNG` for more information.

    Args:
        surface (:obj:`SDL_Surface`): The surface to be saved to PNG.
        dst (:obj:`SDL_RWops`): The destination file object for the saved PNG.
        freedst (int): If non-zero, the destination file object will be closed
            once the PNG has been written.

    Returns:
        int: 0 on success or a negative error code on failure, can call
        :func:`IMG_GetError` for more information.

    """
    return _funcs["IMG_SavePNG_RW"](surface, dst, freedst)

def IMG_SaveJPG(surface, file, quality):
    """Saves an SDL surface to a JPEG file at a given quality.

    JPEG support must be already initialized using :func:`IMG_Init` before this
    function can be used. Otherwise, this function will fail without an explicit
    error that can be retrieved with :func:`IMG_GetError`.

    .. note::
       As of SDL_image 2.0.5, JPEG saving is not supported by the official
       libsdl.org macOS binaries.

    Args:
        surface (:obj:`SDL_Surface`): The surface to be saved to JPEG.
        file (bytes): A UTF-8 encoded bytestring containing the path at which
            to save the PNG.
        quality (int): The compression level of the saved JPEG, from 1
            (lowest quality) to 100 (highest quality).

    Returns:
        int: 0 on success or a negative error code on failure, can call
        :func:`IMG_GetError` for more information.

    """
    # NOTE: Not available in official macOS binaries
    return _funcs["IMG_SaveJPG"](surface, file, quality)

def IMG_SaveJPG_RW(surface, dst, freedst, quality):
    """Saves an SDL surface to a file object containing a JPEG.

    .. note::
       As of SDL_image 2.0.5, JPEG saving is not supported by the official
       libsdl.org macOS binaries.

    Args:
        surface (:obj:`SDL_Surface`): The surface to be saved to JPEG.
        file (bytes): A UTF-8 encoded bytestring containing the path at which
            to save the PNG.
        quality (int): The compression level of the saved JPEG, from 1
            (lowest quality) to 100 (highest quality).

    Returns:
        int: 0 on success or a negative error code on failure, can call
        :func:`IMG_GetError` for more information.

    """
    # NOTE: Not available in official macOS binaries
    return _funcs["IMG_SaveJPG_RW"](surface, dst, freedst, quality)


IMG_SetError = SDL_SetError
IMG_GetError = SDL_GetError
