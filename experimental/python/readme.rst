Embroidermodder v2.0 Python GUI's
=================================
Aims to implement the Embroidermodder v2 GUI in wxPython/PySide/PyQt4/etc...
...with PySide being the main focus GUI as it is closest to C++ Qt.
So that the workload and new or extra features are quick to realize.
Also serves as a base if/when if ever the embroidery formats get ported to python.

At this point in time the CAD Interface is unimplemented in the MDI.
A TextEditor resides in it's place at the moment.

Mockup GUI
----------
Show where GUI improvements can be made and developed quickly
and efficiently before being ported into C++.
Code should be written so it is easiest
to port for the Embroidermodder coding guidelines.

Remember Readability Counts with python.

When in doubt...

``import this``


C++ vs. Python
--------------
While C++ is very fast at execution speeds,
it is slow when it comes to development time overall.

While Python is very fast with development times,
and is a better beginning learning language,
certain things execute slower(Number crunching for one),
and are better written in a faster executing language such as C++,
for a project of this scope.(certainly the cad view and rendering stuff)
Python is elegant and reads like a book, so code is very easy
to decipher.

Combining the best aspects of both these programming languages,
makes for a better development environment for all people to contribute.
Plus not everyone knows the C family of languages.


Requirements/Dependencies
-------------------------

1. The Embroidermodder source for the images/icons.
2. Python 2.7 http://www.python.org or maybe Python 3.3 if using wxPython Project Phoenix/PySide/PyQt



wxPython Classic
----------------
With Microsoft Windows, grab the build or source from here.
http://wxPython.kosoftworks.com/preview/20140104
It fixes compiled issues.

If you are using linux or mac and run into any problems
with the downloads on the regular download page,
Build wxPython from the source package on the preview builds page
linked above for windows.

wxPython Phoenix
----------------
Phoenix is still in development as of this writing,
but most stuff works with python 3.3 now.

http://wxpython.org/Phoenix/snapshot-builds/

PySide
------
http://qt-project.org/wiki/Category:LanguageBindings::PySide::Downloads

PyQt
----
http://www.riverbankcomputing.co.uk/software/pyqt/download

Tkinter
-------
Sorry no Tkinter GUI...
Feel free to write it and send a pull request if you want to.
