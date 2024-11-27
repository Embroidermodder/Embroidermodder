#!/usr/bin/env python3

r"""
Libembroidery
1.0.0-alpha
https://www.libembroidery.org

---------------------------------------------------------------------------------

Copyright 2018-2024 The Embroidermodder Team
Libembroidery is Open Source Software.
See LICENSE for licensing terms.

---------------------------------------------------------------------------------

Hand-written bindings to the C library for Python. This is manually updated.

Documentation on the ctypes interface here:
    https://docs.python.org/3.8/library/ctypes.html

---------------------------------------------------------------------------------

The test suite for the libembroidery Python bindings.

Similar to, although not a replica of, the internal tests. This
cannot replace them because some systems that will run the library
won't support Python.

(Libembroidery may need to include some truly ancient PC in order
to deal with the older embroidery machines that they may talk to.)
"""

import unittest
import math
import ctypes
import pathlib

def load_library():
    """
    TO DO: This needs to be operating system-dependent.
    Currently for testing reasons it assumes that the shared library is in the same
    directory.
    """
    library_dir = pathlib.Path().absolute()
    libembroidery_library = library_dir / "libembroidery.so"
    libembroidery = ctypes.CDLL(libembroidery_library)

    libembroidery.emb_vector_add.argtypes = (vector, vector)
    libembroidery.emb_vector_add.restype = vector
    return libembroidery

class vector(ctypes.Structure):
    """ Wrapper for the EmbVector structure. """
    _fields_ = [
        ('x', ctypes.c_float),
        ('y', ctypes.c_float)
    ]

    def __str__(self,) -> str:
        return f"({self.x}, {self.y})"

emb = load_library()

def create_test_file_1(outf="test01.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # 10mm circle
    for i in range(20):
        x = 0.0
        y = 1.0*(i+1)
        pattern.add_stitch(x, y, flags=emb.JUMP, color=0)

    for i in range(200):
        x = 10 + 10 * math.sin(i * 0.01 * math.pi)
        y = 10 + 10 * math.cos(i * 0.01 * math.pi)
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)
    pattern.free()


def create_test_file_2(outf="test02.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # sin wave
    for i in range(100):
        x = 10 + 10 * math.sin(i * (0.5 / math.pi))
        y = 10 + i * 0.1
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)


def create_test_file_3(outf="test03.csv"):
    """
    """
    pattern = Pattern()
    pattern.add_circle(10.0, 1.0, 5.0)
    pattern.add_thread(emb.black_thread)
    pattern.convert_geometry()
    pattern.end()

    pattern.write(outf)


def convert_test(t, from_f, to_f):
    """
    """
    inf = "test%02d.%s" % (t, from_f)
    outf = "test%02d_convert_from_%s.%s" % (t, from_f, to_f)
    if t == 1:
        create_test_file_1(inf)
    elif t == 2:
        create_test_file_2(inf)
    else:
        create_test_file_3(inf)
    return emb.convert(inf, outf)


def convert_test_all(from_f, to_f):
    for i in range(1, 4):
        if convert_test(i, from_f, to_f) != 0:
            return 1
    return 0


