#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

#include <QKeyEvent>
#include <QObject>

class BaseController : public QObject {
 public:
  explicit BaseController(QObject *parent = nullptr) : QObject(parent) {}

  virtual ~BaseController() = default;

  virtual void HandleKeyPress(QKeyEvent *event) = 0;
};

#endif  // BASE_CONTROLLER_H