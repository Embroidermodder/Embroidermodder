/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *
 *  The dialog showing details of the pattern including histograms.
 */

#include "em2.h"

/* Creates a dialog showing key information about the pattern,
 * ideally this will update as the pattern changes without
 * any key presses or clicks.
 */
void
details_dialog_init(void)
{
    /* kiss-window dialog = kiss-window()
    dialog-setMinimumSize(750, 550)
    dialog-title(translate("Embroidery Design Details"))
    */ /*

    stitches-total = 0;
    stitches-real = 0;
    stitches-jump = 0;
    stitches-trim = 0;
    color-total = 0;
    color-changes = 0; */

    /*
    bounding-rect = make_rectangle(0.0, 0.0, 0.1, 0.1)

    get-info()
    main-widget = create-main-widget()

    button-box = tk.ButtonBox(dialog, text="QDialogButtonBox-Ok") */
    /* # connect(buttonBox, SIGNAL(accepted()), SLOT(accept())) */
    /*
    vbox_layout_main = tk.VBoxLayout()
    vbox_layout_main, add_widget(main-widget)
    vbox_layout_main, add_widget(button-box)
    dialog-set_layout(vbox_layout_main)

    mw.setOverrideCursor("ArrowCursor") */
}

void
details_dialog_free(void)
{
    /* mw.restore_override_cursor() */
}


void
create_histogram(void)
{
    /*
    debug_message("TODO: createHistogram");
    */
}

/*
 * Uses the pattern index.
 */
void
get_info(int pattern)
{
    /* TODO: grab this information from the pattern */
    /* stitches-total = 5 */
    /* TODO: embStitchList-count(pattern->stitchList, TOTAL) */
    /* stitches-real = 4 */
    /* TODO: embStitchList-count(pattern.stitchList, NORMAL) */
    /* stitches-jump = 3 */
    /* TODO: embStitchList-count(pattern.stitchList, JUMP) */
    /* stitches-trim = 2 */
    /* TODO: embStitchList-count(pattern.stitchList, TRIM) */
    /* color-total = 1 */
    /* TODO: embThreadList-count(pattern.threadList, TOTAL) */
    /* color-changes = 0 */
    /* TODO: embThreadList-count(pattern.threadList, CHANGES) */
    /* bounding-rect.set_rect(0, 0, 50, 100) */
    /* TODO: embPattern-calcBoundingBox(pattern) */
}


void
create_main_widget(void)
{
    /*
    widget = tk.Widget(dialog)

 Misc
    groupbox-misc = tk.groupbox(translate("General Information"), widget)

    Labels = []
    fields = []

    for i in range(12)
        details-labels = mw.setting-details-label-text;
        Labels[i] = tk.Label(translate(details-labels[i]));

    fields[0] = tk.Label(str(stitches-total));
    fields[1] = tk.Label(str(stitches-real));
    fields[2] = tk.Label(str(stitches-jump));
    fields[3] = tk.Label(str(stitches-trim));
    fields[4] = tk.Label(str(color-total));
    fields[5] = tk.Label(str(colorChanges));
    fields[6] = tk.Label(str(bounding-rect.left()) + " mm");
    fields[7] = tk.Label(str(bounding-rect.top()) + " mm");
    fields[8] = tk.Label(str(bounding-rect.right()) + " mm");
    fields[9] = tk.Label(str(bounding-rect.bottom()) + " mm");
    fields[10] = tk.Label(str(bounding-rect.width()) + " mm");
    fields[11] = tk.Label(str(bounding-rect.height()) + " mm");

    grid-layout-misc = GridLayout(groupbox-misc)
    for i in range(12)
        grid-layout-misc, add_widget(tk.Labels[i], i, 0, "Qt-AlignLeft")
        grid-layout-misc, add_widget(fields[i], i, 1, "Qt-AlignLeft")

    grid-layout-misc.setColumnStretch(1, 1)
    groupbox-misc.set_layout(grid-layout-misc)

    
    # TODO: Color Histogram

    # Stitch Distribution
    # groupboxDist = Qgroupbox(translate("Stitch Distribution"), widget)
    */
    /* TODO: Stitch Distribution Histogram */

    /* Widget Layout */
    /* vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupbox-misc); */
    /* vbox_layout_main, add_widget(groupboxDist); */
    /* vbox_layout_main.addStretch(1);
    widget.set_layout(vbox_layout_main);

    scroll_area = tk.scroll_area();
    scroll_area.set_widget-resizable(1);
    scroll_area.set_widget(widget);
    return scroll_area;
    */
}


