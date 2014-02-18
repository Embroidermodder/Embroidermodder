Pascal bindings
----------------

The files and examples contained within are intended for use with Delphi(Windows/Mac) or Lazarus/FreePascal (cross platform).

Dependencies
------------
1. libembroidery binary shared library and it's wrapper for pascal:
* libembroidery.dll (or libembroidery.so in linux)
* libembroidery.pas

2. Delphi or Lazarus.

* Delphi ver 5.0, 6.0, 7.0, 2005, 2006, 2010, XE, XE2, XE3, XE4, XE5 (Delphi 8 is not supported)
http://www.embarcadero.com/products/delphi

* Lazarus ver 1.0.10 up
http://www.lazarus.freepascal.org/

Ubuntu repository packages:
The FreePascal and Lazarus build dependencies can be installed easily by opening a terminal and issuing this command:
```
sudo apt-get install fpc-source lazarus
```
Building
--------

* Create a new project with you favourite pascal IDE (Delphi / Lazarus)
* Set your IDE's search path to where you put libembroidery.pas, so this file can be accessed.
  Delphi: menu Project->Options->Directories/Conditionals->Search path.
* Add libembroidery in "uses" section, like this:
  uses libembroidery;
* Make sure your runtime application can access the libembroidery.dll, 
  usually put this in the same directory of your application (.exe)
That's all!

Modifying libembroidery.pas
---------------------------

libembroidery.pas is a wrapper for pascal language of libembroidery.dll /.so
Modifying this pascal unit is not required.

IFF (if and only if) you sure you need to regenerate it from C header (*.h), 
or in case there is a need to reflect any changes made of libembroidery's API,
then you can use any tool to generate .PAS from .H such:
* h2pas by FreePascal
http://www.freepascal.org/tools/h2pas.var
http://wiki.freepascal.org/Creating_bindings_for_C_libraries
http://wiki.freepascal.org/H2Pas
* HeadConv (Delphi & Windows only) by DrBob, later by JEDI
http://www.drbob42.com/delphi/headconv.htm
http://sourceforge.net/projects/jdarth/
http://www.codeforge.com/read/156535/Headconv.ini__html

