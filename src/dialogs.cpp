/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Dialogs
 */

#include "embroidermodder.h"

/* Show the about dialog. */
int
about_dialog_c(void)
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    debug("about()");
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(_mainWin);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName
        + translate_str("\n\n")
        + translate_str("http://www.libembroidery.org")
        + translate_str("\n\n")
        + translate_str("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi")
        + translate_str("\n\n")
        + translate_str("Embroidery formats by Josh Varga.") +
                          translate_str("\n") +
                          translate_str("User Interface by Jonathan Greig.") +
                          translate_str("\n\n") +
                          translate_str("Free under the zlib/libpng license.")
#if defined(BUILD_GIT_HASH)
                          + translate_str("\n\n") +
                          translate_str("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
#endif
    );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    _mainWin->connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&img);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(img.minimumWidth()+30);
    dialog.setMinimumHeight(img.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
    return 0;
}

/* Show the changelog dialog. */
int
changelog_dialog_c(void)
{
    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
    return 0;
}

/* . */
int
details_dialog_c(void)
{
    QGraphicsScene* scene = activeScene();
    if (scene) {
        EmbDetailsDialog dialog(scene, _mainWin);
        dialog.exec();
    }
    return 0;
}

/* . */
int
help_dialog_c(void)
{
    // Open the HTML Help in the default browser
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    /* TODO: This is how to start an external program. Use this elsewhere...
     * QString program = "firefox";
     * QStringList arguments;
     * arguments << "help/commands.html";
     * QProcess *myProcess = new QProcess(this);
     * myProcess->start(program, arguments);
     */
    return 0;
}

/* . */
int
settings_dialog_c(void)
{
    _mainWin->settingsDialog("General");
    return 0;
}

/* . */
int
tip_of_the_day_dialog_c(void)
{
    _mainWin->tipOfTheDay();
    return 0;
}

void
MainWindow::tipOfTheDay()
{
    debug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    StringList tip_list = string_tables["tips"];
    if (st[ST_CURRENT_TIP].i >= tip_list.size()) {
        st[ST_CURRENT_TIP].i = 0;
    }
    labelTipOfTheDay = new QLabel(tip_list[st[ST_CURRENT_TIP].i].c_str(), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    checkBoxTipOfTheDay->setChecked(st[ST_TIP_OF_THE_DAY].b);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, tr("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, tr("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, tr("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void
MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    st[ST_TIP_OF_THE_DAY].b = checked;
}

void
MainWindow::buttonTipOfTheDayClicked(int button)
{
    debug("buttonTipOfTheDayClicked(%d)", button);
    StringList tip_list = string_tables["tips"];
    if (button == QWizard::CustomButton1) {
        if (st[ST_CURRENT_TIP].i > 0) {
            st[ST_CURRENT_TIP].i--;
        }
        else {
            st[ST_CURRENT_TIP].i = tip_list.size() - 1;
        }
        labelTipOfTheDay->setText(tip_list[st[ST_CURRENT_TIP].i].c_str());
    }
    else if(button == QWizard::CustomButton2) {
        st[ST_CURRENT_TIP].i++;
        if (st[ST_CURRENT_TIP].i >= tip_list.size()) {
            st[ST_CURRENT_TIP].i = 0;
        }
        labelTipOfTheDay->setText(tip_list[st[ST_CURRENT_TIP].i].c_str());
    }
    else if(button == QWizard::CustomButton3) {
        wizardTipOfTheDay->close();
    }
}

