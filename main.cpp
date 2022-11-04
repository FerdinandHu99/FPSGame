#include "mainwindow.h"
#include "fopenglwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FOpenGLWidget w;
    w.show();

    return a.exec();
}
