#include "embroidermodder.h"

MainWindow* _mainWin = 0;

//NOTE: qthelp://com.trolltech.qt.470/qdoc/scripting.html
//NOTE: Wrapping a Native Function
//NOTE: It is currently not possible to wrap member functions; i.e., methods of a C++ class that require a this object.
void initMainWinPointer(MainWindow* mw)
{
    _mainWin = mw;
}

MainWindow* mainWin()
{
    return _mainWin;
}
