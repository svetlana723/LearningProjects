#include "name_dialog.h"

NameDialog::NameDialog(QWidget *parent) : QDialog(parent), userName("") {
  setWindowTitle("Введите ваше имя");
  setFixedSize(300, 150);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *label = new QLabel(ENTER_NAME_MESSAGE, this);
  nameEdit = new QLineEdit(this);
  QPushButton *okButton = new QPushButton("OK", this);

  layout->addWidget(label);
  layout->addWidget(nameEdit);
  layout->addWidget(okButton);

  connect(okButton, &QPushButton::clicked, this, &NameDialog::onAccept);
  connect(nameEdit, &QLineEdit::returnPressed, this, &NameDialog::onAccept);
}

std::string NameDialog::getName() const { return userName.toStdString(); }

void NameDialog::onAccept() {
  userName = nameEdit->text().trimmed();
  if (!userName.isEmpty()) {
    accept();
  }
}