import sys
import copy
import pytest
import random
from ctypes import byref, c_int
import sdl2
from sdl2.stdinc import SDL_FALSE, SDL_TRUE
from sdl2 import rect

to_ctypes = lambda seq, dtype: (dtype * len(seq))(*seq)


class TestSDLPoint(object):
    __tags__ = ["sdl"]

    def test_init(self):
        pt = rect.SDL_Point()
        assert (pt.x, pt.y) == (0, 0)
        for i in range(0, 100):
            x = random.randint(-1000, 1000)
            y = random.randint(-1000, 1000)
            pt = rect.SDL_Point(x, y)
            assert (pt.x, pt.y) == (x, y)

    def test_xy(self):
        pt = rect.SDL_Point()
        for i in range(0, 50):
            x = random.randint(-1000, 1000)
            y = random.randint(-1000, 1000)
            pt.x = x
            pt.y = y
            assert (pt.x, pt.y) == (x, y)
        with pytest.raises(TypeError):
            pt.x = 10.4
        with pytest.raises(TypeError):
            pt.y = 10.4
        with pytest.raises(TypeError):
            pt.x = "point"
        with pytest.raises(TypeError):
            pt.y = "point"
        with pytest.raises(TypeError):
            pt.x = None
        with pytest.raises(TypeError):
            pt.y = None

    def test___repr__(self):
        pt = rect.SDL_Point(10, 12)
        pt2 = eval("rect.%s" % repr(pt))
        assert pt == pt2
        assert (pt.x, pt.y) == (pt2.x, pt2.y)

    def test___copy__(self):
        pt = rect.SDL_Point()
        pt2 = copy.copy(pt)
        assert pt == pt2
        assert (pt.x, pt.y) == (pt2.x, pt2.y)
        pt2.x = 7
        pt2.y = 9
        pt3 = copy.copy(pt2)
        assert pt != pt2
        assert pt3 == pt2

    def test___eq__(self):
        assert rect.SDL_Point() == rect.SDL_Point()
        coords = [(0, 0), (10, 0), (0, 10), (12, 10), (7, 10)]
        for x1, y1 in coords:
            for x2, y2 in coords:
                equal = rect.SDL_FPoint(x1, y1) == rect.SDL_FPoint(x2, y2)
                assert equal if (x1 == x2 and y1 == y2) else not equal

    def test___ne__(self):
        assert not rect.SDL_Point() != rect.SDL_Point()
        coords = [(0, 0), (10, 0), (0, 10), (12, 10), (7, 10)]
        for x1, y1 in coords:
            for x2, y2 in coords:
                notequal = rect.SDL_Point(x1, y1) != rect.SDL_Point(x2, y2)
                assert notequal if (x1 != x2 or y1 != y2) else not notequal



