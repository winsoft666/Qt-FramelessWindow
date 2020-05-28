#include "MainWindowSample.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);

  MainWindowSample w;

  w.show();

  return a.exec();
}
