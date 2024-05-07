#ifndef COMMAND_H
#define COMMAND_H

namespace s21 {

class Command {
 public:
  Command();
  // virtual ~Command() {}
  virtual void Execute() const noexcept = 0;
  // virtual void Return() const noexcept = 0;
};
}  // namespace s21

#endif  // COMMAND_H
