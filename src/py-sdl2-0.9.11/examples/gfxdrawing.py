"""2D drawing examples utilising the SDL2_gfx functions."""
import sys
import ctypes
from random import randint

import sdl2
import sdl2.sdlgfx
import sdl2.ext


# Draws random lines using the passed rendering context
def draw_lines(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    whalf = width // 2 - 2
    hhalf = height // 2 - 2
    lw = 5
    x0, x1 = whalf, whalf
    y0, y1 = 0, height
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    x0, x1 = 0, width
    y0, y1 = hhalf, hhalf
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    for x in range(15):
        # In the first quadrant, draw normal lines
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(0, whalf), randint(0, whalf)
        y0, y1 = randint(0, hhalf), randint(0, hhalf)
        sdl2.sdlgfx.lineColor(context.sdlrenderer, x0, y0, x1, y1, color)
        # In the second quadrant, draw aa lines
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(whalf + lw, width), randint(whalf + lw, width)
        y0, y1 = randint(0, hhalf), randint(0, hhalf)
        sdl2.sdlgfx.aalineColor(context.sdlrenderer, x0, y0, x1, y1, color)
        # In the third quadrant, draw horizontal lines
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(0, whalf), randint(0, whalf)
        y0 = randint(hhalf + lw, height)
        sdl2.sdlgfx.hlineColor(context.sdlrenderer, x0, x1, y0, color)
        # In the fourth quadrant, draw vertical lines
        color = randint(0, 0xFFFFFFFF)
        x0 = randint(whalf + lw, width)
        y0, y1 = randint(hhalf + lw, height), randint(hhalf + lw, height)
        sdl2.sdlgfx.vlineColor(context.sdlrenderer, x0, y0, y1, color)


# Draws random circles using the passed rendering context
def draw_circles(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    wthird = width // 3 - 1
    lw = 3
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, wthird, 0, wthird, height,
                               lw, 0xFFFFFFFF)
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, (2 * wthird + lw), 0,
                               (2 * wthird + lw), height, lw, 0xFFFFFFFF)
    for x in range(15):
        # In the first part, draw circles
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        r = randint(1, max(min(x, wthird - x), 2))
        sdl2.sdlgfx.circleColor(context.sdlrenderer, x, y, r, color)
        # In the second part, draw aa circles
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        r = randint(1, max(min(x, wthird - x), 2))
        sdl2.sdlgfx.aacircleColor(context.sdlrenderer, x + wthird + lw, y, r,
                                  color)
        # In the third part, draw filled circles
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        r = randint(1, max(min(x, wthird - x), 2))
        sdl2.sdlgfx.filledCircleColor(context.sdlrenderer, x + 2 * (wthird + lw),
                                      y, r, color)


# Draws random ellipsis using the passed rendering context
def draw_ellipsis(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    wthird = width // 3 - 1
    eheight = height // 4
    lw = 3
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, wthird, 0, wthird, height,
                               lw, 0xFFFFFFFF)
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, (2 * wthird + lw), 0,
                               (2 * wthird + lw), height, lw, 0xFFFFFFFF)
    for x in range(15):
        # In the first part, draw ellipsis
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        rx, ry = randint(1, max(min(x, wthird - x), 2)), randint(0, eheight)
        sdl2.sdlgfx.ellipseColor(context.sdlrenderer, x, y, rx, ry, color)
        # In the second part, draw aa ellipsis
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        rx, ry = randint(1, max(min(x, wthird - x), 2)), randint(0, eheight)
        sdl2.sdlgfx.aaellipseColor(context.sdlrenderer, x + wthird + lw, y,
                                   rx, ry, color)
        # In the third part, draw filled ellipsis
        color = randint(0, 0xFFFFFFFF)
        x, y = randint(0, wthird), randint(0, height)
        rx, ry = randint(1, max(min(x, wthird - x), 2)), randint(0, eheight)
        sdl2.sdlgfx.filledEllipseColor(context.sdlrenderer,
                                       x + 2 * (wthird + lw), y, rx, ry,
                                       color)


