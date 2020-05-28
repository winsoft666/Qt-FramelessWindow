#pragma once

#include "FramelessWindow.hpp"
#include <QtWidgets>

class MainWindowSample : public FramelessWindow<QMainWindow> {
  Q_OBJECT

 public:
  MainWindowSample(QWidget* parent = Q_NULLPTR);

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  void setupUi();

  QWidget* widgetCentral_;
  QWidget* widgetTitle_;
  QLabel* labelTitle_;
  QPushButton* pushButtonClose_;
  QPushButton* pushButtonDialog_;
  QPushButton* pushButtonResizableDialog_;
  QPushButton* pushButtonH_;
  QPushButton* pushButtonV_;
};
