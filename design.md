This file is for ideas copied over from GitHub Issues that are no longer current.

## Design

Embroidermodder 2 was written in C++/Qt5 and it was far too complex. We had issues with people
not able to build from source because the Qt5 libraries were so ungainly. So I decided to do a
rewrite in C/SDL2 (originally FreeGLUT, but that was a mistake) with data stored as YAML. This
means linking 4-7 libraries depending on your system which are all well supported and widely available.

This is going well, although it's slow progress as I'm trying to keep track of the design while
also doing a ground up rewrite. I don't want to throw away good ideas. Since I also write code
for libembroidery my time is divided.
Overview of the UI rewrite

(Problems to be solved in brackets.)

It's not much to look at because I'm trying to avoid using an external widgets system, which
in turn means writing things like toolbars and menubars over. If you want to get the design
the actuator is the heart of it.

Without Qt5 we need a way of assigning signals with actions, so this is what I've got: the
user interacts with a UI element, this sends an integer to the actuator that does the thing
using the current state of the main_window struct of which we expect there to be exactly one
instance. The action is taken out by a jump table that calls the right function (most of
which are missing in action and not connected up properly). It also logs the number, along
with key parts of the main struct in the undo_history (an unsolved problem because we need
to decide how much data to copy over per action). This means undo, redo and repeat actions
can refer to this data.
