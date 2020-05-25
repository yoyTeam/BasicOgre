#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "OgreRoot.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent* event ) {
    //this is not the propper way to end the Ogre thread
    Ogre::Root::getSingletonPtr()->shutdown();
}
