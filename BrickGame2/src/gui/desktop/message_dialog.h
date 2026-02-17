#ifndef MESSAGE_DIALOG_H
#define MESSAGE_DIALOG_H

#include <QApplication>
#include <QCursor>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QStyle>
#include <QVBoxLayout>

#include "view_defines.h"

class MessageDialog : public QDialog {
 public:
  explicit MessageDialog(const QString& title, const QString& message,
                         QWidget* parent = nullptr);

  static void showMessage(const QString& title, const QString& message,
                          QWidget* parent = nullptr);

 private:
  void centerOnScreen();

  void onOkClicked();

  QLabel* messageLabel;
  QPushButton* okButton;
};

#endif  // MESSAGE_DIALOG_H