@pytest.mark.skipif(sdl2.dll.version < 2010, reason="not available")
class TestSDLFPoint(object):
    __tags__ = ["sdl"]

    def test_init(self):
        pt = rect.SDL_FPoint()
        assert (pt.x, pt.y) == (0, 0)
        for i in range(0, 100):
            x = random.uniform(-1000, 1000)
            y = random.uniform(-1000, 1000)
            pt = rect.SDL_FPoint(x, y)
            assert (pt.x, pt.y) == pytest.approx((x, y))

    def test_xy(self):
        pt = rect.SDL_FPoint()
        for i in range(0, 50):
            x = random.uniform(-1000, 1000)
            y = random.uniform(-1000, 1000)
            pt.x = x
            pt.y = y
            assert (pt.x, pt.y) == pytest.approx((x, y))
        with pytest.raises(TypeError):
            pt.x = "point"
        with pytest.raises(TypeError):
            pt.y = "point"
        with pytest.raises(TypeError):
            pt.x = None
        with pytest.raises(TypeError):
            pt.y = None

    def test___repr__(self):
        pt = rect.SDL_FPoint(3.24, 12.8)
        pt2 = eval("rect.%s" % repr(pt))
        assert pt == pt2
        assert (pt.x, pt.y) == (pt2.x, pt2.y)

    def test___copy__(self):
        pt = rect.SDL_FPoint()
        pt2 = copy.copy(pt)
        assert pt == pt2
        assert (pt.x, pt.y) == (pt2.x, pt2.y)
        pt2.x = 7
        pt2.y = 9
        pt3 = copy.copy(pt2)
        assert pt != pt2
        assert pt3 == pt2

    def test___eq__(self):
        assert rect.SDL_FPoint() == rect.SDL_FPoint()
        coords = [(0, 0.5), (10, 0.5), (0, 10.5), (12, 10.5), (7, 10.5)]
        for x1, y1 in coords:
            for x2, y2 in coords:
                equal = rect.SDL_FPoint(x1, y1) == rect.SDL_FPoint(x2, y2)
                assert equal if (x1 == x2 and y1 == y2) else not equal

    def test___ne__(self):
        assert not rect.SDL_FPoint() != rect.SDL_FPoint()
        coords = [(0, 0.5), (10, 0.5), (0, 10.5), (12, 10.5), (7, 10.5)]
        for x1, y1 in coords:
            for x2, y2 in coords:
                notequal = rect.SDL_FPoint(x1, y1) != rect.SDL_FPoint(x2, y2)
                assert notequal if (x1 != x2 or y1 != y2) else not notequal


class TestSDLRect(object):
    __tags__ = ["sdl"]

    def test_init(self):
        rt = rect.SDL_Rect()
        assert (rt.x, rt.y, rt.w, rt.h) == (0, 0, 0, 0)
        for i in range(0, 50):
            x = random.randint(-1000, 1000)
            y = random.randint(-1000, 1000)
            w = random.randint(-1000, 1000)
            h = random.randint(-1000, 1000)
            rt = rect.SDL_Rect(x, y, w, h)
            assert (rt.x, rt.y, rt.w, rt.h) == (x, y, w, h)

    def test_xywh(self):
        rt = rect.SDL_Rect()
        for i in range(0, 50):
            x = random.randint(-1000, 1000)
            y = random.randint(-1000, 1000)
            w = random.randint(-1000, 1000)
            h = random.randint(-1000, 1000)
            rt.x = x
            rt.y = y
            rt.w = w
            rt.h = h
            assert (rt.x, rt.y, rt.w, rt.h) == (x, y, w, h)

        bad_inputs = [10.4, "point", None]
        for val in bad_inputs:  
            with pytest.raises(TypeError):
                rt.x = val
            with pytest.raises(TypeError):
                rt.y = val
            with pytest.raises(TypeError):
                rt.w = val
            with pytest.raises(TypeError):
                rt.h = val

    def test___repr__(self):
        rt = rect.SDL_Rect(1, 2, 3, 4)
        rt2 = eval("rect.%s" % repr(rt))
        assert (rt.x, rt.y, rt.w, rt.h) == (rt2.x, rt2.y, rt2.w, rt2.h)
        assert rt == rt2

    def test___copy__(self):
        rt = rect.SDL_Rect()
        rt2 = copy.copy(rt)
        assert rt == rt2
        assert (rt.x, rt.y, rt.w, rt.h) == (rt2.x, rt2.y, rt2.w, rt2.h)
        rt2.x = 5
        rt2.y = 33
        rt2.w = 17
        rt2.w = 212
        rt3 = copy.copy(rt2)
        assert rt != rt2
        assert rt3 == rt2

    def test___eq__(self):
        sdlr = rect.SDL_Rect
        assert sdlr() == sdlr()
        rects = [
            (0, 0, 0, 0), (0, 0, 0, 1), (10, 0, 1, 1), (0, 10, 1, 1),
            (1, 2, 3, 4)
        ]
        for x1, y1, w1, h1 in rects:
            for x2, y2, w2, h2 in rects:
                same = x1 == x2 and y1 == y2 and w1 == w2 and h1 == h2
                equal = sdlr(x1, y1, w1, h1) == sdlr(x2, y2, w2, h2)
                assert equal if same else not equal

    def test___ne__(self):
        sdlr = rect.SDL_Rect
        assert sdlr() == sdlr()
        rects = [
            (0, 0, 0, 0), (0, 0, 0, 1), (10, 0, 1, 1), (0, 10, 1, 1),
            (1, 2, 3, 4)
        ]
        for x1, y1, w1, h1 in rects:
            for x2, y2, w2, h2 in rects:
                same = x1 == x2 and y1 == y2 and w1 == w2 and h1 == h2
                notequal = sdlr(x1, y1, w1, h1) != sdlr(x2, y2, w2, h2)
                assert notequal if same == False else not notequal


