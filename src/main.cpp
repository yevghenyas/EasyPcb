#include <QApplication>
#include "easypcb.h"
#include "mywidget.h"
#include <QMainWindow>
#include "mainwindow.h"


int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    MainWindow window;
    window.show();
    return a.exec();
}

