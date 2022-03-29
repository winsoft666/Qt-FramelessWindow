#include "MainWindowSample.h"
#include "DialogSample.h"

MainWindowSample::MainWindowSample(QWidget* parent)
    : FramelessWindow<QMainWindow>(true, parent, true) {
  setupUi();
  setTitlebar({widgetTitle_, labelTitle_});
  setResizeable(true);
  loadStyleSheetFile(":/QtFramelessWindowSample/Resources/main.css", this);

  connect(pushButtonClose_, &QPushButton::clicked, [this]() { this->close(); });

  connect(pushButtonDialog_, &QPushButton::clicked, [this]() {
    DialogSample* pWnd = new DialogSample(this);
    pWnd->setResizeable(false);
    pWnd->setModal(true);
    pWnd->show();
  });

  connect(pushButtonResizableDialog_, &QPushButton::clicked, [this]() {
    DialogSample* pWnd = new DialogSample(this);
    pWnd->setResizeable(true);
    pWnd->setModal(true);
    pWnd->show();
  });
}

void MainWindowSample::resizeEvent(QResizeEvent* event) {
  labelTitle_->setText(
      QString("Frameless Window (Title Height: %1px)").arg(widgetTitle_->height()));
  pushButtonH_->setText(QString::number(pushButtonH_->width()));
  pushButtonV_->setText(QString::number(pushButtonV_->height()));

  QMainWindow::resizeEvent(event);
}

void MainWindowSample::setupUi() {
  this->setObjectName("FramelessWnd");

  widgetCentral_ = new QWidget();
  widgetCentral_->setObjectName("widgetCentral");

  widgetTitle_ = new QWidget();
  widgetTitle_->setObjectName("widgetTitle");

  labelTitle_ = new QLabel();
  labelTitle_->setObjectName("labelTitle");
  labelTitle_->setText("Frameless Window");

  pushButtonClose_ = new QPushButton();
  pushButtonClose_->setObjectName("pushButtonClose");
  pushButtonClose_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonClose_->setFixedSize(30, 30);

  pushButtonH_ = new QPushButton();
  pushButtonH_->setObjectName("pushButtonH");
  pushButtonH_->setText("--");
  pushButtonH_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonH_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  pushButtonH_->setFixedHeight(30);

  pushButtonV_ = new QPushButton();
  pushButtonV_->setObjectName("pushButtonV");
  pushButtonV_->setText("--");
  pushButtonV_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonV_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  pushButtonV_->setFixedWidth(30);

  pushButtonDialog_ = new QPushButton();
  pushButtonDialog_->setObjectName("pushButtonDialog");
  pushButtonDialog_->setText("Dialog");
  pushButtonDialog_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonDialog_->setFixedSize(80, 26);

  pushButtonResizableDialog_ = new QPushButton();
  pushButtonResizableDialog_->setObjectName("pushButtonResizableDialog");
  pushButtonResizableDialog_->setText("Resizable Dialog");
  pushButtonResizableDialog_->setCursor(QCursor(Qt::PointingHandCursor));
  pushButtonResizableDialog_->setFixedSize(160, 26);

  QHBoxLayout* hlTitle = new QHBoxLayout();
  hlTitle->setContentsMargins(10, 0, 10, 0);
  hlTitle->setSpacing(0);
  hlTitle->addWidget(labelTitle_);
  hlTitle->addStretch(2);
  hlTitle->addWidget(pushButtonDialog_);
  hlTitle->addStretch(1);
  hlTitle->addWidget(pushButtonClose_);

  widgetTitle_->setLayout(hlTitle);
  widgetTitle_->setFixedHeight(30);

  QGridLayout* gridBody = new QGridLayout();
  gridBody->setSpacing(0);
  gridBody->setContentsMargins(0, 0, 0, 0);
  gridBody->addWidget(pushButtonResizableDialog_, 0, 0, Qt::AlignCenter);
  gridBody->addWidget(pushButtonH_, 1, 0, 1, 3, Qt::AlignVCenter);
  gridBody->addWidget(pushButtonV_, 0, 2, 3, 1, Qt::AlignHCenter);

  QVBoxLayout* hlCenteral = new QVBoxLayout();
  hlCenteral->setSpacing(0);
  hlCenteral->setContentsMargins(2, 2, 2, 2);  // 2px border
  hlCenteral->addWidget(widgetTitle_);
  hlCenteral->addLayout(gridBody);
  widgetCentral_->setLayout(hlCenteral);

  this->setCentralWidget(widgetCentral_);

  this->setMinimumSize(800, 600);
}