@pytest.mark.skipif(sdl2.dll.version < 2010, reason="not available")
class TestSDLFRect(object):
    __tags__ = ["sdl"]

    def test_init(self):
        rt = rect.SDL_FRect()
        assert (rt.x, rt.y, rt.w, rt.h) == (0, 0, 0, 0)
        for i in range(0, 50):
            x = random.uniform(-1000, 1000)
            y = random.uniform(-1000, 1000)
            w = random.uniform(-1000, 1000)
            h = random.uniform(-1000, 1000)
            rt = rect.SDL_FRect(x, y, w, h)
            assert (rt.x, rt.y, rt.w, rt.h) == pytest.approx((x, y, w, h))

    def test_xywh(self):
        rt = rect.SDL_FRect()
        for i in range(0, 50):
            x = random.uniform(-1000, 1000)
            y = random.uniform(-1000, 1000)
            w = random.uniform(-1000, 1000)
            h = random.uniform(-1000, 1000)
            rt.x = x
            rt.y = y
            rt.w = w
            rt.h = h
            assert (rt.x, rt.y, rt.w, rt.h) == pytest.approx((x, y, w, h))

        bad_inputs = ["point", None]
        for val in bad_inputs:  
            with pytest.raises(TypeError):
                rt.x = val
            with pytest.raises(TypeError):
                rt.y = val
            with pytest.raises(TypeError):
                rt.w = val
            with pytest.raises(TypeError):
                rt.h = val

    def test___repr__(self):
        rt = rect.SDL_FRect(1.5, 2.2, 3.8, 4.9)
        rt2 = eval("rect.%s" % repr(rt))
        assert (rt.x, rt.y, rt.w, rt.h) == (rt2.x, rt2.y, rt2.w, rt2.h)
        assert rt == rt2

    def test___copy__(self):
        rt = rect.SDL_FRect()
        rt2 = copy.copy(rt)
        assert rt == rt2
        assert (rt.x, rt.y, rt.w, rt.h) == (rt2.x, rt2.y, rt2.w, rt2.h)
        rt2.x = 5.5
        rt2.y = 33
        rt2.w = 17.2
        rt2.w = 212
        rt3 = copy.copy(rt2)
        assert rt != rt2
        assert rt3 == rt2

    def test___eq__(self):
        sdlr = rect.SDL_FRect
        assert sdlr() == sdlr()
        rects = [
            (0, 0.5, 1, 1), (10, 0.5, 99.9, 1.9), (0, 0.5, 99.9, 96),
            (-2.4, 0.5, 1, 1), (0, 0, 99.9, 1.9)
        ]
        for x1, y1, w1, h1 in rects:
            for x2, y2, w2, h2 in rects:
                same = x1 == x2 and y1 == y2 and w1 == w2 and h1 == h2
                equal = sdlr(x1, y1, w1, h1) == sdlr(x2, y2, w2, h2)
                assert equal if same else not equal

    def test___ne__(self):
        sdlr = rect.SDL_FRect
        assert sdlr() == sdlr()
        rects = [
            (0, 0.5, 1, 1), (10, 0.5, 99.9, 1.9), (0, 0.5, 99.9, 96),
            (-2.4, 0.5, 1, 1), (0, 0, 99.9, 1.9)
        ]
        for x1, y1, w1, h1 in rects:
            for x2, y2, w2, h2 in rects:
                same = x1 == x2 and y1 == y2 and w1 == w2 and h1 == h2
                notequal = sdlr(x1, y1, w1, h1) != sdlr(x2, y2, w2, h2)
                assert notequal if same == False else not notequal


