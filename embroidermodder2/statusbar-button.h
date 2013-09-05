#ifndef STATUSBARBUTTON_H
#define STATUSBARBUTTON_H

#include <QToolButton>

class MainWindow;
class StatusBar;

class StatusBarButton : public QToolButton
{
    Q_OBJECT

public:
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *event = 0);

private slots:
    void settingsSnap();
    void settingsGrid();
    void settingsRuler();
    void settingsOrtho();
    void settingsPolar();
    void settingsQSnap();
    void settingsQTrack();
    void settingsLwt();
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
    void enableReal();
    void disableReal();

private:
    MainWindow* mainWin;
    StatusBar*  statusbar;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
