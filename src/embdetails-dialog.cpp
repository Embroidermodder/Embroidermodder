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

/**
 * \file object-rect.cpp
 */

#include "embroidermodder.h"

/**
 * @brief EmbDetailsDialog::EmbDetailsDialog
 * @param theScene
 * @param parent
 */
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

/**
 * @brief EmbDetailsDialog::~EmbDetailsDialog
 */
EmbDetailsDialog::~EmbDetailsDialog()
{
    QApplication::restoreOverrideCursor();
}

/**
 * @brief EmbDetailsDialog::getInfo
 */
void
EmbDetailsDialog::getInfo()
{
    //TODO: generate a temporary pattern from the scene data.

    //TODO: grab this information from the pattern
    stitchesTotal = 5; //TODO: embStitchList_count(pattern->stitchList, TOTAL);
    stitchesReal  = 4; //TODO: embStitchList_count(pattern->stitchList, NORMAL);
    stitchesJump  = 3; //TODO: embStitchList_count(pattern->stitchList, JUMP);
    stitchesTrim  = 2; //TODO: embStitchList_count(pattern->stitchList, TRIM);
    colorTotal    = 1; //TODO: embThreadList_count(pattern->threadList, TOTAL);
    colorChanges  = 0; //TODO: embThreadList_count(pattern->threadList, CHANGES);

    boundingRect.setRect(0, 0, 50, 100); //TODO: embPattern_calcBoundingBox(pattern);
}

/* EmbDetailsDialog::createMainWidget */
QWidget*
EmbDetailsDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);

    //Misc
    QGroupBox* groupBoxMisc = new QGroupBox(tr("General Information"), widget);

    QLabel* labelStitchesTotal = new QLabel(tr("Total Stitches:"), this);
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

