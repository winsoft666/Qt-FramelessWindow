#include "QtFramelessWindowSample.h"

QtFramelessWindowSample::QtFramelessWindowSample(QWidget *parent)
    : FramelessMainWindow(true, parent) // Titlebar widget has border radius so set translucentBackground true.
{
    ui.setupUi(this);

    // Necessary
    FramelessMainWindow::setAllWidgetMouseTracking(this);

    // Optional
    setResizeable(true);
    setTitlebar({ ui.widgetTitlebar, ui.labelTitle });


    FramelessMainWindow::loadStyleSheetFile(":/QtFramelessWindowSample/Resources/main.css", this);


    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        this->close();
    });
}