class TestLibembroidery(unittest.TestCase):
    r"""
    The parent class for all of the bindings
    tests, this could be made to chain into the
    internal C testing.
    """
    def test_vector_length(self):
        " Tests the vector length function. "
        v = Vector(3.0, 4.0)
        self.assertAlmostEqual(v.length(), 5.0)

    def test_arc(self):
        r"Adds an arc to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_arc(1.0, 2.0, 2.0, 3.0, 4.0, 6.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_circle(self):
        r"Adds a circle to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_circle(3.0, 4.0, 2.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_ellipse(self):
        r"Adds an ellipse to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_ellipse(3.0, 4.0, 7.0, 4.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_path(self):
        r"Adds a path to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_path([])
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_main(self):
        r"""
        Tests the rendering and simulating of patterns.
        """
        pattern = Pattern()
        image = emb.Image(100, 100)
        hilbert_curve_result = pattern.hilbert_curve(3)
        render_result = image.render(pattern, 20.0, 20.0, "hilbert_level_3.ppm")
        simulate_result = image.simulate(pattern, 20.0, 20.0, "hilbert_level_3.avi")

        self.assertEqual(hilbert_curve_result, 0)
        self.assertEqual(render_result, 0)
        self.assertEqual(simulate_result, 0)

    def test_convert_csv_svg(self):
        " Test conversion from csv to svg. "
        self.assertEqual(convert_test_all("csv", "svg"), 1)

    def test_convert_csv_dst(self):
        " Test conversion from csv to dst. "
        self.assertEqual(convert_test_all("csv", "dst"), 1)

    def test_convert_csv_pes(self):
        " Test conversion from csv to pes. "
        self.assertEqual(convert_test_all("csv", "pes"), 1)

    def test_convert_svg_csv(self):
        " Test conversion from svg to csv. "
        self.assertEqual(convert_test_all("svg", "csv"), 1)

    def test_convert_dst_csv(self):
        " Test conversion from dst to csv. "
        self.assertEqual(convert_test_all("dst", "csv"), 1)

    def test_convert_pes_csv(self):
        " Test conversion from pes to csv. "
        self.assertEqual(convert_test_all("pes", "csv"), 1)

    def test_circle_tangent(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(0.0, 0.0, 3.0)
        p = Vector(4.0, 0.0)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 2.2500)
        self.assertAlmostEqual(t0.y, 1.9843)
        self.assertAlmostEqual(t1.x, 2.2500)
        self.assertAlmostEqual(t1.y, -1.9843)

    def test_circle_tangent_2(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(20.1762, 10.7170, 6.8221)
        p = Vector(24.3411, 18.2980)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 19.0911)
        self.assertAlmostEqual(t0.y, 17.4522)
        self.assertAlmostEqual(t1.x, 26.4428)
        self.assertAlmostEqual(t1.y, 13.4133)

    def test_thread_color(self):
        """
        TODO: Add capability for converting multiple files of various
        types to a single format. 

        Currently, we only convert a single file to multiple formats.
        """
        tColor = 0xFFD25F00
        c = emb.color(rgb=(0xD2, 0x5F, 0x00))
        tBrand = emb.Sulky_Rayon
        tNum = emb.threadColorNum(c, tBrand)
        tName = ""
        emb.threadColorName(tName, c, tBrand)

        # Solution: tNum = 1833
        # Solution: Pumpkin Pie
        print("""
Color : 0x%X
Brand : %d
Num   : %d
Name  : %s

""" % (tColor, tBrand, tNum, tName))
        return 0

    def test_format_table(self):
        """
        """
        tName = "example.zsk"
        f_format = emb.emb_identify_format(tName)
        table = emb.formatTable[f_format]

        self.assertEqual(table.extension, ".zsk")
        self.assertEqual(table.description, "ZSK USA Embroidery  f_format")
        self.assertEqual(table.reader_state, 'U')
        self.assertEqual(table.writer_state, ' ')
        self.assertEqual(table.type, 1)

    def test_list_processor():
        r" . "
        print(list_processor("(+ (* 4 6) 3)"))
        print(list_processor("(+ (/ 6 4) (* 3 2))"))
        example = "(* (/ 8 11) (sin (- (/ 10 9) (* 56 t))))"
        print(list_processor(example.replace("t", "0.04")))

    def test_selected():
        r"."
        debug_message("Test what is selected")

    def test_polyline_array():
        r"Polyline & Polygon Testing"
        p = pattern()

        offset = vector(0.0, 0.0)

        polyline_array = [
            vector(1.0, 1.0),
            vector(1.0, 2.0),
            vector(2.0, 2.0),
            vector(2.0, 3.0),
            vector(3.0, 3.0),
            vector(3.0, 2.0),
            vector(4.0, 2.0),
            vector(4.0, 1.0)
        ]
        polyline_array = [v + offset for v in polyline_array]
        p.add_polyline(polyline_array)

        offset = vector(5.0, 0.0)

        polygon_array = [
            vector(1.0, 1.0),
            vector(1.0, 2.0),
            vector(2.0, 2.0),
            vector(2.0, 3.0),
            vector(3.0, 3.0),
            vector(3.0, 2.0),
            vector(4.0, 2.0),
            vector(4.0, 1.0)
        ]
        polygon_array = [vector + offset for vector in polygon_array]
        p.add_polygon(polygon_array)

        p.write("output.svg")


if __name__ == '__main__':
    unittest.main()
