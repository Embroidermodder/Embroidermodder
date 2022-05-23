# TinyScheme

## Notes by armornick

This is a sort-of continuation of the TinyScheme project, which seems to have been abandoned.

It started with just trying to get it to compile via CMake before deciding to separate the shell into its own file, and things escalated from there.

**About support**: while everyone is free to report any issues, this is a only a minor hobby project for me. That means there's about a 50/50 chance that I'll do something about it. The TinyScheme source code seems to be fairly understandable, however, so an intermediate programmer should be able to make changes without too much difficulty.

As a final note, I don't need any personal credits since I haven't actually written the original project source. Be sure to properly credit the original author, though.

## Description by original author

TinyScheme is a lightweight Scheme interpreter that implements as large a subset of R5RS as was possible without getting very large and complicated. It is meant to be used as an embedded scripting interpreter for other programs. As such, it does not offer IDEs or extensive toolkits although it does sport a small top-level loop, included conditionally. A lot of functionality in TinyScheme is included conditionally, to allow developers freedom in balancing features and footprint.

As an embedded interpreter, it allows multiple interpreter states to coexist in the same program, without any interference between them. Programmatically, foreign functions in C can be added and values can be defined in the Scheme environment. Being a quite small program, it is easy to comprehend, get to grips with, and use. TinyScheme was grown out of the MiniScheme distribution during the development of Ovrimos.

The TinyScheme project has slowly acquired foothold in many open-source projects over the years, notably GIMP. It is evident that it is in need of constant attention, since any bug potentially affects a lot of installations. I am very grateful to the people that have helped with TinyScheme all this time, and I hope they will continue to do so in the future.

There is an issue that is better put forth, rather than stashed under the carpet. The same qualities that have made TinyScheme suitable for many embedded or small-footprint jobs, have earned it an unwanted place in less desirable software. I hope it is clear that TinyScheme itself is domain-agnostic and I, in particular, am not a supporter of malware or adware.

## Licensing

This software is open source, covered by a BSD-style license. See the LICENSE file in this directory for the full license terms.