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

#define MAX_HISTOGRAM_BINS                    1000

/* . */
QLabel *
make_label(const char *label_str, QWidget *parent)
{
    return new QLabel(label_str, parent);
}

/* . */
QLabel *
make_translated_label(const char *label_str, QWidget *parent)
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

/* EmbDetailsDialog::createMainWidget */
QWidget*
EmbDetailsDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);

    //Misc
    QGroupBox* groupBoxMisc = new QGroupBox(tr("General Information"), widget);

    QLabel* labelStitchesReal  = new QLabel(tr("Real Stitches:"),  this);
    QLabel* labelStitchesJump  = new QLabel(tr("Jump Stitches:"),  this);
    QLabel* labelStitchesTrim  = new QLabel(tr("Trim Stitches:"),  this);
    QLabel* labelColorTotal    = new QLabel(tr("Total Colors:"),   this);
    QLabel* labelColorChanges  = new QLabel(tr("Color Changes:"),  this);
    QLabel* labelRectLeft      = new QLabel(tr("Left:"),           this);
    QLabel* labelRectTop       = new QLabel(tr("Top:"),            this);
    QLabel* labelRectRight     = new QLabel(tr("Right:"),          this);
    QLabel* labelRectBottom    = new QLabel(tr("Bottom:"),         this);
    QLabel* labelRectWidth     = new QLabel(tr("Width:"),          this);
    QLabel* labelRectHeight    = new QLabel(tr("Height:"),         this);

    QLabel* fieldStitchesTotal = new QLabel(QString::number(stitchesTotal), this);
    QLabel* fieldStitchesReal  = new QLabel(QString::number(stitchesReal),  this);
    QLabel* fieldStitchesJump  = new QLabel(QString::number(stitchesJump),  this);
    QLabel* fieldStitchesTrim  = new QLabel(QString::number(stitchesTrim),  this);
    QLabel* fieldColorTotal    = new QLabel(QString::number(colorTotal),    this);
    QLabel* fieldColorChanges  = new QLabel(QString::number(colorChanges),  this);
    QLabel* fieldRectLeft      = new QLabel(QString::number(boundingRect.left())   + " mm", this);
    QLabel* fieldRectTop       = new QLabel(QString::number(boundingRect.top())    + " mm", this);
    QLabel* fieldRectRight     = new QLabel(QString::number(boundingRect.right())  + " mm", this);
    QLabel* fieldRectBottom    = new QLabel(QString::number(boundingRect.bottom()) + " mm", this);
    QLabel* fieldRectWidth     = new QLabel(QString::number(boundingRect.width())  + " mm", this);
    QLabel* fieldRectHeight    = new QLabel(QString::number(boundingRect.height()) + " mm", this);

    QGridLayout* gridLayoutMisc = new QGridLayout(groupBoxMisc);
    gridLayoutMisc->addWidget(make_translated_label("Total Stitches:", widget),  0, 0, Qt::AlignLeft);
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

    //TODO: Color Histogram

    //Stitch Distribution
    //QGroupBox* groupBoxDist = new QGroupBox(tr("Stitch Distribution"), widget);

    //TODO: Stitch Distribution Histogram

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxMisc);
    //vboxLayoutMain->addWidget(groupBoxDist);
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
 */
