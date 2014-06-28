#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=========================
|module_summary| hacks.py
=========================

TOWRITE

"""

def ForceRepaint(widget):
    """
    Force an immediate full paint refresh/update.

    :note: This is rather HACK-ish. Qt should have a built-in way to do this...?
    :TODO: Figure out which ForceRefresh method is the least expensive performance-wise.
    """
    # Method1
    widget.hide()
    widget.show()

    # ...or this works also...
    # ... Not so well on wx when the frame is fullscreen/maximized. May need to be (-1, -1)

    # Method2
    # HACK: Take that QMdiArea!
    ## hack = widget.size()
    ## widget.resize(hack + QSize(1, 1))
    ## widget.resize(hack)
