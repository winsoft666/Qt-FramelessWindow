#include "SettingWnd.h"
#include <QGraphicsDropShadowEffect>

// Support Chinese
#pragma execution_character_set("utf-8")

SettingWnd::SettingWnd(QWidget* parent /*= nullptr*/)
    : FramelessWindow(true, false, false, parent)
    , shadow_(nullptr) {
  setAttribute(Qt::WA_DeleteOnClose, true);
  setupUi();
  this->setWindowTitle("设置");
  this->setWindowIcon(QIcon(":/LiveWallpaper/Resources/wallpaper.ico"));
  setTitlebar({ centralWidget_ }, {}, {"QAbstractButton"});
  loadStyleSheetFile(":/QtFramelessWindowSample/Resources/SetttingWnd.css", this);

  setShadowEnable(true);

  connect(pushButtonClose_, &QPushButton::clicked, [this]() { close(); });
}

SettingWnd::~SettingWnd() {}


void SettingWnd::closeEvent(QCloseEvent* event) {
  event->accept();
}

void SettingWnd::setupUi() {
  this->setObjectName("SettingWnd");

  centralWidget_ = new QWidget();
  centralWidget_->setObjectName("centralWidget");

  pushButtonClose_ = new QPushButton();
  pushButtonClose_->setObjectName("pushButtonClose");
  pushButtonClose_->setFixedSize(30, 30);
  pushButtonClose_->setCursor(QCursor(Qt::PointingHandCursor));

  checkboxAutoStart_ = new QCheckBox();
  checkboxAutoStart_->setObjectName("checkboxAutoStart");
  checkboxAutoStart_->setText("开机时自动启动");
  checkboxAutoStart_->setCursor(QCursor(Qt::PointingHandCursor));

  labelSaveDir_ = new QLabel();
  labelSaveDir_->setObjectName("labelWallpaperSaveDir");
  labelSaveDir_->setText("存放目录: ");

  pushButtonSaveDir_ = new QPushButton();
  pushButtonSaveDir_->setObjectName("pushButtonWallpaperSaveDir");
  pushButtonSaveDir_->setText("D:\\Test\123");
  pushButtonSaveDir_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonSaveDir_->setToolTip(pushButtonSaveDir_->text());

  pushButtonChangeSaveDir_ = new QPushButton();
  pushButtonChangeSaveDir_->setObjectName("pushButtonChangeWallpaperSaveDir");
  pushButtonChangeSaveDir_->setText("更改存放目录");
  pushButtonChangeSaveDir_->setFixedSize(150, 35);
  pushButtonChangeSaveDir_->setCursor(QCursor(Qt::PointingHandCursor));

  labelCurVer_ = new QLabel();
  labelCurVer_->setObjectName("labelCurVer");
  labelCurVer_->setText(QString("当前版本号: 1.0.0.1"));

  labelTitle_ = new QLabel();
  labelTitle_->setObjectName("labelTitle");
  labelTitle_->setText("设置");

  QHBoxLayout* hlTitle = new QHBoxLayout();
  hlTitle->setContentsMargins(0, 0, 0, 0);
  hlTitle->setSpacing(0);
  hlTitle->addWidget(labelTitle_);
  hlTitle->addStretch();
  hlTitle->addWidget(pushButtonClose_);

  QHBoxLayout* hlWallpaperSD = new QHBoxLayout();
  hlWallpaperSD->setContentsMargins(0, 0, 0, 0);
  hlWallpaperSD->setSpacing(0);
  hlWallpaperSD->addWidget(labelSaveDir_);
  hlWallpaperSD->addWidget(pushButtonSaveDir_);
  hlWallpaperSD->addStretch();

  QVBoxLayout* vLayout = new QVBoxLayout();
  vLayout->setSpacing(20);
  vLayout->setContentsMargins(20, 0, 10, 0);
  vLayout->addLayout(hlTitle);
  vLayout->addWidget(checkboxAutoStart_);
  vLayout->addLayout(hlWallpaperSD);
  vLayout->addWidget(pushButtonChangeSaveDir_);
  vLayout->addStretch();
  vLayout->addWidget(labelCurVer_);
  vLayout->addSpacing(20);

  centralWidget_->setLayout(vLayout);

  QHBoxLayout* hMain = new QHBoxLayout();
  hMain->setContentsMargins(0, 0, 0, 0);
  hMain->addWidget(centralWidget_);
  this->setLayout(hMain);

  this->setContentsMargins(22, 22, 22, 22);

  QMetaObject::connectSlotsByName(this);

  this->setFixedSize(436, 330);
}

void SettingWnd::setShadowEnable(bool enable) {
  if (!shadow_) {
    shadow_ = new QGraphicsDropShadowEffect();
    shadow_->setColor(QColor(2, 122, 255, 255));
    shadow_->setBlurRadius(20);
    shadow_->setOffset(0.0);
  }

  centralWidget_->setGraphicsEffect(enable ? shadow_ : nullptr);
}

void SettingWnd::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      break;
    default:
      QDialog::keyPressEvent(event);
  }
}