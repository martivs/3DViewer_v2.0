#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>

#include "observer.h"

namespace s21 {

class Observable {
 public:
  Observable();
  void AddObserver(Observer *observer) noexcept;
  void NotifyUpdate() const noexcept;

 private:
  std::vector<Observer *> observers_;
};
}  // namespace s21

#endif  // OBSERVABLE_H
