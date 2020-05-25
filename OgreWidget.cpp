#include "OgreWidget.h"
#include "OgreRoot.h"
#include <iostream>

OgreWidget::OgreWidget()
{
    mOgreRenderWindow = nullptr;

    //create the Ogre window
    Ogre::NameValuePairList params;
    params["gamma"] = "true";
    //and set this QWindow as the handle
    params["externalWindowHandle"] = Ogre::StringConverter::toString( ulong( this->winId() ) );
    mOgreRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("OgreTest", 1280, 720, false, &params);

    //sice we are a QWindow we need to create a self container which is a QWidget, that way
    //we can use our widget as any other widget in a Qt window
    mWindowContainer = QWidget::createWindowContainer( this, nullptr, Qt::FramelessWindowHint );

}

//Rezise Window Event
void OgreWidget::resizeEvent( QResizeEvent* rEvent ) {
    Q_UNUSED( rEvent );
    if( mOgreRenderWindow ){
        mOgreRenderWindow->windowMovedOrResized();
        mOgreRenderWindow->reposition(0,0);
    }
}
