Visual Studio project files
---------------------------

We recommend using Visual Studio 2010 or newer.

Troubleshooting
---------------

If for some reason you cannot build with Visual Studio, try this:

1) There may be an error in our code. If this is the case and you can
   easily spot and fix the error, please send us a bug report, pull request
   or patch and as we are likely not aware of the problem.

2) Maintaining multiple build configurations for different operating systems
   and compilers is quite tedious. We use Qt's qmake as our primary build
   system because it is cross-platform. Try building with those files instead
   and if they work, the file you are using here may be out of sync with the
   latest .pro file needed for a particular component.
