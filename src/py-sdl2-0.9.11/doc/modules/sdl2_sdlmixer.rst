.. currentmodule:: sdl2.sdlmixer

sdl2.sdlmixer - Python bindings for SDL_mixer
=============================================

The :mod:`sdl2.sdlmixer` module provides bindings for wrapper the SDL_mixer
audio playback library. It supports loading, playing, and mixing audio from a
wide range of popular formats (e.g. WAV, FLAC, MP3, OGG).

The SDL_mixer library is designed around the concept of virtual channels, each
of which can be set to different volumes, have different left/right panning, and
play different effects and sounds simultaneously (like a mixing board in a
recording studio). In addition to the main mixing channels, SDL_mixer also
provides a single channel for music playback, which is less flexible than the
other channels but supports a wider range of playback formats (including MIDI)
and requires less memory for playback.

Music playback is performed using :class:`Mix_Music` objects and their
corresponding functions. All other audio playback is performed using
:class:`Mix_Chunk` objects.

.. contents:: Table Of Contents
   :depth: 2
   :local:

.. note::
   The PySDL2 documentation for this module is currently a work-in-progress, and
   not all functions are documented. The official C documentation (linked below)
   can be used in the meantime to fill in any gaps.

The official documentation for the SDL_mixer library can be found `here
<http://www.libsdl.org/projects/SDL_mixer/docs/index.html>`_. Note that
not all functions may be documented, as the official doucmentation is
currently several releases out-of-date.


Main data types
---------------

.. autoclass:: Mix_Chunk

.. autoclass:: Mix_Music


Initialization and library information functions
------------------------------------------------

.. autofunction:: Mix_Init

.. autofunction:: Mix_Quit

.. autofunction:: Mix_OpenAudio

.. autofunction:: Mix_OpenAudioDevice

.. autofunction:: Mix_CloseAudio

.. function:: Mix_GetError()

   Returns the most recently encountered SDL2 error message, if any.

   This function is a simple wrapper around :func:`SDL_GetError`.

   :retuns: A UTF-8 encoded string describing the most recent SDL2 error.
   :rtype: bytes

.. function:: Mix_SetError(fmt)

   Sets the most recent SDL2 error message to a given string.

   This function is a simple wrapper around :func:`SDL_SetError`.

   :param fmt: A UTF-8 encoded string containing the error message to set.
   :type fmt: bytes

   :retuns: Always returns ``-1``.
   :rtype: int

.. autofunction:: Mix_QuerySpec

.. autofunction:: Mix_Linked_Version


Audio loading and closing functions
-----------------------------------

Loading and closing audio clips
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_LoadWAV

.. autofunction:: Mix_LoadWAV_RW

.. autofunction:: Mix_QuickLoad_WAV

.. autofunction:: Mix_QuickLoad_RAW

.. autofunction:: Mix_FreeChunk


Loading and closing music
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_LoadMUS

.. autofunction:: Mix_LoadMUS_RW

.. autofunction:: Mix_LoadMUSType_RW

.. autofunction:: Mix_FreeMusic


Decoder availability & info functions
-------------------------------------

.. autofunction:: Mix_GetNumChunkDecoders

.. autofunction:: Mix_GetChunkDecoder

.. autofunction:: Mix_HasChunkDecoder

.. autofunction:: Mix_GetNumMusicDecoders

.. autofunction:: Mix_GetMusicDecoder

.. autofunction:: Mix_GetMusicType


Channel functions
-----------------

These functions affect regular mixer channels. Music is not affected by these functions.

Channel configuration
^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_AllocateChannels

.. autofunction:: Mix_ReserveChannels

.. autofunction:: Mix_ChannelFinished


Channel grouping
^^^^^^^^^^^^^^^^

.. autofunction:: Mix_GroupChannel

.. autofunction:: Mix_GroupChannels

.. autofunction:: Mix_GroupAvailable

.. autofunction:: Mix_GroupCount

.. autofunction:: Mix_GroupOldest

.. autofunction:: Mix_GroupNewer

.. autofunction:: Mix_HaltGroup

.. autofunction:: Mix_FadeOutGroup


Playback functions
------------------

Channel playback
^^^^^^^^^^^^^^^^

.. autofunction:: Mix_PlayChannel

.. autofunction:: Mix_PlayChannelTimed

.. autofunction:: Mix_FadeInChannel

.. autofunction:: Mix_FadeInChannelTimed

.. autofunction:: Mix_Pause

.. autofunction:: Mix_Resume

.. autofunction:: Mix_HaltChannel

.. autofunction:: Mix_ExpireChannel

.. autofunction:: Mix_FadeOutChannel


Music playback
^^^^^^^^^^^^^^

.. autofunction:: Mix_PlayMusic

.. autofunction:: Mix_FadeInMusic

.. autofunction:: Mix_FadeInMusicPos

.. autofunction:: Mix_PauseMusic

.. autofunction:: Mix_ResumeMusic

.. autofunction:: Mix_RewindMusic

.. autofunction:: Mix_SetMusicPosition

.. autofunction:: Mix_HaltMusic

.. autofunction:: Mix_FadeOutMusic

.. autofunction:: Mix_HookMusicFinished


Volume functions
----------------

.. autofunction:: Mix_Volume

.. autofunction:: Mix_VolumeChunk

.. autofunction:: Mix_VolumeMusic


Playback status functions
-------------------------

.. autofunction:: Mix_Playing

.. autofunction:: Mix_Paused

.. autofunction:: Mix_FadingChannel

.. autofunction:: Mix_GetChunk

.. autofunction:: Mix_PlayingMusic

.. autofunction:: Mix_PausedMusic

.. autofunction:: Mix_FadingMusic


Effects-processing functions
----------------------------

.. autofunction:: Mix_RegisterEffect

.. autofunction:: Mix_UnregisterEffect

.. autofunction:: Mix_UnregisterAllEffects

.. autofunction:: Mix_SetPanning

.. autofunction:: Mix_SetPosition

.. autofunction:: Mix_SetDistance

.. autofunction:: Mix_SetReverseStereo

.. autofunction:: Mix_SetPostMix


Decoder-specific functions
--------------------------

External music playback functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_SetMusicCMD

.. autofunction:: Mix_HookMusic

.. autofunction:: Mix_GetMusicHookData


MikMod configuration functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_SetSynchroValue

.. autofunction:: Mix_GetSynchroValue


MIDI SoundFont functions
^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: Mix_SetSoundFonts

.. autofunction:: Mix_GetSoundFonts

.. autofunction:: Mix_EachSoundFont