# Draws random rectangles using the passed rendering context
def draw_rects(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    whalf = width // 2 - 2
    hhalf = height // 2 - 2
    lw = 5
    x0, x1 = whalf, whalf
    y0, y1 = 0, height
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    x0, x1 = 0, width
    y0, y1 = hhalf, hhalf
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    for x in range(15):
        # In the first quadrant, draw normal rectangles
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(0, whalf), randint(0, whalf)
        y0, y1 = randint(0, hhalf), randint(0, hhalf)
        sdl2.sdlgfx.rectangleColor(context.sdlrenderer, x0, y0, x1, y1, color)
        # In the second quadrant, draw rounded rectangles
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(whalf + lw, width), randint(whalf + lw, width)
        y0, y1 = randint(0, hhalf), randint(0, hhalf)
        r = randint(0, max(x1 - x0, x0 - x1))
        sdl2.sdlgfx.roundedRectangleColor(context.sdlrenderer, x0, y0, x1, y1, r,
                                          color)
        # In the third quadrant, draw horizontal lines
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(0, whalf), randint(0, whalf)
        y0, y1 = randint(hhalf + lw, height), randint(hhalf + lw, height)
        sdl2.sdlgfx.boxColor(context.sdlrenderer, x0, y0, x1, y1, color)
        # In the fourth quadrant, draw vertical lines
        color = randint(0, 0xFFFFFFFF)
        x0, x1 = randint(whalf + lw, width), randint(whalf + lw, width)
        y0, y1 = randint(hhalf + lw, height), randint(hhalf + lw, height)
        r = randint(0, max(x1 - x0, x0 - x1))
        sdl2.sdlgfx.roundedBoxColor(context.sdlrenderer, x0, y0, x1, y1, r,
                                    color)


# Draws random triangles using the passed rendering context
def draw_trigons(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    wthird = width // 3 - 1
    lw = 3
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, wthird, 0, wthird, height,
                               lw, 0xFFFFFFFF)
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, (2 * wthird + lw), 0,
                               (2 * wthird + lw), height, lw, 0xFFFFFFFF)
    for x in range(15):
        # In the first part, draw triangles
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(0, wthird), randint(0, height)
        x1, y1 = randint(0, wthird), randint(0, height)
        x2, y2 = randint(0, wthird), randint(0, height)
        sdl2.sdlgfx.trigonColor(context.sdlrenderer, x0, y0, x1, y1, x2, y2,
                                color)
        # In the second part, draw aa triangles
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(0, wthird) + wthird + lw, randint(0, height)
        x1, y1 = randint(0, wthird) + wthird + lw, randint(0, height)
        x2, y2 = randint(0, wthird) + wthird + lw, randint(0, height)
        sdl2.sdlgfx.aatrigonColor(context.sdlrenderer, x0, y0, x1, y1, x2, y2,
                                  color)
        # In the third part, draw filled triangles
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(0, wthird) + 2 * (wthird + lw), randint(0, height)
        x1, y1 = randint(0, wthird) + 2 * (wthird + lw), randint(0, height)
        x2, y2 = randint(0, wthird) + 2 * (wthird + lw), randint(0, height)
        sdl2.sdlgfx.filledTrigonColor(context.sdlrenderer, x0, y0, x1, y1,
                                      x2, y2, color)


# Draws random polygons using the passed rendering context
def draw_polygons(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    wthird = width // 3 - 1
    lw = 3
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, wthird, 0, wthird, height,
                               lw, 0xFFFFFFFF)
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, (2 * wthird + lw), 0,
                               (2 * wthird + lw), height, lw, 0xFFFFFFFF)
    for x in range(5):
        # In the first part, draw polygons
        color = randint(0, 0xFFFFFFFF)
        ptcount = randint(3, 10)
        xlist, ylist = (sdl2.Sint16 * ptcount)(), (sdl2.Sint16 * ptcount)()
        for k in range(ptcount):
            xlist[k] = randint(0, wthird)
            ylist[k] = randint(0, height)
        xptr = ctypes.cast(xlist, ctypes.POINTER(sdl2.Sint16))
        yptr = ctypes.cast(ylist, ctypes.POINTER(sdl2.Sint16))
        sdl2.sdlgfx.polygonColor(context.sdlrenderer, xptr, yptr, ptcount, color)
        # In the second part, draw aa polygons
        color = randint(0, 0xFFFFFFFF)
        ptcount = randint(3, 10)
        xlist, ylist = (sdl2.Sint16 * ptcount)(), (sdl2.Sint16 * ptcount)()
        for k in range(ptcount):
            xlist[k] = randint(0, wthird) + wthird + lw
            ylist[k] = randint(0, height)
        xptr = ctypes.cast(xlist, ctypes.POINTER(sdl2.Sint16))
        yptr = ctypes.cast(ylist, ctypes.POINTER(sdl2.Sint16))
        sdl2.sdlgfx.aapolygonColor(context.sdlrenderer, xptr, yptr, ptcount,
                                   color)
        # In the third part, draw filled polygons
        color = randint(0, 0xFFFFFFFF)
        ptcount = randint(3, 10)
        xlist, ylist = (sdl2.Sint16 * ptcount)(), (sdl2.Sint16 * ptcount)()
        for k in range(ptcount):
            xlist[k] = randint(0, wthird) + 2 * (wthird + lw)
            ylist[k] = randint(0, height)
        xptr = ctypes.cast(xlist, ctypes.POINTER(sdl2.Sint16))
        yptr = ctypes.cast(ylist, ctypes.POINTER(sdl2.Sint16))
        sdl2.sdlgfx.filledPolygonColor(context.sdlrenderer, xptr, yptr, ptcount,
                                       color)


