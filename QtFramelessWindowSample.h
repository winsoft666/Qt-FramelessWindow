#pragma once

#include "FramelessWindow.hpp"
#include "ui_QtFramelessWindowSample.h"

class QtFramelessWindowSample : public FramelessWindow<QMainWindow>
{
    Q_OBJECT

public:
    QtFramelessWindowSample(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtFramelessWindowSampleClass ui;
};
