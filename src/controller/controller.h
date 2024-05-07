#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
#include "../model/movecommand.h"
#include "../model/snapshot.h"

namespace s21 {

class Controller {
 public:
  Controller();
  Controller(Model *model) : model_(model), snapshot_(nullptr){};

  int Validate(const char *file_name) const noexcept;
  int Parser(const char *file_name) const noexcept;

  void PreparationModel() noexcept;
  void FreeModel() const noexcept;

  // model info
  const char *GetModelName() const noexcept;
  const double *GetModelVertex() const noexcept;
  int GetModelVertexAmount() const noexcept;
  const int *GetModelFacet() const noexcept;
  int GetModelFacetAmount() const noexcept;
  double GetModelDiametr() const noexcept;

  // snapshot
  void MakeSnapshot() noexcept;

  // transformations
  void MoveModel(double xMove, double yMove, double zMove) const noexcept;
  void RotateModel(int line, double angle) const noexcept;
  void ScallingModel(double k) const noexcept;
  void ReturnModel() const noexcept;

 private:
  // ---------------------------------------------- fields
  Model *model_;
  Snapshot *snapshot_;
};

}   // namespace s21

#endif  // CONTROLLER_H
