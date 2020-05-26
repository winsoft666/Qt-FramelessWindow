#include "QtFramelessWindowSample.h"

QtFramelessWindowSample::QtFramelessWindowSample(QWidget *parent)
    : FramelessWindow(true, false, true, parent) // Titlebar widget has border radius so set translucentBackground true.
{
    ui.setupUi(this);

    // Optional
    setResizeable(true);
    setTitlebar({ ui.widgetTitlebar, ui.labelTitle }, {ui.pushButtonClose}, {});


    loadStyleSheetFile(":/QtFramelessWindowSample/Resources/main.css", this);


    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        this->close();
    });
}
