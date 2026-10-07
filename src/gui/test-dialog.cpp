#include <QApplication>
#include <QtGui>
#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QScrollArea>
#include <QApplication>
#include <QGroupBox>

#include <vector>

#include "test-dialog.h"
#include "embroidery.h"
#include "../libembroidery/tests/tests.h"

TestDialog::TestDialog(QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    run_all_tests();
    /* NOTE: this has to be created after running tests */
    mainWidget = createMainWidget();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(mainWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Libembroidery Basic Testing"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

TestDialog::~TestDialog()
{
    QApplication::restoreOverrideCursor();
}

QGroupBox *testingGroupBox(QWidget *widget, QString gb_label, const char *key)
{
    int n = number_of_tests();
    QGroupBox* groupBox = new QGroupBox(gb_label, widget);
    QGridLayout* gridLayout = new QGridLayout(groupBox);

    int pos = 0;
    for (int i=0; i<n; i++) {
        if (!strncmp(test_list[i].category, key, 20)) {
            QString result = test_list[i].label;
            if (!test_list[i].result) {
                result += " FAIL";
            }
            else {
                result += " PASS";
            }
            QLabel *label = new QLabel(result);
            if (!test_list[i].result) {
                label->setStyleSheet("font-weight:bold; color:red");
            }

            int row = pos/3;
            int column = pos%3;
            pos++;
            gridLayout->addWidget(label, row, column, Qt::AlignRight);
        }
    }

    gridLayout->setColumnMinimumWidth(0, 100);
    groupBox->setLayout(gridLayout);
    return groupBox;
}

QWidget *TestDialog::createMainWidget()
{
    QWidget* widget = new QWidget(this);

    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(testingGroupBox(widget, tr("Binary"), "BINARY"));
    vboxLayoutMain->addWidget(testingGroupBox(widget, tr("Vector"), "VECTOR"));
    vboxLayoutMain->addWidget(testingGroupBox(widget, tr("Point"), "POINT"));
    vboxLayoutMain->addWidget(testingGroupBox(widget, tr("Format"), "FORMAT"));
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