/**
 * @brief MainWindow::designDetails
 *
 * \todo Move majority of this code into libembroidery

void MainWindow::designDetails()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("designDetails()");
    QString appName = QApplication::applicationName();
    QString title = "Design Details";

    EmbPattern* pattern = 0;

    //TODO: This is temporary. Generate actual pattern data from the scene.
    //======================================================
    //embPattern_read(pattern, "/mydata/embroidery-designs/KDE.EXP"); //TODO: This convenience function is messed up.

    EmbReaderWriter* reader = 0;
    int readSuccessful;
    QString tmpFileName = "/mydata/embroidery-designs/KDE.EXP";

    pattern = embPattern_create();
    if (!pattern) { printf("Could not allocate memory for embroidery pattern\n"); }

    readSuccessful = 0;
    reader = embReaderWriter_getByFileName(qPrintable(tmpFileName));
    if (!reader) {
        readSuccessful = 0;
        printf("Unsupported read file type\n");
    }
    else {
        readSuccessful = reader->reader(pattern, qPrintable(tmpFileName));
        if (!readSuccessful) printf("Reading file was unsuccessful\n");
    }
    free(reader);
    if (!readSuccessful) {
        embPattern_free(pattern);
    }

    EmbRect bounds = embPattern_calcBoundingBox(pattern);

    int colors = 1;
    int num_stitches = 0;
    int real_stitches = 0;
    int jump_stitches = 0;
    int trim_stitches = 0;
    int unknown_stitches = 0;
    int num_colors = 0;
    double minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    double xx = 0.0, yy = 0.0;
    double dx = 0.0, dy = 0.0;
    double length = 0.0;

    num_colors = embThreadList_count(pattern->threadList);
    num_stitches = embStitchList_count(pattern->stitchList);
    if (num_stitches == 0)
    {
        QMessageBox::warning(this, tr("No Design Loaded"), tr("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<double> stitchLengths;

    double totalColorLength = 0.0;
    for (int i = 0; i < num_stitches; i++)
    {
        dx = embStitchList_getAt(pattern->stitchList, i).xx - xx;
        dy = embStitchList_getAt(pattern->stitchList, i).yy - yy;
        xx = embStitchList_getAt(pattern->stitchList, i).xx;
        yy = embStitchList_getAt(pattern->stitchList, i).yy;
        length=sqrt(dx * dx + dy * dy);
        totalColorLength += length;
        if (i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags != NORMAL)
            length = 0.0; //can't count first normal stitch;
        if (!(embStitchList_getAt(pattern->stitchList, i).flags & (JUMP | TRIM)))
        {
            real_stitches++;
            if (length > max_stitchlength) { max_stitchlength = length; number_of_maxlength_stitches = 0; }
            if (length == max_stitchlength) number_of_maxlength_stitches++;
            if (length > 0 && length < min_stitchlength)
            {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if (length == min_stitchlength) number_of_minlength_stitches++;
            total_stitchlength += length;
            if (xx < minx) minx = xx;
            if (xx > maxx) maxx = xx;
            if (yy < miny) miny = yy;
            if (yy > maxy) maxy = yy;
        }
        if (embStitchList_getAt(pattern->stitchList, i).flags & JUMP)
        {
            jump_stitches++;
        }
        if (embStitchList_getAt(pattern->stitchList, i).flags & TRIM)
        {
            trim_stitches++;
        }
        if (embStitchList_getAt(pattern->stitchList, i).flags & STOP)
        {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if (embStitchList_getAt(pattern->stitchList, i).flags & END)
        {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    for (int i = 0; i <= NUMBINS; i++)
    {
        bin[i]=0;
    }

    for (int i = 0; i < num_stitches; i++)
    {
        dx = embStitchList_getAt(pattern->stitchList, i).xx - xx;
        dy = embStitchList_getAt(pattern->stitchList, i).yy - yy;
        xx = embStitchList_getAt(pattern->stitchList, i).xx;
        yy = embStitchList_getAt(pattern->stitchList, i).yy;
        if (i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags == NORMAL && embStitchList_getAt(pattern->stitchList, i).flags == NORMAL)
        {
            length=sqrt(dx * dx + dy * dy);
            bin[int(floor(NUMBINS*length/max_stitchlength))]++;
        }
    }

    double binSize = max_stitchlength / NUMBINS;

    QString str;
    for (int i = 0; i < NUMBINS; i++)
    {
        str += QString::number(binSize * (i), 'f', 1) + " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    QDialog dialog(this);

    QGridLayout* grid = new QGridLayout(this);
    grid->setSpacing(2);

    grid->addWidget(new QLabel(tr("Stitches:")),0,0,1,1);
    grid->addWidget(new QLabel(QString::number(num_stitches)), 0, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Colors:")),1,0,1,1);
    grid->addWidget(new QLabel(QString::number(num_colors)), 1, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Jumps:")),2,0,1,1);
    grid->addWidget(new QLabel(QString::number(jump_stitches)), 2, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Top:")),3,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.top) + " mm"), 3, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Left:")),4,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.left) + " mm"), 4, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Bottom:")),5,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.bottom) + " mm"), 5, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Right:")),6,0,1,1);
    grid->addWidget(new QLabel(QString::number(bounds.right) + " mm"), 6, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Width:")),7,0,1,1);
    grid->addWidget(new QLabel(QString::number((bounds.right - bounds.left)) + " mm"), 7, 1, 1, 1);
    grid->addWidget(new QLabel(tr("Height:")),8,0,1,1);
    grid->addWidget(new QLabel(QString::number((bounds.bottom - bounds.top)) + " mm"), 8, 1, 1, 1);
    grid->addWidget(new QLabel(tr("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(tr("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;
*/
/*
    for (int i = 0; i < num_colors; i++)
    {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        qDebug("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }
*/
/*
    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    grid->addWidget(&buttonbox, currentRow, 0, 1, 2);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(100);
    dialog.setMinimumHeight(50);
    dialog.setLayout(grid);
    dialog.exec();
    QApplication::restoreOverrideCursor();

}
*/
