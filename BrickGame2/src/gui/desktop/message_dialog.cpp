#include "message_dialog.h"

MessageDialog::MessageDialog(const QString& title, const QString& message,
                             QWidget* parent)
    : QDialog(parent) {
  setWindowTitle(title);
  setFixedSize(300, 100);
  setModal(true);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  QHBoxLayout* buttonLayout = new QHBoxLayout();

  messageLabel = new QLabel(message, this);
  messageLabel->setWordWrap(true);
  messageLabel->setAlignment(Qt::AlignCenter);

  okButton = new QPushButton("OK", this);
  okButton->setFixedSize(80, 30);

  mainLayout->addWidget(messageLabel);
  mainLayout->addStretch();
  buttonLayout->addStretch();
  buttonLayout->addWidget(okButton);
  buttonLayout->addStretch();
  mainLayout->addLayout(buttonLayout);

  connect(okButton, &QPushButton::clicked, this, &MessageDialog::onOkClicked);
  connect(okButton, &QPushButton::clicked, this, &MessageDialog::accept);

  centerOnScreen();
}

void MessageDialog::centerOnScreen() {
  QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
  if (!screen) {
    screen = QGuiApplication::primaryScreen();
  }

  QRect screenGeometry = screen->availableGeometry();
  int x = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
  int y = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

  move(x, y);
}

void MessageDialog::onOkClicked() { accept(); }

void MessageDialog::showMessage(const QString& title, const QString& message,
                                QWidget* parent) {
  MessageDialog dialog(title, message, parent);
  dialog.exec();
}