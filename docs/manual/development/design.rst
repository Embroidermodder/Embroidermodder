Design
======


Ideas
=====
 
Originally the thumbnailer was part of this project, however since the
binary is fairly simple and low level it's now part of libembroidery.
The same is true of the bindings to different languages.

Scheme
~~~~~~

Registerables
+++++++++++++

A scheme registerable should have the form

```cpp
pointer name_f(scheme *sc, pointer args);
```

where the name of the function is is all lowercase, seperated with underscores,
and ending with `_f`: `like_this_f`. Note that the `_f` is present on all scheme
registerable function pointers to differentiate them from C functions that
aren't going to be scheme callbacks.

The name of the function in scheme is the same, but with hyphens joining the
words `like-this-f`. Also the `-f` suffix is present to show that the scheme
function has been described in Embroidermodder and is not a TinyScheme
built-in or scheme-only function.

After creating the function, add a pointer to the list at the top of the file
and add a row to the table `ff_list` following these rules.

Scheme State
++++++++++++

We don't require that scheme parts of the source don't create new state
entries, however, the state is managed by requiring all state labels are
declared in "boot.scm". For example, we use the `context` state variable to
store what context the current command has been called in. The line::

    (define context context-main)

is present in this file to alert programmers that this state variable is
present.

Commands
~~~~~~~~

A command is a scheme function designed to be called by a QAction. It may
check the context state variable in order to act differently

