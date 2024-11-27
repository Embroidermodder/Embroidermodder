#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The basis of the geometry class: Vector, Pen, Painter and Path.
"""

import math
import os


class Vector():
    r"""
    The basic vector class definition.
    """
    def __init__(self, *args):
        r"""
        If one argument is supplied it is the
        angle of a unit vector in the direction defined by "angle",
        measured anti-clockwise from the x axis.

        If two arguments are supplied then they are the x and y co-ordinates.
        """
        self.x = 0.0
        self.y = 0.0
        if len(args) == 1:
            self.x = math.cos(args[0])
            self.y = math.sin(args[0])
        elif len(args) == 2:
            self.x = args[0]
            self.y = args[1]

    def __add__(self, vector):
        r" Add this vector to the supplied one, overloading the + symbol. "
        output = Vector()
        output.x = self.x + vector.x
        output.y = self.y + vector.y
        return output

    def __sub__(self, vector):
        r""" Subtract this vector from the supplied one,
        overloading the - symbol. """
        output = Vector()
        output.x = self.x - vector.x
        output.y = self.y - vector.y
        return output

    def __mul__(self, vector):
        r"""
        Find the 2D version of a cross product for this and the supplied vector.
        """
        output = self.x * vector.y
        output -= self.y * vector.x
        return output

    def __neg__(self):
        r" Return the negation of this vector. "
        return Vector(-self.x, -self.y)

    def rotate(self, angle):
        r" Rotate this vector by the angle supplied. "
        u = Vector(angle)
        rot = Vector()
        rot.x = self.x*u.x - self.y*u.y
        rot.y = self.x*u.y + self.y*u.x
        return rot

    def scale(self, scale_factor):
        r" Scale this vector. "
        output = Vector(self.x, self.y)
        output.x *= scale_factor
        output.y *= scale_factor
        return output

    def dot(self, output):
        r" Find the dot product of this vector with the supplied one. "
        return self.x*output.x + self.y*output.y

    def length(self):
        r" Returns the length of this vector. "
        return math.sqrt(self.dot(self))


def vector_from_str(input_string):
    r"Convert a string to a vector."
    str_list = input_string.split(",")
    vector_x = float(str_list[0])
    vector_y = float(str_list[1])
    if math.isnan(vector_x) or math.isnan(vector_y):
        return None
    return Vector(vector_x, vector_y)


def closest_vector(vector_list, mouse_point):
    " Returns the closest vector to the point from vector_list. "
    closest = 1000.0
    result = vector_list[0]
    for point in vector_list:
        distance = (point - mouse_point).length()
        if distance < closest:
            result = point
            closest = distance
    return result


class Pen():
    r"""
    The Pen class definition.
    """
    def __init__(self, rgb="#FFFFFF", line_type="solid", line_weight=0.35):
        " . "
        self.rgb = rgb
        self.line_weight = line_weight
        self.line_type = line_type
        self.cap_style = "round cap"
        self.join_style = "round join"

    def paint(self):
        " . "
        debug_message(".")


def get_n_floats(terms, index, n):
    r"Takes n floats out at the position given from the string array."
    floats = []
    for offset in range(n):
        floats += [float(terms[index+offset])]
    return floats, index+offset


def path_from_command(command):
    r"""
    Creates a path matching the SVG-style path string.
    """
    debug_message("add_to_path()")
    path = []
    terms = command.split()
    index = 0
    while index < len(terms):
        if terms[index] == "M":
            values, index = get_n_floats(terms, index, 2)
            path += ["move_to", Vector(*values)]
        elif terms[index] == "L":
            values, index = get_n_floats(terms, index, 2)
            path += ["line_to", Vector(*values)]
        elif terms[index] == "A":
            values, index = get_n_floats(terms, index, 6)
            path += ["arc_to", values]
        elif terms[index] == "a":
            values, index = get_n_floats(terms, index, 6)
            path += ["arc_move_to", values]
        elif terms[index] == "E":
            values, index = get_n_floats(terms, index, 4)
            path += ["ellipse", values]
        elif terms[index] == "Z":
            path += ["close_path", []]
        index += 1
    return path


def convert(src, dst):
    r"Convert a file to another file."
    os.system(f"embroider {src} {dst}")


def translate(message):
    r"Placeholder function"
    return message


def clear_selection():
    r"To clear what the user has selected in the scene."
    return


def debug_message(message, msgtype="DEBUG"):
    r"To clear what the user has selected in the scene."
    if msgtype == "DEBUG":
        print("DEBUG " + message)
    elif msgtype == "ERROR":
        print("ERROR")
        print("    " + message)
    elif msgtype == "ALERT":
        print("ALERT")
        print("    " + message)
    else:
        print("UNKNOWN TYPE")
        print("    " + message)


def set_prompt_prefix(*message):
    r"Placeholder function."
    print(message)


def add_rubber(*message):
    r"Placeholder function."
    print(message)


def append_prompt_history(*message):
    r"Placeholder function."
    print(message)


def load_data(fname):
    r"Placeholder function."
    return {}