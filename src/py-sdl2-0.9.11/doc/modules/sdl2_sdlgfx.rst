.. currentmodule:: sdl2.sdlgfx

sdl2.sdlgfx - Python bindings for SDL2_gfx
==========================================

py-sdl2 provides bindings for SDL2_gfx, an unofficial addon library for SDL2
that contains a range of useful functions for rendering shapes, frame pacing,
and SDL surface manipulation.

Note that unlike the other SDL2 modules supported by this package, SDL2_gfx is
not an official part of the SDL2 project.

.. contents:: Table Of Contents
   :depth: 1
   :local:


Framerate management classes and functions
------------------------------------------

.. autoclass:: FPSManager

.. autofunction:: SDL_initFramerate

.. autofunction:: SDL_setFramerate

.. autofunction:: SDL_getFramerate

.. autofunction:: SDL_getFramecount

.. autofunction:: SDL_framerateDelay


Drawing functions
-----------------

Pixel drawing functions
^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: pixelColor

.. autofunction:: pixelRGBA


Line drawing functions
^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: hlineColor

.. autofunction:: hlineRGBA

.. autofunction:: vlineColor

.. autofunction:: vlineRGBA

.. autofunction:: lineColor

.. autofunction:: lineRGBA

.. autofunction:: aalineColor

.. autofunction:: aalineRGBA

.. autofunction:: thickLineColor

.. autofunction:: thickLineRGBA


Rectangle drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: rectangleColor

.. autofunction:: rectangleRGBA

.. autofunction:: roundedRectangleColor

.. autofunction:: roundedRectangleRGBA

.. autofunction:: boxColor

.. autofunction:: boxRGBA

.. autofunction:: roundedBoxColor

.. autofunction:: roundedBoxRGBA


Circle drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: circleColor

.. autofunction:: circleRGBA

.. autofunction:: arcColor

.. autofunction:: arcRGBA

.. autofunction:: aacircleColor

.. autofunction:: aacircleRGBA

.. autofunction:: filledCircleColor

.. autofunction:: filledCircleRGBA


Ellipse drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: ellipseColor

.. autofunction:: ellipseRGBA

.. autofunction:: aaellipseColor

.. autofunction:: aaellipseRGBA

.. autofunction:: filledEllipseColor

.. autofunction:: filledEllipseRGBA


Pie slice drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: pieColor

.. autofunction:: pieRGBA

.. autofunction:: filledPieColor

.. autofunction:: filledPieRGBA


Triangle drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: trigonColor

.. autofunction:: trigonRGBA

.. autofunction:: aatrigonColor

.. autofunction:: aatrigonRGBA

.. autofunction:: filledTrigonColor

.. autofunction:: filledTrigonRGBA


Polygon drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: polygonColor

.. autofunction:: polygonRGBA

.. autofunction:: aapolygonColor

.. autofunction:: aapolygonRGBA

.. autofunction:: filledPolygonColor

.. autofunction:: filledPolygonRGBA

.. autofunction:: texturedPolygon


Bezier curve drawing functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: bezierColor

.. autofunction:: bezierRGBA



Text rendering functions
------------------------

.. autofunction:: gfxPrimitivesSetFont

.. autofunction:: gfxPrimitivesSetFontRotation

.. autofunction:: characterColor

.. autofunction:: characterRGBA

.. autofunction:: stringColor

.. autofunction:: stringRGBA



Surface rotation and scaling functions
--------------------------------------

.. autofunction:: rotozoomSurface

.. autofunction:: rotozoomSurfaceXY

.. autofunction:: rotozoomSurfaceSize

.. autofunction:: rotozoomSurfaceSizeXY

.. autofunction:: zoomSurface

.. autofunction:: zoomSurfaceSize

.. autofunction:: shrinkSurface

.. autofunction:: rotateSurface90Degrees
