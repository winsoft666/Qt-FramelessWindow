#pragma once

#include <QtWidgets>
#include "FramelessWindow.hpp"

class QGraphicsDropShadowEffect;

class SettingWnd : public FramelessWindow<QDialog> {
  Q_OBJECT
 public:
  SettingWnd(QWidget* parent = nullptr);
  ~SettingWnd();


 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void closeEvent(QCloseEvent* event) override;
  void setupUi();

  void setShadowEnable(bool enable);

  QWidget* centralWidget_;
  QPushButton* pushButtonClose_;
  QCheckBox* checkboxAutoStart_;
  QLabel* labelSaveDir_;
  QPushButton* pushButtonChangeSaveDir_;
  QLabel* labelCurVer_;
  QLabel* labelTitle_;
  QPushButton* pushButtonSaveDir_;

  QGraphicsDropShadowEffect* shadow_;
};