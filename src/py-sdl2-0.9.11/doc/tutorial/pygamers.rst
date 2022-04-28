PySDL2 for Pygamers
===================

Care to move to a newer SDL with your Pygame knowledge? Then you should
know one thing or two about PySDL2 before hacking code, since it is
completely different from Pygame. Do not let that fact scare you away,
the basics with graphics and sound are still the same (as they are
fundamental), but you will not find many similarities to the Pygame API
within PySDL2.

.. todo::

   More details, examples, etc.

Technical differences
---------------------
Pygame is implemented as a mixture of Python, C and Assembler code,
wrapping 3rd party libraries with CPython API interfaces. PySDL2 in
contrast is written in pure Python, using :mod:`ctypes` to interface
with the C interfaces of 3rd party libraries.

API differences
---------------

.. _pygamers_pygame:

pygame
^^^^^^
======================= =================================================
pygame                  sdl2
======================= =================================================
``init()``              :func:`sdl2.SDL_Init()` where appropriate
``quit()``              :func:`sdl2.SDL_Quit()` where appropriate
``error``               No equivalent
``get_error()``         :func:`sdl2.SDL_GetError()`
``set_error()``         :func:`sdl2.SDL_SetError()`
``get_sdl_version()``   :func:`sdl2.SDL_GetVersion()`
``get_sdl_byteorder()`` :data:`sdl2.SDL_BYTEORDER`
``register_quit()``     No equivalent planned
``encode_string()``     No equivalent planned
``encode_file_path()``  No equivalent planned
======================= =================================================

:func:`sdl2.ext.init()` initializes only the video subsystem. By comparison,
:func:`pygame.init()` initializes all Pygame submodules (which includes
initializing other SDL subsystems).

pygame.cdrom
^^^^^^^^^^^^
PySDL2 does not feature any CD-ROM related interfaces. They were
removed in SDL2 and PySDL2 does not provide its own facilities.

pygame.Color
^^^^^^^^^^^^
You can find a similar class in :class:`sdl2.ext.Color`. It does
not feature a ``set_length()`` or ``correct_gamma()`` method, though.

pygame.cursors
^^^^^^^^^^^^^^
PySDL2 does not feature any pre-defined cursor settings at the moment.

pygame.display
^^^^^^^^^^^^^^
======================= =================================================
pygame.display          sdl2
======================= =================================================
``init()``              :func:`sdl2.ext.init()`
``quit()``              :func:`sdl2.ext.quit()`
``get_init()``          :func:`sdl2.SDL_WasInit()`
``set_mode()``          :class:`sdl2.ext.Window`
``get_surface()``       :meth:`sdl2.ext.Window.get_surface()`
``flip()``              :meth:`sdl2.ext.Window.refresh()`
``update()``            :meth:`sdl2.ext.Window.refresh()`
``get_driver()``        :func:`sdl2.SDL_GetCurrentVideoDriver()`
``Info``                No equivalent
``get_wm_info()``       :func:`sdl2.SDL_GetWindowWMInfo()`
``list_modes()``        :func:`sdl2.SDL_GetNumDisplayModes()`
``mode_ok()``           :func:`sdl2.SDL_GetClosestDisplayMode()`
``gl_get_attribute()``  :func:`sdl2.SDL_GL_GetAttribute()`
``gl_set_attribute()``  :func:`sdl2.SDL_GL_SetAttribute()`
``get_active()``        No equivalent
``iconify()``           :meth:`sdl2.ext.Window.minimize()`
``toggle_fullscreen()`` :func:`sdl2.SDL_SetWindowFullscreen()`
``set_gamma()``         :func:`sdl2.SDL_SetWindowBrightness()`
``set_gamma_ramp()``    :func:`sdl2.SDL_SetWindowGammaRamp.()`
``set_icon()``          :func:`sdl2.SDL_SetWindowIcon()`
``set_caption()``       :attr:`sdl2.ext.Window.title`
``get_caption()``       :attr:`sdl2.ext.Window.title`
``set_palette()``       :func:`sdl2.SDL_SetSurfacePalette()`
======================= =================================================

pygame.draw
^^^^^^^^^^^
Drawing primitives can be accessed through either the
``sdl2.SDL_RenderDraw*()`` and ``sdl2.SDL_RenderFill*()`` functions or
the more powerful :mod:`sdl2.sdlgfx` module,

