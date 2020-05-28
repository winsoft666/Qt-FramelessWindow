#pragma once

#include <QtWidgets>
#include "FramelessWindow.hpp"

class QGraphicsDropShadowEffect;

class DialogSample : public FramelessWindow<QDialog> {
  Q_OBJECT
 public:
  DialogSample(QWidget* parent = nullptr);
  ~DialogSample();

 protected:
  void setupUi();

  void setShadowEnable(bool enable);

 protected:
  QWidget* centralWidget_;
  QPushButton* pushButtonClose_;
  QLabel* labelSaveDir_;
  QPushButton* pushButtonChangeSaveDir_;
  QLabel* labelTitle_;
  QPushButton* pushButtonSaveDir_;

  QGraphicsDropShadowEffect* shadow_;
};