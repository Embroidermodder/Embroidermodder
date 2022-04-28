# Embroidermodder 2.0.0 Alpha

(UNDER MAJOR RESTRUCTURING, PLEASE WAIT FOR VERSION 2)

![Pylint score button](icons/rating.png).

Embroidermodder is a free machine embroidery application.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [our website](http://embroidermodder.org).

Embroidermodder 2 is very much a work in progress since we're doing a ground
up rewrite to an interface in Python using the GUI toolkit Tk.
The reasoning for this is detailed in the issues tab.

For a more in-depth look at what we are developing read
the [developer notes](#developers-notes). This discusses recent changes
in a less formal way than a changelog (since this software is in
development) and covers what we are about to try.

To see what we're focussing on at the moment check this table.

| *Date* | *Event* |
|----|----|
| April-June 2022 | Finish the conversion to Python/Tk |
| July-August 2022 | Finish all the targets in the Design, or assign them to 2.1. |
| September 2022 | Bugfixing, Testing, QA. libembroidery 1.0 will be released, then updates will slow down and the Embroidermodder 2 development version will be fixed to the API of this version. |
| October 2022 | Embroidermodder 2 is officially released. |

## Run

To run the development version, without installing, you can simply run these commands from the top directory:

    $ python3 -m pip install libembroidery
    $ cd src
    $ python3 -m embroidermodder

or, on Windows:

    > py -m pip install libembroidery
    > cd src
    > py -m embroidermodder

From this point on we assume that you have Python installed as `python3` but this same advice applies.

### Run Without Install

If you're using a machine that you don't have to ability to modify
(like some office workstations) you don't need to install libembroidery for
basic functioning.

In this mode the code will be able to read and write SVG embroidery files and do all of the necessary
editing. Should you then wish to convert the result without installing anything then we plan to have
a version of the convert utility as a part of the [embroidermodder.org](https://embroidermodder.org) site.
This would be a version of libembroidery compiled and run through emscripten with a simple form for
putting files in.

## Install

### Desktop

On most sytems the command:

    python3 -m pip install embroidermodder

will install the most up-to date released version.

Currently this is the 2.0-alpha, which will have a build code of
some kind.

### Mobile

These are currently unsupported (see iMobileViewer and Mobileviewer for
iOS and Android respectively), but after the Desktop version is
released we'll work on them.

The Mobile version will share some of the UI and all of the backend,
so development of the Desktop version will help us make both.

## Build

To run the development version just use

    $ python3 -m build
    $ cd dist
    $ python3 -m pip install embroidermodder-2*whl --force-reinstall

before using the command line boot (may be currently broken,
use the next method instead)

    $ embroidermodder

If you are on a shared computer you have some ability to store user data on,
like a university or library machine with a login, then use:

    $ python3 -m embroidermodder

when your shell isn't in the `src/` directory (which will boot the local
version).

This may help if you are running tests that require the `embroidermodder`
command to be in your system `PATH` or you just want to use the
latest version before it comes out for some particular feature.

## Documentation

The documentation is in the form of the website (included in the `docs/`
directory) and the printed docs in the three files:

  * [docs/libembroidery_0.1_manual.pdf](docs/libembroidery_0.1_manual.pdf)
  * [docs/embroidermodder_1.90.0_user_manual.pdf](docs/embroidermodder_1.90.0_user_manual.pdf)
  * [docs/embroidermodder_1.90.0_developer_notes.pdf](docs/embroidermodder_1.90.0_developer_notes.pdf).

## Development

If you wish to develop with us you can chat via the contact email
on the [website](embroidermodder.org) or in the issues tab on the
[github page](https://github.com/Embroidermodder/Embroidermodder/issues).
People have been polite and friendly in these conversations and I (Robin)
have really enjoyed them.
If we do have any arguments please note we have a
[Code of Conduct](CODE_OF_CONDUCT.md) so there is a consistent policy to
enforce when dealing with these arguments.

The first thing you should try is building from source using the [build advice](#build)
above. Then read some of the [development notes](dev_notes.md) to get the general
layout of the source code and what we are currently planning.

### Testing

To find unfixed errors run the `qa_tests()` script in `scripts.py`, then
dig through the output. It's currently not worth reporting the errors, since
there are so many but if you can fix anything reported here you can submit a PR.

### Tools

We assume you have access to a Bash scripting environment and standard shell tools,
if you struggle to use any of the scripts intended for developers after reading this
document then please contact us on the issues tab of our GitHub page.

### Build and Run

To test the changes you've just made, use

    $ ./EM2 dev-run

which will install the local version of the software over your current version
(which may damage it, so use a docker or similar if you need to) then run it.

## Overall Structure

## Code Optimisations and Simplifications

### Current

What Robin is currently doing.

Getting the code to pass PyLint, that involves getting all source files
under 1000 lines, renaming all variables to be in `snake_case`.

Changing the seperation of code between EM and libembroidery.

Translating the Qt widget framework to Tk.

### Geometry

The geometry is stored, processed and altered via libembroidery. See the Python specific part of the documentation for libembroidery for this. What the code in Embroidermodder does is make the GUI widgets to change and view this information graphically.

For example if we create a circle with radius 10mm and center at (20mm, 30mm) then fill it with stitches the commands would be

    from libembroidery import Pattern, Circle, Vector, satin
    circle = Circle(Vector(20, 30), 10)
    pattern = Pattern()
    pattern.add_circle(circle, fill=satin)
    pattern.to_stitches()

but the user would do this through a series of GUI actions:

1. Create new file
2. Click add circle
3. Use the Settings dialog to alter the radius and center
4. Use the fill tool on circle
5. Select satin from the drop down menu

So EM2 does the job of bridging that gap.

### Settings Dialog

There are many codeblocks for changing out the colors in one go, for example:
    
    self.mw.update_all_view_select_box_colors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

This could be replaced with a simpler call

    self.mw.update_all_view_select_box_colors(
                    self.accept["display_selectbox_colors"],
                    self.preview["display_selectbox_alpha"])

where we require that

    self.accept["display_selectbox_colors"] == {
        "left_color": "#color",
        "left_fill": "#color",
        "right_color": "#color",
        "right_fill": "#color"
    }

with #color being some valid hex code.

### Kivy

Once the tkinter interface is up and running we can experiment
with different frontends to improve the look of the application.
For example, the MIT licensed KIVY would allow us to replace the 
mobile development in Swift and Java with all Python development:

https://kivy.org/#home

### Data/Code Seperation

All the "data" is in code files that are within the `config/`
submodule. So this way we don't have to deal with awkward data
packaging, it's just available as a single JSON style object
called `settings` available with this import line:

    from embroidermodder.config import settings

In order to pass PyLint style guides this will be split up and
formatted into Python code but no processing beyond inlining
the data into a single dict should be carried out here.

#### The Settings Dictionary

No more than 4 levels of indentation

Only strings, arrays, dicts and integers so matching the JSON standard. Ideally you should be able to copy/paste the data in and out and it would parse as JSON. Currently this fails because we have multi-line strings in Python syntax and inlining.

We may be able to extend the lisp support, which would deal with this. Or we can change multiline strings out for arrays of strings.

#### Lisp Expression Support

In order to safely support user contributed/shared data that can
define, for example, double to double functions we need a consistent
processor for these descriptions.

Embroidermodder uses a list processor (a subset of the language
Lisp which is short for LISt Processor) to accomplish this.

For example the string:

    (+ (* t 2) 5)

is equivalent to the expression:

    2*t + 5

The benefit of not allowing this to simply be a Python expression
is that it is safe against malicious use, or accidental misuse.
The program can identify whether the output is of the appropriate
form and give finitely many calculations before declaring the
function to have run too long (stopping equations that hang).

To see examples of this see `parser.py` and
`config/design_primatives.py`.

It's also worth noting that we don't use the simpler reverse Polish
notation (RPN) approach because:

    1. It's more compact to use Lisp because `a b c + +` for example needs a new `+` sign for each new term as opposed to `(+ a b c)`.
    2. It's easier to support expressions that are themselves function calls defined by the user (by adding support for `defun` or `lambda`.

#### SVG Icons

To make the images easier to alter and restyle we could
switch to svg icons. There's some code in the git history
to help with this.

### The Actions System

In order to simplify the development of a GUI that is flexible and
easy to understand to new developers we have a custom action system that all
user actions will go via an `actuator` that takes a string argument. By using a
string argument the undo history is just an array of strings.

The C `action_hash_data` struct will contain: the icon used, the labels for the
menus and tooltips and the function pointer for that action.
There will be an accompanying argument for this function call, currently being
drafted as `action_call`. So when the user makes a function call it should
contain information like the mouse position, whether special key is pressed
etc.

### Accessibility

Software can be more or less friendly to people with dylexia, partial sightedness,
reduced mobility and those who don't speak English.
Embroidermodder 2 has, in its design, the following features to help:

* icons for everything to reduce the amount of reading required
* the system font is configurable: if you have a dyslexia-friendly font you can load it
* the interface rescales to help with partial-sightedness
* the system language is configurable, unfortunately the docs will only be in English but we can try to supply lots of images of the interface to make it easier to understand as a second language
* buttons are remappable: XBox controllers are known for being good for people with reduced mobility so remapping the buttons to whatever setup you have should help

Note that most of these features will be released with version 2.1, which is planned for around early 2023.

### Current work

1. Converting C++ to Python throughout.
2. OpenGL Rendering
    1. "Real" rendering to see what the embroidery looks like.
    2. Icons and toolbars.
    3. Menu bar
3. Libembroidery interfacing:
    1. Get all classes to use the proper libembroidery types within them.
       So `Ellipse` has `EmbEllipse` as public data within it.
    2. Move calculations of rotation and scaling into `EmbVector` calls.
1. Get undo history widget back (BUG).
2. Switch website to a CMake build.
3. GUI frontend for embroider features that aren't supported by embroidermodder: flag selector from a table
4. Update all formats without color to check for edr or rgb files.
5. EmbroideryFLOSS - Color picker that displays catalog numbers and names
6. Setting for reverse scrolling direction (for zoom, vertical pan)
7. Stitching simulation
8. User designed custom fill
9. Keyboard zooming, panning
10. Advanced printing
11. Libembroidery 1.0
12. Better integrated help: I don't think the help should backend to a html file somewhere on the user's system. A better system would be a custom widget within the program that's searchable.
13. New embroidermodder2.ico 16x16 logo that looks good at that scale.
14. saving dst, pes, jef
15. Settings dialog: notify when the user is switching tabs that the setting has been changed, adding apply button is what would make sense for this to happen.
16. Update language translations
17. Replace KDE4 thumbnailer.
18. Import raster image
19. Statistics from 1.0, needs histogram.
20. SNAP/ORTHO/POLAR
21. Cut/copy allow post-selection
22. Layout into config
23. Notify user of data loss if not saving to an object format.
24. Add which formats to work with to preferences.
25. Cannot open file with # in the name when opening multiple files but works with opening a single file.
26. Closing settings dialog with the X in the window saves settings rather than discarding them.
27. Otto theme icons: units, render, selectors, what's this icon doesn't scale
28. Layer manager and Layer switcher dock widget
29. test that all formats read data in correct scale (format details should match other programs).
30. Custom filter bug -- doesn't save changes in some cases.
31. Get flake8, pylint and tests to pass.
32. Sphinx documentation from docstrings or similar.

For more details read on into the Design section.

### Sample Files

Various sample embroidery design files can be found in
the embroidermodder2/samples folder.

## Design

These are key bits of reasoning behind why the software is built the way it is.

### CAD command review

| ID | Name | Arguments | Description |
|---|---|---|---|
| 0 | newfile | none | Create a new EmbPattern with a new tab in the GUI. |
| 1 | openfile | `char *fname;` | Open an EmbPattern with the supplied filename `fname`. |
| 2 | savefile | `char *fname;` | Save the current loaded EmbPattern to the supplied filname `fname`. |
| 1 | scale | selected objects, 1 float | Scale all selected objects by the number supplied, without selection scales the entire design |
| 2 | circle | mouse co-ords | Adds a circle to the design based on the supplied numbers, converts to stitches on save for stitch only formats. |
| 3 | offset | mouse co-ords | Shifts the selected objects by the amount given by the mouse co-ordinates. |
| 4 | extend | | |
| 5 | trim | | |
| 6 | BreakAtPoint | | |
| 7 | Break2Points | | |
| 8 | Fillet | | |
| 9 | star | | |
| 10 | singlelinetext | | |
| 11 | Chamfer | | |
| 12 | split | | |
| 13 | area | | |
| 14 | time | | |
| 15 | pickadd | | |
| 16 | zoomfactor | | |
| 17 | product | | |
| 18 | program | | |
| 19 | zoomwindow | | |
| 20 | divide | | |
| 21 | find | | |
| 22 | record | | |
| 23 | playback | | |
| 24 | rotate | | |
| 25 | rgb | | |
| 26 | move | | |
| 27 | grid | | |
| 28 | griphot | | |
| 29 | gripcolor | | |
| 30 | gripcool | | |
| 31 | gripsize | | |
| 32 | highlight | | |
| 33 | units | | |
| 34 | locatepoint | | |
| 35 | distance | | |
| 36 | arc | | |
| 37 | ellipse | | |
| 38 | array | | |
| 39 | point | | |
| 40 | polyline | | |
| 41 | polygon | | |
| 42 | rectangle | | |
| 43 | line | | |
| 44 | arc (rt) | | |
| 45 | dolphin | | |
| 46 | heart | | |


### Removed Elements

So I've had a few pieces of web infrastructure fail me recently and
I think it's worth noting. An issue that affects us is an issue that
can effect people who use our software.

#### Qt and dependencies

Downloading and installing Qt has been a pain for some users
(46Gb on possibly slow connections).

I'm switching to FreeGLUT 3 (which is a whole other conversation) which means we
can ship it with the source code package meaning only a basic build
environment is necessary to build it.

#### Social Platform

Github is giving me a server offline (500) error and is still giving a bad ping.

So... all the issues and project boards etc. being on Github is all well and
good assuming that we have our own copies. But we don't if Github goes down
or some other major player takes over the space and we have to move (again,
since this started on SourceForge).

This file is a backup for that which is why I'm repeating myself between them.

#### Pandoc Documentation

The documentation is, well better in that it's housed in the main repository,
but I'm not a fan of the "write once build many" approach as it means
trying to weigh up how 3 versions are going to render.

Can we treat the website being a duplicate of the docs a non-starter?
I'd be happier with tex/pdf only and (I know this is counter-intuitive) one
per project.

### OpenGL

OpenGL rendering within the application. This will allow for
Realistic Visualization - Bump Mapping/OpenGL/Gradients?

This should backend to a C renderer or something.

### Configuration Data Ideas

embroidermodder should boot from the command line
regardless of whether it is or is not installed (this helps with testing and
running on machines without root). Therefore, it can create an initiation file
but it won't rely on its existence to boot: `~/.embroidermodder/config.json`.

1. Switch colors to be stored as 6 digit hexcodes with a #.
2. We've got close to a hand implemented ini read/write setup in `settings.py`.

### Distribution

When we release the new pip wheel we should also package:

* `.tar.gz` and `.zip` source archive.
* Debian package
* RPM package

Only do this once per minor version number.

### Scripting Overhaul

Originally Embroidermodder had a terminal widget, this is why we removed it.

> ROBIN: I think supporting scripting within Embroidermodder doesn't make sense.
>
>  All features that use scripting can be part of libembroidery instead.
> Users who are capable of using scripting won't need it, they can alter their embroidery files in CSV format, or import pyembroidery to get access.
> It makes maintaining the code a lot more complicated, especially if we move away from Qt.
>     Users who don't want the scripting feature will likely be confused by it, since we say that's what libembroidery, embroider and pyembroidery are for.
> 
> How about a simpler "call user shell" feature? Similar to texmaker we just call system on a batch or shell script supplied by the user and it processes the file directly then the software reloads the file. Then we aren't parsing it directly.
> 
>I don't want to change this without Josh's support because it's a fairly major change.
> 
>> JOSH: I totally agree.
>>
>> I like the idea of scripting just so people that know how to code could write their own designs without needing to fully build the app. Scripting would be a very advanced feature that most users would be confused by. Libembroidery would be a good fit for advanced features.

Now we are using Python (again, sort of) this would be a lot more natural,
perhaps we could boot the software without blocking the shell so they can
interact? TODO: Screenshot a working draft to demonstrate.

## Perennial Jobs

1. Check for memory leaks
2. Write new tests for new code.
3. Get Embroidermodder onto the current version of libembroidery-python.
4. PEP8 compliance.
5. Better documentation with more photos/screencaps.

### Developing for Android

https://developer.android.com/studio/projects/add-native-code

    apt install google-android-ndk-installer cmake lldb gradle

