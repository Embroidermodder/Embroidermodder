#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

"""


def satin_fill(outline, direction, scale, max_steps):
    r"""
    A traditional directional satin fill, where the supplied
    direction is the .
    """
    return


def radial_satin_fill(outline, center, scale, max_steps):
    r"""
    .
    """
    return


def herringbone_fill(outline, scale, max_steps):
    r"""
    .
    """
    return


def join_stitches(stitch_list):
    r"""
    .
    """
    return


def clip_path(outline, path):
    r"""
    .
    """
    return


def apply_fill(outline, fill, *args):
    r"""Given a contour, overlay a fill pattern and clip out the inner part.

    outline:
        The contour is supplied as a polygon which is a list of vectors
        anticlockwise about the polygon with the first and last point joined.

        For example, a square could be encoded as:
        [[0.0, 0.0], [0.0, 1.0], [1.0, 1.0], [1.0, 0.0]].

    fill:
        The fill is a program that generates a path with points that
        mark where the needle sews. The fill continues until the outline
        is covered.
    """
    stitch_list = fill(outline, *args)
    stitch_list = clip_path(outline, stitch_list)
    stitch_list = join_stitches(stitch_list)
    return stitch_list
