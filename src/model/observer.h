#ifndef OBSERVER_H
#define OBSERVER_H

namespace s21 {

class Observer {
 public:
  Observer();
  virtual void Update() = 0;
};
}  // namespace s21

#endif  // OBSERVER_H
