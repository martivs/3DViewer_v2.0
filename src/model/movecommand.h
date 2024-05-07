#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "command.h"
#include "model.h"
#include "snapshot.h"

namespace s21 {

class MoveCommand : public Command {
 public:
  MoveCommand(Model *model, double xMove, double yMove, double zMove)
      : model_(model), xMove_(xMove), yMove_(yMove), zMove_(zMove){};

  void Execute() const noexcept { model_->Move(xMove_, yMove_, zMove_); };

 private:
  Model *model_;
  double xMove_;
  double yMove_;
  double zMove_;
  Snapshot snapshot_;
};

}  // namespace s21

#endif  // MOVECOMMAND_H
