.. currentmodule:: sdl2.ext

`sdl2.ext.resources` - Resource Management
==========================================

Nearly every application or game includes resources, such as image and data
files, configuration files and so on. Accessing those files from an asset folder
hierarchy or a compressed bundle across platforms can become a complex
task. The :class:`Resources` class aims to simplify this by providing 
dictionary-style access for your application's resources.

Let's assume your application has the following installation layout: ::

    Application Directory
        Application.py
        Application.conf
        data/
            background.jpg
            button1.jpg
            button2.jpg
            info.dat

Within the ``Application.py`` code, you can - completely system-agnostic -
define a new resource that keeps track of all ``data`` items. ::

    apppath = os.path.dirname(os.path.abspath(__file__))
    appresources = Resources(os.path.join(apppath, "data"))
    
    # Access some images
    bgimage = appresources.get("background.jpg")
    btn1image = appresources.get("button1.jpg")
    ...

To access individual files, you do not need to concat paths the whole
time and regardless of the current directory, your application operates
on, you can access your resource files at any time through the
:class:`Resources` instance, you created initially.

The :class:`Resources` class is also able to scan an index archived files,
compressed via ZIP or TAR (gzip or bzip2 compression), and subdiectories
automatically. ::

    Application Directory
        Application.py
        Application.conf
        data/
            audio/
                example.wav
            background.jpg
            button1.jpg
            button2.jpg
            graphics.zip
                [tileset1.bmp
                 tileset2.bmp
                 tileset3.bmp
                 ]
            info.dat

    tilesimage = appresources.get("tileset1.bmp")
    audiofile = appresources.get("example.wav")

If you request an indexed file via :meth:`Resources.get`, you will receive
a :class:`io.BytesIO` stream, containing the file data, for further processing.

.. note::

   The scanned files act as keys within the :class:`Resources` class. This
   means that two files, that have the same name, but are located in different
   directories, will not be indexed. Only one of them will be accessible
   through the :class:`Resources` class.


API
---

.. automodule:: sdl2.ext.resources
   :exclude-members: open_url
   :members:
