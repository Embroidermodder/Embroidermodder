.. currentmodule:: sdl2.sdlimage

sdl2.sdlimage - Python bindings for SDL_image
=============================================

py-sdl2 provides bindings for SDL2_image, a library designed for use with SDL2
that adds support for loading a wide range of different common (and uncommon)
image formats for easy use with SDL2. In addition, SDL2_image includes functions
for saving :obj:`SDL_Surface` objects to PNG and/or JPEG images.

.. contents:: Table Of Contents
   :depth: 1
   :local:

.. note::
   If using an alternative rendering system that doesn't use SDL surfaces as
   input (e.g. PyOpenGL), the Pillow imaging library may be a better fit for
   your project.

Initialization and library information functions
------------------------------------------------

.. autofunction:: IMG_Init

.. autofunction:: IMG_Quit

.. function:: IMG_GetError()

   Returns the most recently encountered SDL2 error message, if any.

   This function is a simple wrapper around :func:`SDL_GetError`.

   :retuns: A UTF-8 encoded string describing the most recent SDL2 error.
   :rtype: bytes


.. function:: IMG_SetError(fmt)

   Sets the most recent SDL2 error message to a given string.

   This function is a simple wrapper around :func:`SDL_SetError`.

   :param fmt: A UTF-8 encoded string containing the error message to set.
   :type fmt: bytes

   :retuns: Always returns ``-1``.
   :rtype: int


.. autofunction:: IMG_Linked_Version


Image format-checking functions
-------------------------------

These functions are used to check whether an SDL file object
(:obj:`SDL_RWops`) is a valid image file of a given format. Note that
all of these functions will return 0 if SDL2_image was not built with
support for that format, even if it is a valid image of that type, so be
cautious when using these for formats like WEBP, JPEG, PNG, and TIFF, which
are optional when building SDL2_image.

.. autofunction:: IMG_isICO

.. autofunction:: IMG_isCUR

.. autofunction:: IMG_isBMP

.. autofunction:: IMG_isGIF

.. autofunction:: IMG_isJPG

.. autofunction:: IMG_isLBM

.. autofunction:: IMG_isPCX

.. autofunction:: IMG_isPNG

.. autofunction:: IMG_isPNM

.. autofunction:: IMG_isSVG

.. autofunction:: IMG_isTIF

.. autofunction:: IMG_isXCF

.. autofunction:: IMG_isXPM

.. autofunction:: IMG_isXV

.. autofunction:: IMG_isWEBP


General image loading functions
-------------------------------

.. autofunction:: IMG_Load

.. autofunction:: IMG_Load_RW

.. autofunction:: IMG_LoadTyped_RW

.. autofunction:: IMG_LoadTexture

.. autofunction:: IMG_LoadTexture_RW

.. autofunction:: IMG_LoadTextureTyped_RW


Format-specific image loading functions
---------------------------------------

.. autofunction:: IMG_LoadICO_RW

.. autofunction:: IMG_LoadCUR_RW

.. autofunction:: IMG_LoadBMP_RW

.. autofunction:: IMG_LoadGIF_RW

.. autofunction:: IMG_LoadJPG_RW

.. autofunction:: IMG_LoadLBM_RW

.. autofunction:: IMG_LoadPCX_RW

.. autofunction:: IMG_LoadPNG_RW

.. autofunction:: IMG_LoadPNM_RW

.. autofunction:: IMG_LoadSVG_RW

.. autofunction:: IMG_LoadTGA_RW

.. autofunction:: IMG_LoadTIF_RW

.. autofunction:: IMG_LoadXCF_RW

.. autofunction:: IMG_LoadXPM_RW

.. autofunction:: IMG_LoadXV_RW

.. autofunction:: IMG_LoadWEBP_RW

.. autofunction:: IMG_ReadXPMFromArray


Image writing functions
-----------------------

.. autofunction:: IMG_SavePNG

.. autofunction:: IMG_SavePNG_RW

.. autofunction:: IMG_SaveJPG

.. autofunction:: IMG_SaveJPG_RW


Module constants
----------------

.. data:: IMG_MAJOR_VERSION

    Latest SDL2_image library major number supported by PySDL2.

.. data:: IMG_MINOR_VERSION

    Latest SDL2_image library minor number supported by PySDL2.

.. data:: IMG_PATCHLEVEL

    Latest SDL2_image library patch level number supported by PySDL2.

.. data:: IMG_INIT_JPG

    :func:`IMG_Init` flag to enable support for the JPEG image format.

.. data:: IMG_INIT_PNG

    :func:`IMG_Init` flag to enable support for the PNG image format.

.. data:: IMG_INIT_TIF

    :func:`IMG_Init` flag to enable support for the TIFF image format.

.. data:: IMG_INIT_WEBP

    :func:`IMG_Init` flag to enable support for the WEBP image format.