def test_SDL_RectEmpty():
    for i in range(0, 50):
        w = random.randint(-100, 100)
        h = random.randint(-100, 100)
        r = rect.SDL_Rect(0, 0, w, h)
        empty = rect.SDL_RectEmpty(r)
        assert empty if not (w > 0 and h > 0) else not empty
    with pytest.raises(AttributeError):
        rect.SDL_RectEmpty("Test")

def test_SDL_RectEquals():
    r1 = rect.SDL_Rect(0, 0, 1, 1)
    r2 = rect.SDL_Rect(0, 0, 1, 1)
    assert rect.SDL_RectEquals(r1, r2)
    r2 = rect.SDL_Rect(-1, 2, 1, 1)
    assert not rect.SDL_RectEquals(r1, r2)
    r2 = rect.SDL_Rect(0, 0, 1, 2)
    assert not rect.SDL_RectEquals(r1, r2)
    # Test exceptions
    with pytest.raises(AttributeError):
        rect.SDL_RectEquals("Test", r2)
    with pytest.raises(AttributeError):
        rect.SDL_RectEquals(r1, None)

def test_SDL_UnionRect():
    tests = [
        [(0, 0, 10, 10), (20, 20, 10, 10), (0, 0, 30, 30)],
        [(0, 0, 0, 0), (20, 20, 10, 10), (20, 20, 10, 10)],
        [(-200, -4, 450, 33), (20, 20, 10, 10), (-200, -4, 450, 34)],
        [(0, 0, 15, 16), (20, 20, 0, 0), (0, 0, 15, 16)]
    ]
    out = rect.SDL_Rect()
    for rect1, rect2, expected in tests:
        r1 = rect.SDL_Rect(*rect1)
        r2 = rect.SDL_Rect(*rect2)
        rect.SDL_UnionRect(r1, r2, byref(out))
        assert (out.x, out.y, out.w, out.h) == expected
    # Test exceptions
    with pytest.raises((AttributeError, TypeError)):
        rect.SDL_UnionRect(None, None)
    with pytest.raises((AttributeError, TypeError)):
        rect.SDL_UnionRect("Test", r2)
    with pytest.raises((AttributeError, TypeError)):
        rect.SDL_UnionRect(r1, None)
    with pytest.raises((AttributeError, TypeError)):
        rect.SDL_UnionRect(r1, "Test")

def test_SDL_IntersectRectAndLine():
    tests = [
        [(0, 0, 0, 0), (-5, -5, 5, 5), SDL_FALSE, None],
        [(0, 0, 2, 2), (-1, -1, 3, 3), SDL_TRUE, (0, 0, 1, 1)],
        [(-4, -4, 14, 14), (8, 22, 8, 33), SDL_FALSE, None]
    ]
    for rect1, line, expected_ret, expected_coords in tests:
        r = rect.SDL_Rect(*rect1)
        x1, y1, x2, y2 = line
        x1, y1, x2, y2 = c_int(x1), c_int(y1), c_int(x2), c_int(y2)
        ret = rect.SDL_IntersectRectAndLine(
            r, byref(x1), byref(y1), byref(x2), byref(y2))
        assert ret == expected_ret
        if ret == SDL_TRUE:
            assert (x1.value, y1.value, x2.value, y2.value) == expected_coords

