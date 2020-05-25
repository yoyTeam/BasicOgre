#include "yEngine.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    yEngine* engine = new yEngine();
    engine->init();

#ifdef USE_QT_WINDOW
    //create a main Qt Window
    MainWindow* mainWindow = new MainWindow();

    //set its size and make it centered to the screen
    QDesktopWidget widget;
    QRect mainScreenRect = widget.availableGeometry(widget.primaryScreen());
    mainWindow->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, QSize(640,480), mainScreenRect ) );
    mainWindow->show();

    //set the OgreWidget as central
    mainWindow->setCentralWidget( engine->getOgreWidget());
#endif

    //start the actual Ogre Render thread
    engine->startRender();

    return 0;
}