pygame.event
^^^^^^^^^^^^
================= =================================================
pygame.event      sdl2
================= =================================================
``pump()``        :func:`sdl2.SDL_PumpEvents()`
``get()``         :func:`sdl2.SDL_PollEvent()` or :func:`sdl2.ext.get_events()`
``poll()``        :func:`sdl2.SDL_PollEvent()`
``wait()``        :func:`sdl2.SDL_WaitEvent()`
``peek()``        :func:`sdl2.SDL_PeepEvents()`
``clear()``       :func:`sdl2.SDL_FlushEvents()`
``event_name()``  No equivalent
``set_blocked()`` :func:`sdl2.SDL_EventState()`
``get_blocked()`` :func:`sdl2.SDL_EventState()`
``set_allowed()`` :func:`sdl2.SDL_EventState()`
``set_grab()``    :func:`sdl2.SDL_SetWindowGrab()`
``get_grab()``    :func:`sdl2.SDL_GetWindowGrab()`
``post()``        :func:`sdl2.SDL_PeepEvents()`
``Event``         :class:`sdl2.SDL_Event`
================= =================================================

pygame.font
^^^^^^^^^^^
====================== =================================================
pygame.font            sdl2
====================== =================================================
``init()``             :func:`sdl2.sdlttf.TTF_Init()`
``quit()``             :func:`sdl2.sdlttf.TTF_Quit()`
``get_init()``         :func:`sdl2.sdlttf.TTF_WasInit()`
``get_default_font()`` No equivalent planned [#f1]_
``get_fonts()``        No equivalent planned [#f1]_
``match_font()``       No equivalent planned [#f1]_
``SysFont``            No equivalent planned [#f1]_
``Font``               No equivalent planned [#f1]_
====================== =================================================

pygame.freetype
^^^^^^^^^^^^^^^
PySDL2 does not feature direct FreeType support.

pygame.gfxdraw
^^^^^^^^^^^^^^
PySDL2 offers SDL_gfx support through the :mod:`sdl2.sdlgfx` module.

pygame.image
^^^^^^^^^^^^
================== =================================================
pygame.image       sdl2
================== =================================================
``load()``         :func:`sdl2.sdlimage.IMG_Load()`,
                   :func:`sdl2.ext.load_image()`
``save()``         :func:`sdl2.surface.SDL_SaveBMP()`,
                   :func:`sdl2.sdlimage.IMG_SavePNG()`
``get_extended()`` :func:`sdl2.sdlimage.IMG_isBMP()` et al.
``tostring()``     No equivalent yet
``fromstring()``   No equivalent yet
``frombuffer()``   No equivalent yet
================== =================================================

pygame.joystick
^^^^^^^^^^^^^^^
================== ========================================================
pygame.joystick    sdl2
================== ========================================================
``init()``         :func:`sdl2.SDL_Init()`
``quit()``         :func:`sdl2.SDL_Quit()`
``get_init()``     :func:`sdl2.SDL_WasInit()`
``get_count()``    :func:`sdl2.joystick.SDL_NumJoysticks()`
``Joystick()``     :class:`sdl2.joystick.SDL_Joystick` and related
                   functions
================== ========================================================

pygame.key
^^^^^^^^^^
================== ========================================================
pygame.key         sdl2
================== ========================================================
``get_focused()``  :func:`sdl2.keyboard.SDL_GetKeyboardFocus()`
``get_pressed()``  :func:`sdl2.keyboard.SDL_GetKeyboardState()`
``get_mods()``     :func:`sdl2.keyboard.SDL_GetModState()`
``set_mods()``     :func:`sdl2.keyboard.SDL_SetModState()`
``set_repeat()``   Based on the OS/WM settings, no equivalent
``get_repeat()``   Based on the OS/WM settings, no equivalent
``name()``         :func:`sdl2.keyboard.SDL_GetKeyName()`
================== ========================================================

pygame.locals
^^^^^^^^^^^^^
Constants in PySDL2 are spread across the different packages and
modules, depending on where they originate from.

pygame.mixer
^^^^^^^^^^^^
====================== ====================================================
pygame.mixer           sdl2
====================== ====================================================
``init()``             :func:`sdl2.sdlmixer.Mix_Init()`
``quit()``             :func:`sdl2.sdlmixer.Mix_Quit()`
``get_init()``         No equivalent planned
``stop()``             :func:`sdl2.sdlmixer.Mix_HaltChannel()`,
                       :func:`sdl2.sdlmixer.Mix_HaltGroup()`,
                       :func:`sdl2.sdlmixer.Mix_HaltMusic()`
``pause()``            :func:`sdl2.sdlmixer.Mix_Pause()`,
                       :func:`sdl2.sdlmixer.Mix_PauseMusic()`
``unpause()``          :func:`sdl2.sdlmixer.Mix_Resume()`,
                       :func:`sdl2.sdlmixer.Mix_ResumeMusic()`
``fadeout()``          :func:`sdl2.sdlmixer.Mix_FadeOutChannel()`,
                       :func:`sdl2.sdlmixer.Mix_FadeOutGroup()`,
                       :func:`sdl2.sdlmixer.Mix_FadeOutMusic()`
``set_num_channels()`` :func:`sdl2.sdlmixer.Mix_AllocateChannels()`
``get_num_channels()`` :func:`sdl2.sdlmixer.Mix_AllocateChannels()`
``set_reserved()``     :func:`sdl2.sdlmixer.Mix_ReserveChannels()`
``find_channel()``     No equivalent planned
``get_busy()``         :func:`sdl2.sdlmixer.Mix_ChannelFinished`
``Sound``              :class:`sdl2.sdlmixer.Mix_Chunk`
``Channel``            No equivalent, use the channel functions instead
====================== ====================================================

pygame.mixer.music
^^^^^^^^^^^^^^^^^^
See `pygame.mixer`_.

pygame.mouse
^^^^^^^^^^^^
================= ====================================================
pygame.mouse      sdl2
================= ====================================================
``get_pressed()`` :func:`sdl2.mouse.SDL_GetMouseState()`
``get_pos()``     :func:`sdl2.mouse.SDL_GetMouseState()`
``get_rel()``     :func:`sdl2.mouse.SDL_GetRelativeMouseState()`
``set_pos()``     :func:`sdl2.mouse.SDL_WarpMouseInWindow()`
``set_visible()`` :func:`sdl2.mouse.SDL_ShowCursor()`
``get_focused()`` :func:`sdl2.mouse.SDL_GetMouseFocus()`
``set_cursor()``  :func:`sdl2.mouse.SDL_GetCursor()`
``get_cursor()``  :func:`sdl2.mouse.SDL_SetCursor()`
================= ====================================================

pygame.movie
^^^^^^^^^^^^
No such module is planned for PySDL2.

pygame.Overlay
^^^^^^^^^^^^^^
You can work with YUV overlays by using the :mod:`sdl2.render` module
with :class:`sdl2.render.SDL_Texture` objects.

pygame.PixelArray
^^^^^^^^^^^^^^^^^
You can access pixel data of sprites and surfaces directly via the
:class:`sdl2.ext.PixelView` class. It does not feature comparison or
extractions methods.

pygame.Rect
^^^^^^^^^^^
No such functionality is available for PySDL2. Rectangles are represented
via :class:`sdl2.rect.SDL_Rect` for low-level SDL2 wrappers or 4-value
tuples.

pygame.scrap
^^^^^^^^^^^^
PySDL2 offers basic text-based clipboard access via the
:mod:`sdl2.clipboard` module. A feature-rich clipboard API as for Pygame
does not exist yet.

pygame.sndarray
^^^^^^^^^^^^^^^
No such module is available for PySDL2 yet.

pygame.sprite
^^^^^^^^^^^^^
PySDL2 uses a different approach of rendering and managing sprite
objects via a component-based system and the :class:`sdl2.ext.Sprite`
class. A sprite module as for Pygame is not planned.

pygame.Surface
^^^^^^^^^^^^^^
======================= =====================================================
pygame.Surface          sdl2
======================= =====================================================
``blit()``              :meth:`sdl2.surface.SDL_BlitSurface()`,
                        :class:`sdl2.ext.SpriteRenderSystem`
``convert()``           :func:`sdl2.surface.SDL_ConvertSurface()`
``convert_alpha()``     :func:`sdl2.surface.SDL_ConvertSurface()`
``copy()``              :func:`sdl2.surface.SDL_ConvertSurface()`
``fill()``              :func:`sdl2.surface.SDL_FillRect()`,
                        :func:`sdl2.surface.SDL_FillRects()`,
                        :func:`sdl2.ext.fill()`
``scroll()``            No equivalent planned
``set_colorkey()``      :func:`sdl2.surface.SDL_SetColorKey()`
``get_colorkey()``      :func:`sdl2.surface.SDL_GetColorKey()`
``set_alpha()``         :func:`sdl2.surface.SDL_SetSurfaceAlphaMod()`
``get_alpha()``         :func:`sdl2.surface.SDL_GetSurfaceAlphaMod()`
``lock()``              :func:`sdl2.surface.SDL_LockSurface()`
``unlock()``            :func:`sdl2.surface.SDL_UnlockSurface()`
``mustlock()``          :func:`sdl2.surface.SDL_MUSTLOCK()`
``get_locked()``        :attr:`sdl2.surface.SDL_Surface.locked`
``get_locks()``         No equivalent planned
``get_at()``            Direct access to the pixels for surfaces can be
                        achieved via the :class:`sdl2.ext.PixelView` class
``set_at()``            Direct access to the pixels for surfaces can be
                        achieved via the :class:`sdl2.ext.PixelView` class
``get_at_mapped()``     No equivalent planned
``get_palette()``       via :attr:`sdl2.surface.SDL_Surface.format` and the
                        :attr:`sdl2.pixels.SDL_PixelFormat.palette`
                        attribute
``get_palette_at()``    ``sdl2.pixels.SDL_Palette.colors[offset]``
``set_palette()``       :func:`sdl2.surface.SDL_SetSurfacePalette()`
``set_palette_at()``    ``sdl2.pixels.SDL_Palette.colors[offset]``
``map_rgb()``           :func:`sdl2.pixels.SDL_MapRGB()`
``unmap_rgb()``         :func:`sdl2.pixels.SDL_GetRGB()`
``set_clip()``          :func:`sdl2.surface.SDL_SetClipRect()`
``get_clip()``          :func:`sdl2.surface.SDL_GetClipRect()`
``subsurface()``        :func:`sdl2.ext.subsurface()`
``get_parent()``        No equivalent yet
``get_abs_parent()``    As for ``get_parent``
``get_offset()``        As for ``get_parent``
``get_abs_offset()``    As for ``get_parent``
``get_size()``          :attr:`sdl2.ext.Sprite.size`,
                        :attr:`sdl2.surface.SDL_Surface.w`,
                        :attr:`sdl2.surface.SDL_Surface.h`
``get_width()``         ``sdl2.ext.Sprite.size[0]``,
                        :attr:`sdl2.surface.SDL_Surface.w`,
``get_height()``        ``sdl2.ext.Sprite.size[1]``,
                        :attr:`sdl2.surface.SDL_Surface.h`
``get_rect()``          No equivalent planned
``get_bitsize()``       :attr:`sdl2.pixels.SDL_PixelFormat.BitsPerPixel`
``get_bytesize()``      :attr:`sdl2.pixels.SDL_PixelFormat.BytesPerPixel`
``get_flags()``         :attr:`sdl2.surface.SDL_Surface.flags`
``get_pitch()``         :attr:`sdl2.surface.SDL_Surface.pitch`
``get_masks()``         :attr:`sdl2.pixels.SDL_PixelFormat.Rmask`, ...
``get_shifts()``        :attr:`sdl2.pixels.SDL_PixelFormat.Rshift`, ...
``get_losses()``        :attr:`sdl2.pixels.SDL_PixelFormat.Rloss`, ...
``get_bounding_rect()`` No equivalent planned
``get_view()``          :class:`sdl2.ext.PixelView`
``get_buffer()``        :class:`sdl2.ext.PixelView` or
                        :attr:`sdl2.surface.SDL_Surface.pixels`
======================= =====================================================

pygame.surfarray
^^^^^^^^^^^^^^^^
2D and 3D pixel access can be achieved via the
:class:`sdl2.ext.PixelView` class in environments without
numpy. Simplified numpy-array creation with direct pixel access (similar
to ``pygame.surfarray.pixels2d()`` and ``pygame.surfarray.pixels3d()``)
is available via :func:`sdl2.ext.pixels2d()` and
:func:`sdl2.ext.pixels3d()`.

pygame.time
^^^^^^^^^^^
=============== =================================================
pygame.time     sdl2
=============== =================================================
``get_ticks()`` :func:`sdl2.timer.SDL_GetTicks()`
``wait()``      :func:`sdl2.timer.SDL_Delay()`
``delay()``     :func:`sdl2.timer.SDL_Delay()`
``Clock``       No equivalent planned
=============== =================================================

pygame.transform
^^^^^^^^^^^^^^^^
The are no transformation helpers in PySDL2 at moment. Those might be
implemented later on via numpy helpers, the Python Imaging Library or
other 3rd party packages.

pygame.version
^^^^^^^^^^^^^^
=============== =================================================
pygame.version  sdl2
=============== =================================================
``ver``         :attr:`sdl2.__version__`
``vernum``      :attr:`sdl2.version_info`
=============== =================================================

.. rubric:: Footnotes

.. [#f1] Check https://bitbucket.org/marcusva/python-utils for an easy
         to use system font detection module
