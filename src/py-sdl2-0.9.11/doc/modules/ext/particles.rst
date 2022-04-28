.. module:: sdl2.ext.particles
   :synopsis: A simple particle system.

A simple particle system
========================

.. class:: ParticleEngine()

   A simple particle processing system. The :class:`ParticleEngine`
   takes care of creating, updating and deleting particles via callback
   functions. It only decreases the life of the particles by itself and
   marks them as dead, once the particle's life attribute has reached 0
   or below.

   .. attribute:: createfunc

      Function for creating new particles. The function needs to take
      two arguments, the ``world`` argument passed to :meth:`process()`
      and a list of the particles considered dead (:attr:`Particle.life`
      <= 0). ::

        def creation_func(world, deadparticles):
            ...

   .. attribute:: updatefunc

      Function for updating existing, living particles. The function
      needs to take two arguments, the ``world`` argument passed to
      :meth:`process()` and a :class:`set` of the still living
      particles. ::

        def update_func(world, livingparticles):
            ...

   .. attribute:: deletefunc

      Function for deleting dead particles. The function needs to take
      two arguments, the ``world`` argument passed to :meth:`process()`
      and a list of the particles considered dead (:attr:`Particle.life`
      <= 0). ::

        def deletion_func(world, deadparticles):
            ...

   .. method:: process(world : World, components : iterable) -> None

      Processes all particle components, decreasing their life by 1.

      Once the life of all particle components has been decreased
      properly and the particles considered dead (life <= 0) are
      identified, the creation, update and deletion callbacks are
      invoked.

      The creation callback takes the passed world as first and the
      list of dead particles as second argument. ::

        def particle_createfunc(world, list_of_dead_ones):
            ...

      Afterwards the still living particles are passed to the update
      callback, which also take the passed world as first and the
      living particles as set as second argument. ::

        def particle_updatefunc(world, set_of_living_ones):
            ...

      Finally, the dead particles need to be deleted in some way or
      another, which is done by the deletion callback, taking the
      passed world as first and the list of dead particles as second
      argument. ::

        def particle_deletefunc(world, list_of_dead_ones):
            ...

.. class:: Particle(x, y, life : int)

   A simple particle component type. It only contains information about
   a x- and y-coordinate and its current life time. The life time will
   be decreased by 1, every time the particle is processed by the
   :class:`ParticleEngine`.

   .. attribute:: x

      The x coordinate of the particle.

   .. attribute:: y

      The y coordinate of the particle.

   .. attribute:: life

      The remaining life time of the particle.

   .. attribute:: position

      The x- and y-coordinate of the particle as tuple.