void
EmbDetailsDialog::getInfo(void)
{
    debug_message("designDetails()");

    //TODO: generate a temporary pattern from the scene data.

    //TODO: grab this information from the pattern
    stitchesTotal = 5; //TODO: embStitchList_count(pattern->stitchList, TOTAL);
    stitchesReal  = 4; //TODO: embStitchList_count(pattern->stitchList, NORMAL);
    stitchesJump  = 3; //TODO: embStitchList_count(pattern->stitchList, JUMP);
    stitchesTrim  = 2; //TODO: embStitchList_count(pattern->stitchList, TRIM);
    colorTotal    = 1; //TODO: embThreadList_count(pattern->threadList, TOTAL);
    colorChanges  = 0; //TODO: embThreadList_count(pattern->threadList, CHANGES);

    boundingRect.setRect(0, 0, 50, 100); //TODO: embPattern_calcBoundingBox(pattern);

    int bin[MAX_HISTOGRAM_BINS+1];

    QApplication::setOverrideCursor(Qt::ArrowCursor);

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern\n");
        return;
    }

    /* TODO: This is temporary. Generate actual pattern data from the scene. */

    /* embPattern_read(pattern, "/mydata/embroidery-designs/KDE.EXP"); */
    /* TODO: This convenience function is messed up. */

    /*
    EmbReaderWriter* reader = 0;
    QString tmpFileName = "/mydata/embroidery-designs/KDE.EXP";

    reader = embReaderWriter_getByFileName(qPrintable(tmpFileName));
    if (!reader) {
        debug_message("Unsupported read file type\n");
        embPattern_free(pattern);
        return;
    }
    else {
        readSuccessful = reader->reader(pattern, qPrintable(tmpFileName));
        if (!readSuccessful) {
            printf("Reading file was unsuccessful\n");
            embPattern_free(pattern);
            return;
        }
    }
    free(reader);

    EmbRect bounds = embPattern_calcBoundingBox(pattern);
    */

    EmbVector minimum, maximum, last_pos, delta;
    int colors = 1;
    int num_bins = 10;
    int real_stitches = 0;
    int jump_stitches = 0;
    int trim_stitches = 0;
    int unknown_stitches = 0;
    int num_colors = pattern->thread_list->count;
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
    last_pos.x = 0.0f;
    last_pos.y = 0.0f;
    delta.x = 0.0f;
    delta.y = 0.0f;

    if (pattern->stitch_list->count == 0) {
        QMessageBox::warning(_mainWin,
            translate_str("No Design Loaded"),
            translate_str("<b>A design needs to be loaded or created before details can be determined.</b>"));
        embPattern_free(pattern);
        return;
    }

    QVector<double> stitchLengths;
    double totalColorLength = 0.0;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - last_pos.x;
        delta.y = st.y - last_pos.y;
        last_pos.x = st.x;
        last_pos.y = st.y;
        length = embVector_length(delta);
        totalColorLength += length;
        // can't count first normal stitch
        if (i > 0 && pattern->stitch_list->stitch[i-1].flags != NORMAL) {
            length = 0.0;
        }
        if (!(st.flags & (JUMP | TRIM))) {
            real_stitches++;
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
            jump_stitches++;
        }
        if (st.flags & TRIM) {
            trim_stitches++;
        }
        if (st.flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if (st.flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
        if (st.flags | (JUMP & TRIM & STOP & END)) {
            unknown_stitches++;
        }
    }

    /* Second pass to fill bins now that we know max stitch length. */
    double binSize = max_stitchlength / num_bins;

    for (int i = 0; i <= num_bins; i++) {
        bin[i]=0;
    }

    last_pos.x = 0.0f;
    last_pos.y = 0.0f;
    for (int i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch st = pattern->stitch_list->stitch[i];
        delta.x = st.x - last_pos.x;
        delta.y = st.y - last_pos.y;
        if (i > 0) {
            if (pattern->stitch_list->stitch[i-1].flags == NORMAL
               && st.flags == NORMAL) {
                length = embVector_length(delta);
                bin[int(floor(num_bins*length/max_stitchlength))]++;
            }
        }
        last_pos.x = st.x;
        last_pos.y = st.y;
    }

    QString str;
    for (int i = 0; i < num_bins; i++) {
        str += QString::number(binSize * (i), 'f', 1) + " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    QDialog dialog(_mainWin);

    QGridLayout* grid = new QGridLayout(_mainWin);
    grid->setSpacing(2);

    grid->addWidget(new QLabel(translate_str("Stitches:")),0,0,1,1);
    grid->addWidget(new QLabel(QString::number(pattern->stitch_list->count)), 0, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Colors:")),1,0,1,1);
    grid->addWidget(new QLabel(QString::number(num_colors)), 1, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Jumps:")),2,0,1,1);
    grid->addWidget(new QLabel(QString::number(jump_stitches)), 2, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Top:")),3,0,1,1);
    //grid->addWidget(new QLabel(QString::number(bounds.top) + " mm"), 3, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Left:")),4,0,1,1);
    //grid->addWidget(new QLabel(QString::number(bounds.left) + " mm"), 4, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Bottom:")),5,0,1,1);
    //grid->addWidget(new QLabel(QString::number(bounds.bottom) + " mm"), 5, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Right:")),6,0,1,1);
    //grid->addWidget(new QLabel(QString::number(bounds.right) + " mm"), 6, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Width:")),7,0,1,1);
    //grid->addWidget(new QLabel(QString::number((bounds.right - bounds.left)) + " mm"), 7, 1, 1, 1);
    grid->addWidget(new QLabel(translate_str("Height:")),8,0,1,1);
    //grid->addWidget(new QLabel(QString::number((bounds.bottom - bounds.top)) + " mm"), 8, 1, 1, 1);
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

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    //connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    grid->addWidget(&buttonbox, currentRow, 0, 1, 2);

    dialog.setWindowTitle(translate_str("Embroidery Design Details"));
    dialog.setMinimumWidth(100);
    dialog.setMinimumHeight(50);
    dialog.setLayout(grid);
    dialog.exec();

    QApplication::restoreOverrideCursor();
}
