#include "DialogSample.h"
#include <QGraphicsDropShadowEffect>

namespace {
void loadStyleSheetFile(const QString& sheetName, QWidget* widget) {
    if (widget) {
        QString qss;
        QFile qssFile(sheetName);
        qssFile.open(QFile::ReadOnly);
        if (qssFile.isOpen()) {
            qss = QString::fromUtf8(qssFile.readAll());
            widget->setStyleSheet(qss);
            qssFile.close();
        }
    }
}
}

DialogSample::DialogSample(QWidget* parent /*= nullptr*/)
    : FramelessWindow<QDialog>(true, parent, true), shadow_(nullptr) {
    setAttribute(Qt::WA_DeleteOnClose, true);
    setupUi();
    setWindowTitle("Dialog Sample");

    setTitlebar({centralWidget_, labelTitle_});
    loadStyleSheetFile(":/QtFramelessWindowSample/Resources/DialogSample.css", this);

    setShadowEnable(true);

    connect(pushButtonClose_, &QPushButton::clicked, [this]() { close(); });
}

DialogSample::~DialogSample() {}

void DialogSample::setupUi() {
    this->setObjectName("SettingWnd");

    centralWidget_ = new QWidget();
    centralWidget_->setObjectName("centralWidget");

    pushButtonClose_ = new QPushButton();
    pushButtonClose_->setObjectName("pushButtonClose");
    pushButtonClose_->setFixedSize(30, 30);
    pushButtonClose_->setCursor(QCursor(Qt::PointingHandCursor));

    labelSaveDir_ = new QLabel();
    labelSaveDir_->setObjectName("labelWallpaperSaveDir");
    labelSaveDir_->setText("Directory: ");

    pushButtonSaveDir_ = new QPushButton();
    pushButtonSaveDir_->setObjectName("pushButtonWallpaperSaveDir");
    pushButtonSaveDir_->setText("C:\\Users\\JEFFERY\\AppData\\Local");
    pushButtonSaveDir_->setCursor(QCursor(Qt::PointingHandCursor));
    pushButtonSaveDir_->setToolTip(pushButtonSaveDir_->text());

    pushButtonChangeSaveDir_ = new QPushButton();
    pushButtonChangeSaveDir_->setObjectName("pushButtonChangeWallpaperSaveDir");
    pushButtonChangeSaveDir_->setText("Update Directory");
    pushButtonChangeSaveDir_->setFixedSize(150, 35);
    pushButtonChangeSaveDir_->setCursor(QCursor(Qt::PointingHandCursor));

    labelTitle_ = new QLabel();
    labelTitle_->setObjectName("labelTitle");
    labelTitle_->setText("Dialog Sample");

    QHBoxLayout* hlTitle = new QHBoxLayout();
    hlTitle->setContentsMargins(0, 0, 0, 0);
    hlTitle->setSpacing(0);
    hlTitle->addWidget(labelTitle_);
    hlTitle->addStretch();
    hlTitle->addWidget(pushButtonClose_);

    QHBoxLayout* hlSD = new QHBoxLayout();
    hlSD->setContentsMargins(0, 0, 0, 0);
    hlSD->setSpacing(0);
    hlSD->addWidget(labelSaveDir_);
    hlSD->addWidget(pushButtonSaveDir_);
    hlSD->addStretch();

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setSpacing(20);
    vLayout->setContentsMargins(20, 0, 10, 0);
    vLayout->addLayout(hlTitle);
    vLayout->addLayout(hlSD);
    vLayout->addWidget(pushButtonChangeSaveDir_);
    vLayout->addStretch();

    centralWidget_->setLayout(vLayout);

    QHBoxLayout* hMain = new QHBoxLayout();
    hMain->setContentsMargins(0, 0, 0, 0);
    hMain->addWidget(centralWidget_);
    this->setLayout(hMain);

    this->setContentsMargins(25, 25, 25, 25);

    this->setMinimumSize(450, 350);
}

void DialogSample::setShadowEnable(bool enable) {
    if (!shadow_) {
        shadow_ = new QGraphicsDropShadowEffect();
        shadow_->setColor(QColor(2, 122, 255, 255));
        shadow_->setBlurRadius(20);
        shadow_->setOffset(0.0);
    }

    centralWidget_->setGraphicsEffect(enable ? shadow_ : nullptr);
}
