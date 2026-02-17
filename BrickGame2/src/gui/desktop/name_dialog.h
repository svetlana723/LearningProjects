#ifndef NAME_DIALOG_H
#define NAME_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

#include "view_defines.h"

class NameDialog : public QDialog {
 public:
  explicit NameDialog(QWidget *parent = nullptr);
  std::string getName() const;

 private:
  void onAccept();
  QLineEdit *nameEdit;
  QString userName;
};

#endif  // NAME_DIALOG_H