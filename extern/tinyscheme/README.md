# tinyscheme.c

 TinyScheme is a lightweight Scheme interpreter that implements as large
 a subset of R5RS as was possible without getting very large and
 complicated. It is meant to be used as an embedded scripting interpreter
 for other programs. As such, it does not offer IDEs or extensive toolkits
 although it does sport a small top-level loop, included conditionally.
 A lot of functionality in TinyScheme is included conditionally, to allow
 developers freedom in balancing features and footprint.

 As an embedded interpreter, it allows multiple interpreter states to
 coexist in the same program, without any interference between them.
 Programmatically, foreign functions in C can be added and values
 can be defined in the Scheme environment. Being a quite small program,
 it is easy to comprehend, get to grips with, and use.

 This version was modified to support CMake projects for easier embedding.
 Few bugs were fixed and improved.

 Read Manual.txt for more information.