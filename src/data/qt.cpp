/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * C++ Structured Data 
 */

#include "embroidermodder.h"

std::unordered_map<int, QAction*> actionHash;
std::unordered_map<QString, QToolBar*> toolbarHash;
std::unordered_map<QString, QMenu*> menuHash;
const char *settings_file = "settings.toml";

QList<QGraphicsItem*> cutCopyObjectList;

QString formatFilterOpen;
QString formatFilterSave;

QStatusBar* statusbar;
MdiArea* mdiArea;
CmdPrompt* prompt;
PropertyEditor* dockPropEdit;
UndoEditor* dockUndoEdit;
QTimer* testing_timer;

QList<MdiWindow*> listMdiWin;
QString openFilesPath;

QAction* myFileSeparator;

QWizard* wizardTipOfTheDay;
QLabel* labelTipOfTheDay;
QCheckBox* checkBoxTipOfTheDay;

/* Selectors */
QComboBox* layerSelector;
QComboBox* colorSelector;
QComboBox* linetypeSelector;
QComboBox* lineweightSelector;
QFontComboBox* textFontSelector;
QComboBox* textSizeSelector;

QByteArray layoutState;

MainWindow *_main;

StringMap aliases[MAX_ALIASES];
std::unordered_map<std::string, std::string> aliasHash;

QString curText;
QString defaultPrefix;
QString prefix;

QString lastCmd;
QString curCmd;

QString prompt_color_;
QString prompt_selection_bg_color_;
QString prompt_bg_color_;
QString prompt_selection_color_;

QTextBrowser* promptHistory;
CmdPromptInput* promptInput;

QTimer* blinkTimer;
bool blinkState;
/* NOTE: These shortcuts need to be caught since QLineEdit uses them. */

std::unordered_map<int, int> key_map = {
    {QKeySequence::Cut, CUT_SEQUENCE},
    {QKeySequence::Copy, COPY_SEQUENCE},
    {QKeySequence::Paste, PASTE_SEQUENCE},
    {QKeySequence::SelectAll, SELECT_ALL_SEQUENCE},
    {QKeySequence::Undo, UNDO_SEQUENCE},
    {QKeySequence::Redo, REDO_SEQUENCE},
    {Qt::Key_Delete, DELETE_KEY},
    {Qt::Key_Tab, TAB_KEY},
    {Qt::Key_Escape, ESCAPE_KEY},
    {Qt::Key_Up, UP_KEY},
    {Qt::Key_Down, DOWN_KEY},
    {Qt::Key_F1, F1_KEY},
    {Qt::Key_F2, F2_KEY},
    {Qt::Key_F3, F3_KEY},
    {Qt::Key_F4, F4_KEY},
    {Qt::Key_F5, F5_KEY},
    {Qt::Key_F6, F6_KEY},
    {Qt::Key_F7, F7_KEY},
    {Qt::Key_F8, F8_KEY},
    {Qt::Key_F9, F9_KEY},
    {Qt::Key_F10, F10_KEY},
    {Qt::Key_F11, F11_KEY},
    {Qt::Key_F12, F12_KEY},
    {Qt::Key_Shift, SHIFT_KEY}
};

QToolButton* statusBarSnapButton;
QToolButton* statusBarGridButton;
QToolButton* statusBarRulerButton;
QToolButton* statusBarOrthoButton;
QToolButton* statusBarPolarButton;
QToolButton* statusBarQSnapButton;
QToolButton* statusBarQTrackButton;
QToolButton* statusBarLwtButton;
QLabel* statusBarMouseCoord;

QStringList button_list = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT"
};

