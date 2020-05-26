#include "QtFramelessWindowSample.h"
#include "SettingWnd.h"
// Support Chinese
#pragma execution_character_set("utf-8")

QtFramelessWindowSample::QtFramelessWindowSample(QWidget *parent)
    : FramelessWindow(true, false, true, parent) // Titlebar widget has border radius so set translucentBackground true.
{
    ui.setupUi(this);

    setTitlebar({ ui.widgetTitlebar, ui.labelTitle }, {ui.pushButtonClose, ui.pushButtonDialog}, {});


    loadStyleSheetFile(":/QtFramelessWindowSample/Resources/main.css", this);


    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        this->close();
    });

    connect(ui.pushButtonDialog, &QPushButton::clicked, [this]() { 
      SettingWnd* pWnd = new SettingWnd(this);
      pWnd->setModal(true);
      pWnd->show();
    });
}
