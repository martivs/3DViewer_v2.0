#include "observable.h"

#include <vector>

#include "observer.h"

namespace s21 {

Observable::Observable() {}

void Observable::AddObserver(Observer *observer) noexcept {
  observers_.push_back(observer);
}

void Observable::NotifyUpdate() const noexcept {
  int size = observers_.size();
  for (int i = 0; i < size; i++) {
    observers_[i]->Update();
  }
};

}  // namespace s21
