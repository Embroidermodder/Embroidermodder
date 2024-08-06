/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * EmbDetailsDialog
 */

#include "embroidermodder.h"

#define NUMBINS 10

/* TODO: Move majority of this code into libembroidery. */
void
create_details_dialog(void)
{
    QDialog *dialog = new QDialog();
    QRectF boundingRect;

    dialog->setMinimumSize(750, 550);

    qDebug("EmbDetailsDialog()");
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    EmbPattern* pattern = activeMdiWindow()->pattern;
    if (!pattern) {
        printf("Pattern data is missing or failed to initialize.");
        return;
    }

    uint32_t stitchesTotal = pattern->stitch_list->count;
    uint32_t stitchesReal = 0;
    uint32_t stitchesJump = 0;
    uint32_t stitchesTrim = 0;
    uint32_t colorTotal = pattern->thread_list->count;
    uint32_t colorChanges = 0;
    double minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    EmbRect bounds = emb_pattern_calcBoundingBox(pattern);

    if (pattern->stitch_list->count == 0) {
        QMessageBox::warning(dialog,
            translate("No Design Loaded"),
            translate("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<double> stitchLengths;

    EmbVector position;
    position.x = 0.0f;
    position.y = 0.0f;
    double totalColorLength = 0.0;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - position.x;
        delta.y = st.y - position.y;
        position.x = st.x;
        position.y = st.y;
        double length = emb_vector_length(delta);
        totalColorLength += length;
        if (i > 0) {
            if (pattern->stitch_list->stitch[i-1].flags != NORMAL) {
                /* Can't count first normal stitch. */
                length = 0.0;
            }
        }
        if (!(st.flags & (JUMP | TRIM))) {
            stitchesReal++;
            if (length > max_stitchlength) {
                max_stitchlength = length;
                number_of_maxlength_stitches = 0;
            }
            if (length == max_stitchlength) {
                number_of_maxlength_stitches++;
            }
            if (length > 0 && length < min_stitchlength) {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if (length == min_stitchlength) {
                number_of_minlength_stitches++;
            }
            total_stitchlength += length;
            if (position.x < minx) {
                minx = position.x;
            }
            if (position.x > maxx) {
                maxx = position.x;
            }
            if (position.y < miny) {
                miny = position.y;
            }
            if (position.y > maxy) {
                maxy = position.y;
            }
        }
        if (st.flags & JUMP) {
            stitchesJump++;
        }
        if (st.flags & TRIM) {
            stitchesTrim++;
        }
        if (st.flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colorChanges++;
        }
        if (st.flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
    }

    /* Second pass to fill bins now that we know max stitch length. */
    int bin[NUMBINS+1];
    for (int i = 0; i <= NUMBINS; i++) {
        bin[i] = 0;
    }

    position.x = 0.0f;
    position.y = 0.0f;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - position.x;
        delta.y = st.y - position.y;
        position.x = st.x;
        position.y = st.y;
        if (i > 0) {
            if (pattern->stitch_list->stitch[i-1].flags == NORMAL && st.flags == NORMAL) {
                double length = emb_vector_length(delta);
                int bin_number = int(floor(NUMBINS*length/max_stitchlength));
                bin[bin_number]++;
            }
        }
    }

    double binSize = max_stitchlength / NUMBINS;

    QString str;
    for (int i = 0; i < NUMBINS; i++) {
        str += QString::number(binSize * (i), 'f', 1);
        str += " - " + QString::number(binSize * (i+1), 'f', 1);
        str += " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    /*
    grid->addWidget(new QLabel(translate("\nStitch Distribution: \n")), 9, 0, 1, 2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(translate("\nThread Length By Color: \n")), 11, 0, 1, 2);
    */
    int currentRow = 12;

    for (int i = 0; i < pattern->thread_list->count; i++) {
        EmbColor t = pattern->thread_list->thread[i].color;
        /*
        QFrame *frame = new QFrame();
        frame->setGeometry(0, 0, 30, 30);
        QPalette palette = frame->palette();
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        qDebug("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        */
        currentRow++;
    }

    boundingRect.setRect(bounds.left, bounds.top, bounds.right, bounds.bottom);

    QWidget* widget = new QWidget(dialog);

    //Misc
    QGroupBox* groupBoxMisc = new QGroupBox(translate("General Information"), widget);

    QLabel* labelStitchesTotal = new QLabel(translate("Total Stitches:"), dialog);
    QLabel* labelStitchesReal  = new QLabel(translate("Real Stitches:"),  dialog);
    QLabel* labelStitchesJump  = new QLabel(translate("Jump Stitches:"),  dialog);
    QLabel* labelStitchesTrim  = new QLabel(translate("Trim Stitches:"),  dialog);
    QLabel* labelColorTotal    = new QLabel(translate("Total Colors:"),   dialog);
    QLabel* labelColorChanges  = new QLabel(translate("Color Changes:"),  dialog);
    QLabel* labelRectLeft      = new QLabel(translate("Left:"),           dialog);
    QLabel* labelRectTop       = new QLabel(translate("Top:"),            dialog);
    QLabel* labelRectRight     = new QLabel(translate("Right:"),          dialog);
    QLabel* labelRectBottom    = new QLabel(translate("Bottom:"),         dialog);
    QLabel* labelRectWidth     = new QLabel(translate("Width:"),          dialog);
    QLabel* labelRectHeight    = new QLabel(translate("Height:"),         dialog);

    QLabel* fieldStitchesTotal = new QLabel(QString::number(stitchesTotal), dialog);
    QLabel* fieldStitchesReal  = new QLabel(QString::number(stitchesReal),  dialog);
    QLabel* fieldStitchesJump  = new QLabel(QString::number(stitchesJump),  dialog);
    QLabel* fieldStitchesTrim  = new QLabel(QString::number(stitchesTrim),  dialog);
    QLabel* fieldColorTotal    = new QLabel(QString::number(colorTotal),    dialog);
    QLabel* fieldColorChanges  = new QLabel(QString::number(colorChanges),  dialog);
    QLabel* fieldRectLeft      = new QLabel(QString::number(boundingRect.left())   + " mm", dialog);
    QLabel* fieldRectTop       = new QLabel(QString::number(boundingRect.top())    + " mm", dialog);
    QLabel* fieldRectRight     = new QLabel(QString::number(boundingRect.right())  + " mm", dialog);
    QLabel* fieldRectBottom    = new QLabel(QString::number(boundingRect.bottom()) + " mm", dialog);
    QLabel* fieldRectWidth     = new QLabel(QString::number(boundingRect.width())  + " mm", dialog);
    QLabel* fieldRectHeight    = new QLabel(QString::number(boundingRect.height()) + " mm", dialog);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(labelStitchesTotal,  0, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesReal,   1, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesJump,   2, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelStitchesTrim,   3, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorTotal,     4, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelColorChanges,   5, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectLeft,       6, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectTop,        7, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectRight,      8, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectBottom,     9, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectWidth,     10, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(labelRectHeight,    11, 0, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTotal,  0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesReal,   1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesJump,   2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldStitchesTrim,   3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorTotal,     4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldColorChanges,   5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectLeft,       6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectTop,        7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectRight,      8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectBottom,     9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectWidth,     10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(fieldRectHeight,    11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    /* TODO: Color Histogram. */

    /* Stitch Distribution */
    QGroupBox* groupBoxDist = new QGroupBox(translate("Stitch Distribution"), widget);

    /* TODO: Stitch Distribution Histogram */

    /* Widget Layout
    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget); */

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    vboxLayoutMain->addWidget(groupBoxDist);
    vboxLayoutMain->addWidget(buttonBox);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    dialog->setWindowTitle(translate("Embroidery Design Details"));

    dialog->exec();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

