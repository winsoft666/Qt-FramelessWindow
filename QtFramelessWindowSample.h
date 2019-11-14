#pragma once

#include "FramelessMainWindow.h"
#include "ui_QtFramelessWindowSample.h"

class QtFramelessWindowSample : public FramelessMainWindow
{
    Q_OBJECT

public:
    QtFramelessWindowSample(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtFramelessWindowSampleClass ui;
};
