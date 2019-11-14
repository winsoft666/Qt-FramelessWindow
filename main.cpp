#include "QtFramelessWindowSample.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QtFramelessWindowSample w;
    w.show();
    return a.exec();
}