# Draw random elements using the passed rendering context
def draw_mixed(context, width, height):
    # Reset the visible area with a black color.
    context.clear(0)
    # Split the visible area
    whalf = width // 2 - 2
    hhalf = height // 2 - 2
    lw = 5
    x0, x1 = whalf, whalf
    y0, y1 = 0, height
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    x0, x1 = 0, width
    y0, y1 = hhalf, hhalf
    sdl2.sdlgfx.thickLineColor(context.sdlrenderer, x0, y0, x1, y1, lw,
                               0xFFFFFFFF)
    for x in range(15):
        # In the first quadrant, draw arcs
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(0, whalf), randint(0, hhalf)
        rad = randint(0, min(whalf - x0, hhalf - y0))
        start, end = randint(0, 360), randint(0, 360)
        sdl2.sdlgfx.arcColor(context.sdlrenderer, x0, y0, rad, start, end, color)
        # In the second quadrant, draw bezier curves
        color = randint(0, 0xFFFFFFFF)
        ptcount = randint(3, 10)
        xlist, ylist = (sdl2.Sint16 * ptcount)(), (sdl2.Sint16 * ptcount)()
        for k in range(ptcount):
            xlist[k] = randint(whalf, width)
            ylist[k] = randint(0, hhalf)
        steps = randint(2, 10)
        xptr = ctypes.cast(xlist, ctypes.POINTER(sdl2.Sint16))
        yptr = ctypes.cast(ylist, ctypes.POINTER(sdl2.Sint16))
        sdl2.sdlgfx.bezierColor(context.sdlrenderer, xptr, yptr, ptcount, steps,
                                color)
        # In the third quadrant, draw pies
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(0, whalf), randint(hhalf + lw, height)
        rad = randint(0, min(whalf - x0, y0 - (hhalf + lw)))
        start, end = randint(0, 360), randint(0, 360)
        sdl2.sdlgfx.pieColor(context.sdlrenderer, x0, y0, rad, start, end, color)
        # In the fourth quadrant, draw filled pies
        color = randint(0, 0xFFFFFFFF)
        x0, y0 = randint(whalf + lw, width), randint(hhalf + lw, height)
        rad = randint(0, min(x0 - (whalf + lw), y0 - (hhalf + lw)))
        start, end = randint(0, 360), randint(0, 360)
        sdl2.sdlgfx.filledPieColor(context.sdlrenderer, x0, y0, rad, start, end,
                                   color)


def run():
    # You know those from the helloworld.py example.
    # Initialize the video subsystem, create a window and make it visible.
    sdl2.ext.init()
    window = sdl2.ext.Window("sdlgfx drawing examples", size=(800, 600))
    window.show()

    # Create a rendering context for the window. The sdlgfx module requires it.
    # NOTE: Defaults to software rendering to avoid flickering on some systems.
    if "-hardware" in sys.argv:
        renderflags = sdl2.render.SDL_RENDERER_ACCELERATED | sdl2.render.SDL_RENDERER_PRESENTVSYNC
    else:
        renderflags = sdl2.render.SDL_RENDERER_SOFTWARE
    context = sdl2.ext.Renderer(window, flags=renderflags)

    # Retrieve and display renderer + available renderer info
    info = sdl2.render.SDL_RendererInfo()
    sdl2.SDL_GetRendererInfo(context.sdlrenderer, info)

    print("\nUsing renderer: {0}".format(info.name.decode('utf-8')))
    print("\nAvailable renderers:")
    num_drivers = sdl2.SDL_GetNumRenderDrivers()
    for i in range(0, num_drivers):
        info = sdl2.render.SDL_RendererInfo()
        sdl2.SDL_GetRenderDriverInfo(i, info)
        print(" - " + info.name.decode('utf-8'))
        
    # We implement the functionality as it was done in colorpalettes.py and
    # utilise a mapping table to look up the function to be executed, together
    # with the arguments they should receive
    functions = ((draw_lines, (context, 800, 600)),
                 (draw_circles, (context, 800, 600)),
                 (draw_ellipsis, (context, 800, 600)),
                 (draw_rects, (context, 800, 600)),
                 (draw_trigons, (context, 800, 600)),
                 (draw_polygons, (context, 800, 600)),
                 (draw_mixed, (context, 800, 600))
                )

    # A storage variable for the function we are currently on, so that we know
    # which function to execute next.
    curindex = 0
    draw_lines(context, 800, 600)
    context.present()

    # The event loop is nearly the same as we used in colorpalettes.py. If you
    # do not know, what happens here, take a look at colorpalettes.py for a
    # detailed description.
    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
            if event.type == sdl2.SDL_MOUSEBUTTONDOWN:
                curindex += 1
                if curindex >= len(functions):
                    curindex = 0
                # In contrast to colorpalettes.py, our mapping table consists
                # of functions and their arguments. Thus, we get the currently
                # requested function and argument tuple and execute the
                # function with the arguments.
                func, args = functions[curindex]
                func(*args)
                context.present()
                break
    sdl2.ext.quit()
    return 0


if __name__ == "__main__":
    sys.exit(run())
