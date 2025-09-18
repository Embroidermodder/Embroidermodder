To Do
=====

2.0 alpha1
----------

#. WIP - Statistics from 1.0, needs histogram
#. WIP - Saving DST/PES/JEF (varga)
#. WIP - Saving CSV/SVG (rt) + CSV read/write UNKNOWN interpreted as COLOR bug

2.0 alpha2
----------

#. TODO - Notify user of data loss if not saving to an object format.
#. TODO - Import Raster Image
#. TODO - SNAP/ORTHO/POLAR
#. TODO - Layer Manager + LayerSwitcher DockWidget
#. TODO - Reading DXF

2.0 alpha3
----------

#. TODO - Writing DXF
#. DONE - Up and Down keys cycle thru commands in the command prompt
#. TODO - Amount of Thread & Machine Time Estimation (also allow customizable times for setup, color changes, manually trimming jump threads, etc...that way a realistic total time can be estimated)
#. TODO - Otto Theme Icons - whatsthis icon doesn't scale well, needs redone
#. TODO - embroidermodder2.ico 16 x 16 looks horrible

2.0 alpha4
----------

#. WIP - CAD Command: Arc (rt)
#. TODO - Load/Save Menu/Toolbars configurations into settings.ini
#. TODO - automate changelog and write to a javascript file for the docs: git log --pretty=tformat:'<a href="https://github.com/Embroidermodder/Embroidermodder/commit/%H">%s</a>'

2.0 beta1
---------

#. TODO - Custom Filter Bug - doesn't save changes in some cases
#. TODO - Cannot open file with # in name when opening multiple files (works fine when opening the single file)
#. TODO - Closing Settings Dialog with the X in the window saves settings rather than discards them
#. WIP - Advanced Printing
#. TODO - Filling Algorithms (varga)
#. TODO - Otto Theme Icons - beta (rt) - Units, Render, Selectors

2.0 rc1
-------

#. TODO - QDoc Comments
#. TODO - Review KDE4 Thumbnailer
#. TODO - Documentation for libembroidery & formats
#. TODO - HTML Help files
#. TODO - Update language translations
#. TODO - CAD Command review: line
#. TODO - CAD Command review: circle
#. TODO - CAD Command review: rectangle
#. TODO - CAD Command review: polygon
#. TODO - CAD Command review: polyline
#. TODO - CAD Command review: point
#. TODO - CAD Command review: ellipse
#. TODO - CAD Command review: arc
#. TODO - CAD Command review: distance
#. TODO - CAD Command review: locatepoint
#. TODO - CAD Command review: move
#. TODO - CAD Command review: rgb
#. TODO - CAD Command review: rotate
#. TODO - CAD Command review: scale
#. TODO - CAD Command review: singlelinetext
#. TODO - CAD Command review: star
#. TODO - Clean up all compiler warning messages, right now theres plenty :P

2.0 Release
-----------

#. TODO - tar.gz archive
#. TODO - zip archive
#. TODO - Debian Package (rt)
#. TODO - NSIS Installer (rt)
#. TODO - Mac Bundle?
#. TODO - press release

2.x/Ideas
---------

#. TODO - libembroidery.mk for MXE project (refer to qt submodule packages for qmake based building. Also refer to plibc.mk for example of how write an update macro for github.)
#. TODO - libembroidery safeguard for all writers - check if the last stitch is an END stitch. If not, add an end stitch in the writer and modify the header data if necessary.
#. TODO - Cut/Copy - Allow Post-selection
#. TODO - CAD Command: Array
#. TODO - CAD Command: Offset
#. TODO - CAD Command: Extend
#. TODO - CAD Command: Trim
#. TODO - CAD Command: BreakAtPoint
#. TODO - CAD Command: Break2Points
#. TODO - CAD Command: Fillet
#. TODO - CAD Command: Chamfer
#. TODO - CAD Command: Split
#. TODO - CAD Command: Area
#. TODO - CAD Command: Time
#. TODO - CAD Command: PickAdd
#. TODO - CAD Command: Product
#. TODO - CAD Command: Program
#. TODO - CAD Command: ZoomFactor
#. TODO - CAD Command: GripHot
#. TODO - CAD Command: GripColor & GripCool
#. TODO - CAD Command: GripSize
#. TODO - CAD Command: Highlight
#. TODO - CAD Command: Units
#. TODO - CAD Command: Grid
#. TODO - CAD Command: Find
#. TODO - CAD Command: Divide
#. TODO - CAD Command: ZoomWindow (Move out of view.cpp)
#. TODO - Command: Web (Generates Spiderweb patterns)
#. TODO - Command: Guilloche (Generates Guilloche patterns)
#. TODO - Command: Celtic Knots
#. TODO - Command: Knotted Wreath
#. TODO - Lego Mindstorms NXT/EV3 ports and/or commands.
#. TODO - native function that flashes the command prompt to get users attention when using the prompt is required for a command.
#. TODO - libembroidery-composer like app that combines multiple files into one.
#. TODO - Settings Dialog, it would be nice to have it notify you when switching tabs that a setting has been changed. Adding an Apply button is what would make sense for this to happen. 
#. TODO - Keyboard Zooming/Panning
#. TODO - G-Code format?
#. TODO - 3D Raised Embroidery
#. TODO - Gradient Filling Algorithms
#. TODO - Stitching Simulation
#. TODO - RPM packages?
#. TODO - Reports?
#. TODO - Record and Playback Commands
#. TODO - Settings option for reversing zoom scrolling direction
#. TODO - Qt GUI for libembroidery-convert
#. TODO - EPS format? Look at using Ghostscript as an optional add-on to libembroidery...
#. TODO - optional compile option for including LGPL/GPL libs etc... with warning to user about license requirements.
#. TODO - Realistic Visualization - Bump Mapping/OpenGL/Gradients?
#. TODO - Stippling Fill
#. TODO - User Designed Custom Fill
#. TODO - Honeycomb Fill
#. TODO - Hilburt Curve Fill
#. TODO - Sierpinski Triangle fill
#. TODO - Circle Grid Fill
#. TODO - Spiral Fill
#. TODO - Offset Fill
#. TODO - Brick Fill
#. TODO - Trim jumps over a certain length.
#. TODO - FAQ about setting high number of jumps for more controlled trimming.
#. TODO - Minimum stitch length option. (Many machines also have this option too)
#. TODO - Add 'Design Details' functionality to libembroidery-convert
#. TODO - Add 'Batch convert many to one format' functionality to libembroidery-convert
#. TODO - EmbroideryFLOSS - Color picker that displays catalog numbers and names.
#. TODO - emscripten/javascript port of libembroidery

Embedded
--------

#. TODO - Fix emb-outline files
#. TODO - Fix thread-color files
#. TODO - Logging of Last Stitch Location to External USB Storage(commonly available and easily replaced) ...wait until TRE is available to avoid rework
#. TODO - inotool.org - seems like the logical solution for Nightly/CI builds
#. TODO - Smoothieboard experiments

libembroidery-tests
-------------------

#. TODO - looping test that reads 10 times while running valgrind. See embPattern_loadExternalColorFile() Arduino leak note for more info.

