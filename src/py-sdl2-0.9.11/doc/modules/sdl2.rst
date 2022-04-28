.. module:: sdl2
   :synopsis: SDL2 library wrapper

sdl2 - SDL2 library wrapper
===========================
The :mod:`sdl2` package is a :mod:`ctypes`-based wrapper around
the SDL2 library. It wraps nearly all publicly accessible structures and
functions of the SDL2 library to be accessible from Python code.

A detailed documentation about the behaviour of the different functions
can found within the `SDL2 documentation
<http://wiki.libsdl.org/moin.cgi/CategoryAPI>`_.

Usage
-----
You can use :mod:`sdl2` in nearly exactly the same way as you would do with
the SDL library and C code.

.. highlight:: c

A brief example in C code::

   #include <SDL.h>
   
   int main(int argc, char *argv[]) {
       int running;
       SDL_Window *window;
       SDL_Surface *windowsurface;
       SDL_Surface *image;
       SDL_Event event;
       
       SDL_Init(SDL_INIT_VIDEO);
       
       window = SDL_CreateWindow("Hello World",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 592, 460, SDL_WINDOW_SHOWN);
       windowsurface = SDL_GetWindowSurface(window);

       image = SDL_LoadBMP("exampleimage.bmp");
       SDL_BlitSurface(image, NULL, windowsurface, NULL);
       
       SDL_UpdateWindowSurface(window);
       SDL_FreeSurface(image);
       
       running = 1;
       while (running) {
           while (SDL_PollEvent(&event) != 0) {
               if (event.type == SDL_QUIT) {
                   running = 0;
                   break;
               }
           }
       }
       SDL_DestroyWindow(window);
       SDL_Quit();
       return 0;
    }

.. highlight:: python

Doing the same in Python: ::

    import sys
    import ctypes
    from sdl2 import *
    
    def main():
        SDL_Init(SDL_INIT_VIDEO)
        window = SDL_CreateWindow(b"Hello World",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  592, 460, SDL_WINDOW_SHOWN)
        windowsurface = SDL_GetWindowSurface(window)

        image = SDL_LoadBMP(b"exampleimage.bmp")
        SDL_BlitSurface(image, None, windowsurface, None)

        SDL_UpdateWindowSurface(window)
        SDL_FreeSurface(image)

        running = True
        event = SDL_Event()
        while running:
            while SDL_PollEvent(ctypes.byref(event)) != 0:
                if event.type == SDL_QUIT:
                    running = False
                    break

        SDL_DestroyWindow(window)
        SDL_Quit()
        return 0

    if __name__ == "__main__":
        sys.exit(main())

You can port code in a straightforward manner from one language to the other,
though it is important to know about the limitations and slight differences
mentioned below. Also, PySDL2 offers advanced functionality, which also feels
more *'pythonic'*, via the :mod:`sdl2.ext` package.

Missing interfaces
------------------
The following functions, classes, constants and macros of SDL2 are *not*
available within :mod:`sdl2`.

* :c:data:`SDL_REVISION` and :c:data:`SDL_REVISION_NUMBER` from ``SDL_revision.h``
* :c:data:`SDL_NAME()` from ``SDL_name.h``
* :c:func:`SDL_MostSignificantBitIndex32` and `SDL_HasExactlyOneBitSet32` from
  ``SDL_bits.h``
* Most functions from ``SDL2_stdinc.h`` (most are for math and string operations
  Python already has excellent built-in support for)
* Everything from ``SDL_main.h``
* Everything from ``SDL_system.h``
* Everything from ``SDL_assert.h``
* Everything from ``SDL_thread.h``
* Everything from ``SDL_atomic.h``
* Everything from ``SDL_opengl.h`` (see PyOpenGL for a compatible OpenGL API)
* Everything from ``SDL_mutex.h``

Additional interfaces
---------------------
The following functions, classes, constants and macros are *not* part of
SDL2, but were introduced by :mod:`sdl2`.

.. data:: ALL_PIXELFORMATS

   Tuple containing all SDL2 pixel format constants (SDL_PIXELFORMAT_INDEX1LSB,
   ..., SDL_PIXELFORMAT_RGB565, ...).

.. data:: AUDIO_FORMATS

   Set containing all SDL2 audio format constants (AUDIO_U8, AUDIO_S8,
   ... AUDIO_F32LSB, ... ).

.. function:: sdl2.rw_from_object(obj : object) -> SDL_RWops

   Creates a SDL_RWops from any Python object. The Python object must at least
   support the following methods:

   read(length) -> data
   
     length is the size in bytes to be read. A call to len(data) must
     return the correct amount of bytes for the data, so that
     len(data) / [size in bytes for a single element from data] returns
     the amount of elements. Must raise an error on failure.

   seek(offset, whence) -> int
   
     offset denotes the offset to move the read/write pointer of the
     object to. whence indicates the movement behaviour and can be one
     of the following values:
                
     * RW_SEEK_SET - move to offset from the start of the file
     * RW_SEEK_CUR - move by offset from the relative location
     * RW_SEEK_END - move to offset from the end of the file
     
     If it could not move read/write pointer to the desired location,
     an error must be raised.

   tell() -> int
   
     Must return the current offset. This method must only be
     provided, if seek() does not return any value.

   close() -> None
   
     Closes the object(or its internal data access methods). Must raise
     an error on failure.

   write(data) -> None
   
     Writes the passed data(which is a string of bytes) to the object.
     Must raise an error on failure.

     .. note::

        The write() method is optional and only necessary, if the passed
        object should be able to write data.

   The returned :class:`sdl2.SDL_RWops` is a pure Python object and
   **must not** be freed via :func:`sdl2.SDL_FreeRW()`.
