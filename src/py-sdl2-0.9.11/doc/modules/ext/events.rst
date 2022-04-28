.. currentmodule:: sdl2.ext

General purpose event handling routines
=======================================

.. class:: EventHandler(sender)

   A simple event handling class, which manages callbacks to be
   executed.

   The EventHandler does not need to be kept as separate instance, but
   is mainly intended to be used as attribute in event-aware class
   objects. ::

       >>> def myfunc(sender):
       ...     print("event triggered by %s" % sender)
       ...
       >>> class MyClass(object):
       ...     def __init__(self):
       ...         self.anevent = EventHandler(self)
       ...
       >>> myobj = MyClass()
       >>> myobj.anevent += myfunc
       >>> myobj.anevent()
       event triggered by <__main__.MyClass object at 0x801864e50>

   .. attribute:: callbacks

      A list of callbacks currently bound to the :class:`EventHandler`.

   .. attribute:: sender

      The responsible object that executes the :class:`EventHandler`.

   .. method:: add(callback : Callable)

      Adds a callback to the :class:`EventHandler`.

   .. method:: remove(callback : Callable)

      Removes a callback from the :class:`EventHandler`.

   .. method:: __call__(*args) -> [ ... ]

      Executes all connected callbacks in the order of addition,
      passing the :attr:`sender` of the :class:`EventHandler` as first
      argument and the optional args as second, third, ... argument to
      them.
      
      This will return a list containing the return values of the callbacks
      in the order of their execution.


.. class:: MPEventHandler(sender)

   An asynchronous event handling class based on :class:`EventHandler`,
   in which callbacks are executed in parallel. It is the responsibility
   of the caller code to ensure that every object used maintains a
   consistent state. The :class:`MPEventHandler` class will not apply
   any locks, synchronous state changes or anything else to the
   arguments or callbacks being used. Consider it a "fire-and-forget" event
   handling strategy.

   .. note::

      The :class:`MPEventHandler` relies on the :mod:`multiprocessing`
      module. If the module is not available in the target environment,
      a :exc:`sdl2.ext.compat.UnsupportedError` is raised.
      
      Also, please be aware of the restrictions that apply to the
      :mod:`multiprocessing` module; arguments and callback functions for
      example have to be pickable, etc.

   .. method:: __call__(*args) -> AsyncResult

      Executes all connected callbacks within a
      :class:`multiprocessing.pool.Pool`, passing the :attr:`sender` as first
      argument and the optional *args* as second, third, ... argument to them.
      
      This will return a :class:`multiprocessing.pool.AsyncResult` containing
      the return values of the callbacks in the order of their execution.
