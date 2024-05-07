#ifndef SCALINGCOMMAND_H
#define SCALINGCOMMAND_H

#include "command.h"
#include "model.h"
#include "snapshot.h"

namespace s21 {

class ScalingCommand : Command {
 public:
  explicit ScalingCommand(Model *model, double k) : model_(model), k_(k){};
  void Execute() const noexcept { model_->Scaling(k_); };

 private:
  Model *model_;
  double k_;
  Snapshot snapshot_;
};

}  // namespace s21

#endif  // SCALINGCOMMAND_H
