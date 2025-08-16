# The Embroidermodder Project and Team

The *Embroidermodder 2* project is a collection of small software utilities for
manipulating, converting and creating embroidery files in all major embroidery
machine formats. The program *Embroidermodder 2* itself is a larger graphical
user interface (GUI) which is at the heart of the project.

The tools and associated documents are:

* This website (www.libembroidery.org), which is maintained [docs-repo]_.
* The manual [manual-link]_ covering all these projects which is maintained seperately as LaTeX [manual-src]_.
* The GUI (embroidermodder), maintained [gui]_.
* The core library of low-level functions: [libembroidery-src]_.
* The CLI `embroider` which is part of [libembroidery-src]_.
* Mobile embroidery format viewers and tools [EmbroideryMobile-src]_.
* Specs for an open hardware embroidery machine called Embroiderbot (not started yet) which is also part of [libembroidery-src]_.

.. [docs-repo] https://github.com/Embroidermodder/docs
.. [manual-link] https://www.libembroidery.org/embroidermodder_2.0_manual.pdf
.. [manual-src] https://github.com/Embroidermodder/emrm
.. [gui] https://github.com/Embroidermodder/embroidermodder
.. [libembroidery-src] https://github.com/Embroidermodder/libembroidery
.. [EmbroideryMobile-src] https://github.com/Embroidermodder/embroiderymobile

They are all tools to make the standard user experience of working with an
embroidery machine better without expensive software which is locked to specific
manufacturers and formats. But ultimately we hope that the core *Embroidermodder 2*
is a practical, ever-present tool in larger workshops, small cottage industry workshops
and personal hobbyist's bedrooms.

Embroidermodder 2 is licensed under the zlib license and we aim to keep all of
our tools open source and free of charge. If you would like to support the
project check out our `Open Collective <https://opencollective.com/embroidermodder>`_
group. If you would like to help, please
join us on GitHub. This document is written as developer training as well
helping new users (see the last sections) so this is the place to learn how
to start changing the code.

The Embroidermodder Team is the collection of people who've submitted
patches, artwork and documentation to our three projects.
The team was established by Jonathan Greig and Josh Varga.
The full list of contributors who wish to be credited is
here: https://www.libembroidery.org/docs/credits/

## Core Development Team

Embroidermodder 2:

  * Jonathan Greig [https://github.com/redteam316](https://github.com/redteam316)
  * Josh Varga [https://github.com/JoshVarga](https://github.com/JoshVarga)
  * Robin Swift [https://github.com/robin-swift](https://github.com/robin-swift)

Embroidermodder 1:

  * Josh Varga [https://github.com/JoshVarga](https://github.com/JoshVarga)
  * Mark Pontius [http://sourceforge.net/u/mpontius/profile](http://sourceforge.net/u/mpontius/profile)

## History

Embroidermodder 1 was started by Mark Pontius in 2004 while staying up all night
with his son in his first couple months. When Mark returned to his day job,
he lacked the time to continue the project. Mark made the decision to focus on his
family and work, and in 2005, Mark gave full control of the project to Josh Varga
so that Embroidermodder could continue its growth.

Embroidermodder 2 was conceived in mid 2011 when Jonathan Greig and Josh Varga
discussed the possibility of making a cross-platform version. It is currently in
active development and will run on GNU/Linux, Mac OS X, Microsoft Windows and Raspberry Pi.

The source code and binaries for Embroidermodder 1 were hosted on Sourceforge, but
due to link rot we've lost them.

!!! todo
    upload a backup here.

The source code for Embroidermodder
2 was moved to GitHub ([https://github.com/Embroidermodder/Embroidermodder](https://github.com/Embroidermodder/Embroidermodder)) on July 18, 2013.

This website was moved to
GitHub ([https://github.com/Embroidermodder/www.libembroidery.org](https://github.com/Embroidermodder/www.libembroidery.org)) on September 9, 2013. Due to us losing the domain name it was renamed to
``www.libembroidery.org`` from ``www.embroidermodder.org``.

The libembroidery library (https://github.com/Embroidermodder/libembroidery)
became a seperate project in 2018 as a way of supporting other frontends with the
same file parsing and geometry routines.
