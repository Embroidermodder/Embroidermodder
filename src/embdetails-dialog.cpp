/**
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
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

/* . */
QLabel *
make_label(char label_str[100], QWidget *parent)
{
    return new QLabel(label_str, parent);
}

/* . */
QLabel *
make_label(double x, QWidget *parent)
{
    return new QLabel(QString::number(x), parent);
}

/* . */
QLabel *
make_label(uint32_t x, QWidget *parent)
{
    return new QLabel(QString::number(x), parent);
}

/* . */
QLabel *
make_translated_label(char *label_str, QWidget *parent)
{
    return new QLabel(translate_str(label_str), parent);
}

/* Create a details dialog object. */
EmbDetailsDialog::EmbDetailsDialog(QGraphicsScene* theScene, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    getInfo();
    mainWidget = createMainWidget();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(mainWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Embroidery Design Details"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* Destroy the details dialog. */
EmbDetailsDialog::~EmbDetailsDialog()
{
    QApplication::restoreOverrideCursor();
}

/* Create main widget. */
QWidget*
EmbDetailsDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);
    QGroupBox* groupBoxMisc = new QGroupBox(tr("General Information"), widget);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    for (int i=0; i<12; i++) {
        gridLayoutMisc->addWidget(make_translated_label((char*)details_labels[i], widget), i, 0, Qt::AlignLeft);
    }
    gridLayoutMisc->addWidget(make_label(stitchesTotal, widget), 0, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesReal, widget), 1, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesJump, widget), 2, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(stitchesTrim, widget), 3, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(colorTotal, widget), 4, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(colorChanges, widget), 5, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.left(), widget), 6, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.top(), widget), 7, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.right(), widget), 8, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.bottom(), widget), 9, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.width(), widget), 10, 1, Qt::AlignLeft);
    gridLayoutMisc->addWidget(make_label(boundingRect.height(), widget), 11, 1, Qt::AlignLeft);
    gridLayoutMisc->setColumnStretch(1,1);
    groupBoxMisc->setLayout(gridLayoutMisc);

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram

    /*
    QString str = "";
    for (int i = 0; i < num_bins; i++) {
        str += QString::number(binSize * (i), 'f', 1);
        str += " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: ";
        str += QString::number(bin[i]) + "\n\n";
    }

    QGridLayout* grid = new QGridLayout(_mainWin);
    grid->setSpacing(2);
    grid->addWidget(new QLabel(translate_str("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(translate_str("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;

    for (int i = 0; i < num_colors; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = pattern->thread_list->thread[i].color;
        //palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        qDebug("size: %d i: %d", stitchLengths.size(), i);
        //grid->addWidget(new QLabel(QString::number(stitch_lengths[i]) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

     */


    QDialogButtonBox buttonbox(Qt::Horizontal, this);
    QPushButton button(this);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), this, SLOT(accept()));

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    //vboxLayoutMain->addWidget(groupBoxDist);
    //vboxLayoutMain->addWidget(buttonbox);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    return scrollArea;
}

/* Get information from the embroidery.
 *
 * TODO: Move majority of this code into libembroidery.
 * TODO: generate a temporary pattern from the scene data.
 * TODO: grab this information from the pattern
 *
 * TODO: embStitchList_count(pattern->stitchList, TOTAL);
 */
void
EmbDetailsDialog::getInfo(void)
{
    debug_message("designDetails()");

    boundingRect.setRect(0, 0, 50, 100); //TODO: embPattern_calcBoundingBox(pattern);

    int bin[MAX_HISTOGRAM_BINS+1];

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern\n");
        return;
    }

    /* TODO: This is temporary. Generate actual pattern data from the scene. */
    /* TODO: This convenience function is messed up. */

    //boundingRect = embPattern_calcBoundingBox(pattern);

    EmbVector minimum, maximum;
    EmbStitch last_pos;
    int num_bins = 10;
    stitchesTotal = pattern->stitch_list->count;
    stitchesReal = 0;
    stitchesJump = 0;
    stitchesTrim = 0;
    colorTotal = pattern->thread_list->count;
    colorChanges  = 0; //TODO: embThreadList_count(pattern->threadList, CHANGES);

    int unknown_stitches = 0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;
    double length = 0.0;

    minimum.x = 1000.0f;
    minimum.y = 1000.0f;
    maximum.x = 0.0f;
    maximum.y = 0.0f;

    if (pattern->stitch_list->count == 0) {
        QMessageBox::warning(_mainWin,
            translate_str("No Design Loaded"),
            translate_str("<b>A design needs to be loaded or created before details can be determined.</b>"));
        embPattern_free(pattern);
        return;
    }

    QVector<double> stitchLengths;
    double totalColorLength = 0.0;
    last_pos = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbVector delta;
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - last_pos.x;
        delta.y = st.y - last_pos.y;
        length = embVector_length(delta);
        totalColorLength += length;
        // can't count first normal stitch
        if (last_pos.flags != NORMAL) {
            length = 0.0;
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
            if (last_pos.x < minimum.x) {
                minimum.x = last_pos.x;
            }
            if (last_pos.y < minimum.y) {
                minimum.y = last_pos.y;
            }
            if (last_pos.x > maximum.x) {
                maximum.x = last_pos.x;
            }
            if (last_pos.y > maximum.y) {
                maximum.y = last_pos.y;
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
        if (st.flags | (JUMP & TRIM & STOP & END)) {
            unknown_stitches++;
        }
        last_pos = st;
    }

    /* Second pass to fill bins now that we know max stitch length. */
    double binSize = max_stitchlength / num_bins;

    for (int i = 0; i <= num_bins; i++) {
        bin[i]=0;
    }

    last_pos = pattern->stitch_list->stitch[0];
    for (int i = 1; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        if (last_pos.flags == NORMAL && st.flags == NORMAL) {
            EmbVector delta;
            delta.x = st.x - last_pos.x;
            delta.y = st.y - last_pos.y;
            length = embVector_length(delta);
            bin[int(floor(num_bins*length/max_stitchlength))]++;
        }
        last_pos = st;
    }
}