def test_SDL_EnclosePoints():
    tests = [
        [rect.SDL_Rect(0, 0, 10, 10), SDL_TRUE, (0, 0, 6, 8)],
        [rect.SDL_Rect(-10, -10, 3, 3), SDL_FALSE, (0, 0, 0, 0)],
        [None, SDL_TRUE, (0, 0, 6, 8)],
    ]
    pt1, pt2 = [rect.SDL_Point(0, 0), rect.SDL_Point(5, 7)]
    points = to_ctypes([pt1, pt2], rect.SDL_Point)
    res = rect.SDL_Rect()
    for clip, expected_ret, expected_rect in tests:
        clip_p = byref(clip) if isinstance(clip, rect.SDL_Rect) else None
        ret = rect.SDL_EnclosePoints(points, 2, clip_p, byref(res))
        assert ret == expected_ret
        r = rect.SDL_Rect(*expected_rect)
        assert res == r if ret == SDL_TRUE else res != r
    # Test with no points
    ret = rect.SDL_EnclosePoints(None, 0, None, byref(res))
    assert not ret
    assert res != rect.SDL_Rect()
    # Test expceptions
    with pytest.raises(TypeError):
        rect.SDL_EnclosePoints(None, None)
    with pytest.raises(TypeError):
        rect.SDL_EnclosePoints("Test", None)
    with pytest.raises(TypeError):
        rect.SDL_EnclosePoints((1, 2, 3), None)
    with pytest.raises(TypeError):
        rect.SDL_EnclosePoints((None,), None)

def test_SDL_HasIntersection():
    tests = [
        [(0, 0, 0, 0), (0, 0, 0, 0), SDL_FALSE],
        [(0, 0, -200, 200), (0, 0, -200, 200), SDL_FALSE],
        [(0, 0, 10, 10), (-5, 5, 10, 2), SDL_TRUE],
        [(0, 0, 10, 10), (-5, -5, 10, 2), SDL_FALSE],
        [(0, 0, 10, 10), (-5, -5, 2, 10), SDL_FALSE],
        [(0, 0, 10, 10), (-5, -5, 5, 5), SDL_FALSE],
        [(0, 0, 10, 10), (-5, -5, 6, 6), SDL_TRUE]
    ]
    for rect1, rect2, expected in tests:
        r1 = rect.SDL_Rect(*rect1)
        r2 = rect.SDL_Rect(*rect2)
        assert rect.SDL_HasIntersection(r1, r2) == expected

def test_SDL_IntersectRect():
    tests = [
        [(0, 0, 0, 0), (0, 0, 0, 0), SDL_FALSE, None],
        [(0, 0, -200, 200), (0, 0, -200, 200), SDL_FALSE, None],
        [(0, 0, 10, 10), (-5, 5, 10, 2), SDL_TRUE, (0, 5, 5, 2)],
        [(0, 0, 10, 10), (-5, -5, 10, 2), SDL_FALSE, None],
        [(0, 0, 10, 10), (-5, -5, 2, 10), SDL_FALSE, None],
        [(0, 0, 10, 10), (-5, -5, 5, 5), SDL_FALSE, None],
        [(0, 0, 10, 10), (-5, -5, 6, 6), SDL_TRUE, (0, 0, 1, 1)]
    ]
    res = rect.SDL_Rect()
    for rect1, rect2, expected_ret, expected_rect in tests:
        r1 = rect.SDL_Rect(*rect1)
        r2 = rect.SDL_Rect(*rect2)
        ret = rect.SDL_IntersectRect(r1, r2, byref(res))
        assert ret == expected_ret
        if ret == SDL_TRUE:
            res == rect.SDL_Rect(*expected_rect)

def test_SDL_PointInRect():
    r = rect.SDL_Rect(0, 0, 10, 10)
    inside = [(0, 0), (4, 2)]
    outside = [(10, 10), (10, 3), (3, 10), (-1, -3)]
    for x, y in inside:
        p = rect.SDL_Point(x, y)
        assert rect.SDL_PointInRect(p, r)
    for x, y in outside:
        p = rect.SDL_Point(x, y)
        assert not rect.SDL_PointInRect(p, r)
