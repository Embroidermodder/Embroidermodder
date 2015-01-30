libembroidery csharp converter
------------------------------

experimental

Dependencies
------------
```
sudo apt-get install mono-mcs
```
There may be other packages needed that I forgot to list here.


Building
--------
```
mcs -out:libembroidery-convert ../binding/*.cs libembroidery-convert.cs
```

Running
-------
```
export LD_LIBRARY_PATH=../binding
./libembroidery-convert input.exp output.dst
```
