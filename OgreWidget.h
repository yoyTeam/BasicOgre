#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <QWindow>
#include <QMainWindow>
#include "OgreWindow.h"

class OgreWidget : public QWindow
{
public:
    OgreWidget();
    QWidget* mWindowContainer;
    Ogre::Window* mOgreRenderWindow;
private:
    void resizeEvent( QResizeEvent* rEvent ) override;
};

#endif // OGREWIDGET_H
