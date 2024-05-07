#ifndef ROTATECOMMAND_H
#define ROTATECOMMAND_H

#include "command.h"
#include "model.h"
#include "snapshot.h"

namespace s21 {

class RotateCommand : Command {
 public:
  explicit RotateCommand(Model *model, int line, double angle)
      : model_(model), line_(line), angle_(angle){};

  void Execute() const noexcept { model_->Rotate(line_, angle_); };

 private:
  Model *model_;
  int line_;
  double angle_;
  Snapshot snapshot_;
};

}  // namespace s21
#endif  // ROTATECOMMAND_H
