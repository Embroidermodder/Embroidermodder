`sdl2.ext.array` - Tools for Working with `ctypes` Arrays
=========================================================

This module provides a number of utilites for accessing data in ``ctypes``
arrays and converting ``ctypes`` arrays into common Python formats.

.. warning::
   These functions are primarily meant for internal use in PySDL2, and should
   be treated as experimental.


Providing read-write access for sequential data
-----------------------------------------------
Two classes allow you to access sequential data in different ways. The
:class:`CTypesView` provides byte-wise access to iterable objects and allows
you to convert the object representation to matching byte-widths for
:mod:`ctypes` or other modules.

Depending on the the underlying object and the chosen size of each particular
item of the object, the :class:`CTypesView` allows you to operate directly
on different representations of the object's contents. ::

    >>> text = bytearray("Hello, I am a simple ASCII string!")
    >>> ctview = CTypesView(text, itemsize=1)
    >>> ctview.view[0] = 0x61
    >>> print(text)
    aello, I am a simple ASCII string!"
    >>> ctview.to_uint16()[3] = 0x6554
    >>> print(text)
    aello,Te am a simple ASCII string!"

The snippet above provides a single-byte sized view on a :func:`bytearray`
object. Afterwards, the first item of the view is changed, which causes a
change on the :func:`bytearray`, on the first item as well, since both, the
:class:`CTypesView` and the :func:`bytearray` provide a byte-wise access to
the contents.

By using :meth:`CTypesView.to_uint16()`, we change the access representation to
a 2-byte unsigned integer :mod:`ctypes` pointer and change the fourth 2-byte
value, *I* to something else. ::

    >>> text = bytearray("Hello, I am a simple ASCII string!")
    >>> ctview = CTypesView(text, itemsize=2)
    >>> ctview.view[0] = 0x61
    >>> print(text)
    aello, I am a simple ASCII string!"
    >>> ctview.to_uint16()[3] = 0x6554
    >>> print(text)    aello,Te am a simple ASCII string!"

If the encapsuled object does not provide a (writable) :func:`buffer`
interface, but is iterable, the :class:`CTypesView` will create an
internal copy of the object data using Python's :mod:`array` module and
perform all operations on that copy. ::

    >>> mylist = [18, 52, 86, 120, 154, 188, 222, 240]
    >>> ctview = CTypesView(mylist, itemsize=1, docopy=True)
    >>> print(ctview.object)
    array('B', [18, 52, 86, 120, 154, 188, 222, 240])
    >>> ctview.view[3] = 0xFF
    >>> print(mylist)
    [18, 52, 86, 120, 154, 188, 222, 240]
    >>> print(ctview.object)
    array('B', [18, 52, 86, 255, 154, 188, 222, 240])

As for directly accessible objects, you can define your own itemsize to
be used. If the iterable does not provide a direct byte access to their
contents, this won't have any effect except for resizing the item
widths. ::

    >>> mylist = [18, 52, 86, 120, 154, 188, 222, 240]
    >>> ctview = CTypesView(mylist, itemsize=4, docopy=True)
    >>> print(ctview.object)
    array('I', [18L, 52L, 86L, 120L, 154L, 188L, 222L, 240L])

Accessing data over multiple dimensions
---------------------------------------
The second class, :class:`MemoryView` provides an interface to access
data over multiple dimensions. You can layout and access a simple
byte stream over e.g. two or more axes, providing a greater flexibility
for functional operations and complex data.

Let's assume, we are reading image data from a file stream into some buffer
object and want to access and manipulate the image data. Images feature two
axes, one being the width, the other being the height, defining a rectangular
graphics area.

When we read all data from the file, we have an one-dimensional view of the
image graphics. The :class:`MemoryView` allows us to define a
two-dimensional view over the image graphics, so that we can operate on
both, rows and columns of the image. ::

    >>> imagedata = bytearray("some 1-byte graphics data")
    >>> view = MemoryView(imagedata, 1, (5, 5))
    >>> print(view)
    [[s, o, m, e,  ], [1, -, b, y, t], [e,  , g, r, a], [p, h, i, c, s], [ , d, a, t, a]]
    >>> for row in view:
    ...     print(row)
    ...
    [s, o, m, e,  ]
    [1, -, b, y, t]
    [e,  , g, r, a]
    [p, h, i, c, s]
    [ , d, a, t, a]
    >>> for row in view:
    ...    row[1] = "X"
    ...    print row
    ...
    [s, X, m, e,  ]
    [1, X, b, y, t]
    [e, X, g, r, a]
    [p, X, i, c, s]
    [ , X, a, t, a]
    >>> print(imagedata)
    sXme 1XbyteXgrapXics Xata

On accessing a particular dimension of a :class:`MemoryView`, a new
:class:`MemoryView` is created, if it does not access a single
element. ::

    >>> firstrow = view[0]
    >>> type(firstrow)
    <class 'sdl2.ext.array.MemoryView'>
    >>> type(firstrow[0])
    <type 'bytearray'>

A :class:`MemoryView` features, similar to Python's builtin
:class:`memoryview`, dimensions and strides, accessible via the
:attr:`MemoryView.ndim` and :attr:`MemoryView.strides` attributes.

    >>> view.ndim
    2
    >>> view.strides
    (5, 5)

The :attr:`MemoryView.strides`, which have to be passed on creating a
new :class:`MemoryView`, define the layout of the data over different
dimensions. In the example above, we created a 5x5 two-dimensional view
to the image graphics. ::

    >>> twobytes = MemoryView(imagedata, 2, (5, 1))
    >>> print(twobytes)
    [[sX, me,  1, Xb, yt], [eX, gr, ap, Xi, cs]]


Array API
---------

.. automodule:: sdl2.ext.array
	:members:
