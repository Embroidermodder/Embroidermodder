.. currentmodule:: sdl2.ext

Designing component-oriented user interfaces
============================================

.. warning::
   This module is deprecated, and may be removed in a future release of PySDL2.

User interface elements within :mod:`sdl2.ext` are simple
:class:`Sprite` objects, which are enhanced by certain input hooks; as such,
they are not classes on their own, but implemented as mixins. The user input
itself is handled by an :class:`UIProcessor` object, which takes care of
delegating input events, such as mouse movements, clicks and keyboard input,
to the correct UI element.

Depending on the event type (e.g. pressing a mouse button), the UIProcessor
will execute its matching method (e.g. ``mousedown()``) with only those UI
elements, which support the event type.

.. image:: images/uiprocessing.png

.. _ui-elem-types:


UI element types
----------------
Every :class:`sdl2.ext` UI element is a simple :class:`Sprite` object, to
which additional attributes and methods are bound.

Every UI element features the following attributes

``element.uitype``

   The ``uitype`` attribute can have one of the following values,
   identifying the UI element:

   * ``BUTTON`` - a UI element, which can react on mouse input
   * ``CHECKBUTTON`` - as ``BUTTON``, but it retains its state on clicks
   * ``TEXTENTRY`` - a UI element that reacts on keyboard input

``element.events``

   A dictionary containing the SDL2 event mappings. Each supported SDL2 event
   (e.g. ``SDL_MOUSEMOTION``) is associated with a bound
   :class:`EventHandler` acting as callback for user code
   (e.g. ``mousemotion()``).

Depending on the exact type of the element, it will feature additional methods
and attributes explained below.

Button elements
^^^^^^^^^^^^^^^
``BUTTON`` UI elements feature a ``state`` attribute, which can be one of the
following values.

======== =====================================================================
state    Description
======== =====================================================================
RELEASED Indicates that the UI element is not pressed.
HOVERED  Indicates that the mouse cursor is currently hovering the UI element.
PRESSED  Indicates that a mouse button is pressed on the UI element.
======== =====================================================================

``BUTTON`` UI elements react with the following event handlers on events:

``button.motion(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if the mouse moves around while
  being over the ``BUTTON``.

``button.pressed(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if a mouse button is pressed on
  the ``BUTTON``.

``button.released(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if a mouse button is released on
  the ``BUTTON``.

``button.click(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if a mouse button is pressed and
  released on the ``BUTTON``.

Besides the ``BUTTON`` a special ``CHECKBUTTON`` UI element type exists,
which enhances the ``BUTTON`` bindings by an additional ``checked`` attribute.
The ``checked`` attribute switches its status (``False`` to ``True`` and
``True``  to ``False``) every time the UI element is clicked.

Text input elements
^^^^^^^^^^^^^^^^^^^
``TEXTENTRY`` elements react on text input, once they are activated. Text being
input, once a ``TEXTENTRY`` has been activated, is stored in its ``text``
attribute.

The ``TEXTENTRY`` reacts with the following event handlers on events:

``textentry.motion(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if the mouse moves around while
  being over the ``TEXTENTRY``.

``textentry.pressed(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if a mouse button is pressed on
  the ``TEXTENTRY``.

``textentry.released(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked, if a mouse button is released on
  the ``TEXTENTRY``.

``textentry.keydown(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked on pressing a key.

``textentry.keyup(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked on releasing a key.

``textentry.input(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked on text input events.
  Text input events are automatically created, once the :class:`UIProcessor`
  activates a ``TEXTENTRY`` UI element.

``textentry.editing(event : sdl2.events.SDL_Event)``

  An :class:`EventHandler` that is invoked on text editing events. Text
  editing events are automatically created, once the :class:`UIProcessor`
  activates a ``TEXTENTRY`` UI element.

  Text editing events are however only raised, if an IME system is involved,
  which combines glyphs and symbols to characters or word fragments.

API
---

.. class:: UIFactory(spritefactory : SpriteFactory[, **kwargs])

   A factory class for creating UI elements. The :class:`UIFactory`
   allows you to create UI elements based on the
   :class:`Sprite` class. To do this, it requires a :class:`SpriteFactory`,
   which will create the sprites, to which the :class:`UIFactory` then binds
   the additional methods and attributes.

   The additional *kwargs* are used as default arguments for creating
   **sprites** within the factory methods.

   .. attribute:: default_args

      A dictionary containing the default arguments to be passed to the
      sprite creation methods of the bound :class:`SpriteFactory`.

   .. attribute:: spritefactory

      The :class:`SpriteFactory` being used for creating new :class:`Sprite`
      objects.

   .. method:: create_button(**kwargs) -> Sprite

      Creates a new button UI element.

      *kwargs* are the arguments to be passed for the sprite
      construction and can vary depending on the sprite type.
      See :meth:`SpriteFactory.create_sprite()` for further details.

   .. method:: create_check_button(**kwargs) -> Sprite

      Creates a new checkbutton UI element.

      *kwargs* are the arguments to be passed for the sprite
      construction and can vary depending on the sprite type.
      See :meth:`SpriteFactory.create_sprite()` for further details.

   .. method:: create_text_entry(**kwargs) -> Sprite

      Creates a new textentry UI element.

      *kwargs* are the arguments to be passed for the sprite
      construction and can vary depending on the sprite type.
      See :meth:`SpriteFactory.create_sprite()` for further details.

   .. method:: from_color(color : object , size) -> Sprite

      Creates a UI element with a specific color.

      *uitype* must be one of the supported :ref:`ui-elem-types` classifying
      the type of UI element to be created.

   .. method:: from_image(uitype : int, fname : str) -> Sprite

      Creates a UI element from an image file. The image must be
      loadable via :func:`load_image()`.

      *uitype* must be one of the supported :ref:`ui-elem-types` classifying
      the type of UI element to be created.

   .. method:: from_object(uitype : int, obj: object) -> Sprite

      Creates a UI element from an object. The object will be passed through
      :func:`sdl2.rwops_from_object()` in order to try to load image data from
      it.

      *uitype* must be one of the supported :ref:`ui-elem-types` classifying
      the type of UI element to be created.

   .. method:: from_surface(uitype : int,  surface : SDL_Surface[, free=False]) -> Sprite

      Creates a UI element from the passed
      :class:`sdl2.surface.SDL_Surface`. If *free* is set to
      ``True``, the passed *surface* will be freed automatically.

      *uitype* must be one of the supported :ref:`ui-elem-types` classifying
      the type of UI element to be created.

.. class:: UIProcessor()

   A processing system for user interface elements and events.

   .. attribute:: handlers

      A dict containing the mapping of SDL2 events to the available
      :class:`EventHandler` bindings of the :class:`UIProcessor`.

   .. method:: activate(component : object) -> None

      Activates a UI control to receive text input.

   .. method:: deactivate(component : object) -> None

      Deactivate the currently active UI control.

   .. method:: passevent(component : object, event : SDL_Event) -> None

      Passes the *event* to a *component* without any additional checks or
      restrictions.

   .. method:: mousemotion(component : object, event : SDL_Event) -> None

      Checks, if the event's motion position is on the *component* and
      executes the component's event handlers on demand. If the motion event
      position is not within the area of the *component*, nothing will be
      done. In case the component is a ``BUTTON``, its :attr:`state` will be
      adjusted to reflect, if it is currently hovered or not.

   .. method:: mousedown(component : object, event : SDL_Event) -> None

      Checks, if the event's button press position is on the *component* and
      executes the component's event handlers on demand. If the button press
      position is not within the area of the component, nothing will be done.

      In case the component is a ``BUTTON``, its :attr:`state`
      will be adjusted to reflect, if it is currently pressed or not.

      In case the component is a ``TEXTENTRY`` and the pressed button is
      the primary mouse button, the component will be marked as the next
      control to activate for text input.

   .. method:: mouseup(self, component, event) -> None

      Checks, if the event's button release position is on the *component* and
      executes the component's event handlers on demand. If the button release
      position is not within the area of the component, nothing will be done.

      In case the component is a ``BUTTON``, its :attr:`state`
      will be adjusted to reflect, whether it is hovered or not.

      If the button release followed a button press on the same component and
      if the button is the primary button, the ``click()`` event handler is
      invoked, if the component is a ``BUTTON``.

   .. method:: dispatch(obj : object, event : SDL_Event) -> None

      Passes an event to the given object. If *obj* is a
      :class:`World` object, UI relevant components will receive
      the event, if they support the event type. If *obj* is a single object,
      ``obj.events`` **must** be a dict consisting of SDL event type
      identifiers and :class:`EventHandler` instances bound
      to the object. If *obj* is a iterable, such as a list or set, every
      item within *obj* **must** feature an ``events`` attribute as
      described above.

   .. method:: process(world : World, components : iterable) -> None

      The :class:`UIProcessor` class does not implement the `process()`
      method by default. Instead it uses :meth:`dispatch()` to send events
      around to components. :meth:`process()` does nothing.
