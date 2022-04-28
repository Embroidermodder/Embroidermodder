from ctypes import Structure, c_int, c_float, POINTER
from .dll import _bind
from .stdinc import SDL_bool

__all__ = [
    # Structs
    "SDL_Point", "SDL_FPoint", "SDL_Rect", "SDL_FRect",

    # Macro Functions
    "SDL_PointInRect", "SDL_RectEmpty", "SDL_RectEquals",

    # Functions
    "SDL_HasIntersection", "SDL_IntersectRect", "SDL_UnionRect",
    "SDL_EnclosePoints", "SDL_IntersectRectAndLine"
]


class SDL_Point(Structure):
    _fields_ = [("x", c_int), ("y", c_int)]

    def __init__(self, x=0, y=0):
        super(SDL_Point, self).__init__()
        self.x = x
        self.y = y

    def __repr__(self):
        return "SDL_Point(x=%d, y=%d)" % (self.x, self.y)

    def __copy__(self):
        return SDL_Point(self.x, self.y)

    def __deepcopy__(self, memo):
        return SDL_Point(self.x, self.y)

    def __eq__(self, pt):
        return self.x == pt.x and self.y == pt.y

    def __ne__(self, pt):
        return self.x != pt.x or self.y != pt.y


class SDL_FPoint(Structure):
    _fields_ = [("x", c_float), ("y", c_float)]

    def __init__(self, x=0.0, y=0.0):
        super(SDL_FPoint, self).__init__()
        self.x = x
        self.y = y

    def __repr__(self):
        return "SDL_FPoint(x=%.3f, y=%.3f)" % (self.x, self.y)

    def __copy__(self):
        return SDL_FPoint(self.x, self.y)

    def __deepcopy__(self, memo):
        return SDL_FPoint(self.x, self.y)

    def __eq__(self, pt):
        return self.x == pt.x and self.y == pt.y

    def __ne__(self, pt):
        return self.x != pt.x or self.y != pt.y


class SDL_Rect(Structure):
    _fields_ = [("x", c_int), ("y", c_int),
                ("w", c_int), ("h", c_int)]

    def __init__(self, x=0, y=0, w=0, h=0):
        super(SDL_Rect, self).__init__()
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def __repr__(self):
        return "SDL_Rect(x=%d, y=%d, w=%d, h=%d)" % (self.x, self.y, self.w,
                                                     self.h)

    def __copy__(self):
        return SDL_Rect(self.x, self.y, self.w, self.h)

    def __deepcopy__(self, memo):
        return SDL_Rect(self.x, self.y, self.w, self.h)

    def __eq__(self, rt):
        origin_equal = self.x == rt.x and self.y == rt.y
        size_equal = self.w == rt.w and self.h == rt.h
        return origin_equal and size_equal

    def __ne__(self, rt):
        origin_equal = self.x == rt.x and self.y == rt.y
        size_equal = self.w == rt.w and self.h == rt.h
        return not (origin_equal and size_equal)


class SDL_FRect(Structure):
    _fields_ = [("x", c_float), ("y", c_float),
                ("w", c_float), ("h", c_float)]

    def __init__(self, x=0.0, y=0.0, w=0.0, h=0.0):
        super(SDL_FRect, self).__init__()
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def __repr__(self):
        dims = (self.x, self.y, self.w, self.h)
        return "SDL_FRect(x=%.3f, y=%.3f, w=%.3f, h=%.3f)" % dims

    def __copy__(self):
        return SDL_FRect(self.x, self.y, self.w, self.h)

    def __deepcopy__(self, memo):
        return SDL_FRect(self.x, self.y, self.w, self.h)

    def __eq__(self, rt):
        origin_equal = self.x == rt.x and self.y == rt.y
        size_equal = self.w == rt.w and self.h == rt.h
        return origin_equal and size_equal

    def __ne__(self, rt):
        origin_equal = self.x == rt.x and self.y == rt.y
        size_equal = self.w == rt.w and self.h == rt.h
        return not (origin_equal and size_equal)


SDL_RectEmpty = lambda x: ((not x) or (x.w <= 0) or (x.h <= 0))
SDL_RectEquals = lambda a, b: ((a.x == b.x) and (a.y == b.y) and
                               (a.w == b.w) and (a.h == b.h))
SDL_PointInRect = lambda p, r: ((p.x >= r.x) and (p.x < (r.x + r.w)) and
                                (p.y >= r.y) and (p.y < (r.y + r.h)))
SDL_HasIntersection = _bind("SDL_HasIntersection", [POINTER(SDL_Rect), POINTER(SDL_Rect)], SDL_bool)
SDL_IntersectRect = _bind("SDL_IntersectRect", [POINTER(SDL_Rect), POINTER(SDL_Rect), POINTER(SDL_Rect)], SDL_bool)
SDL_UnionRect = _bind("SDL_UnionRect", [POINTER(SDL_Rect), POINTER(SDL_Rect), POINTER(SDL_Rect)])
SDL_EnclosePoints = _bind("SDL_EnclosePoints", [POINTER(SDL_Point), c_int, POINTER(SDL_Rect), POINTER(SDL_Rect)], SDL_bool)
SDL_IntersectRectAndLine = _bind("SDL_IntersectRectAndLine", [POINTER(SDL_Rect), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)], SDL_bool)
