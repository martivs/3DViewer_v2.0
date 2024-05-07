#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "model.h"

namespace s21 {

class Snapshot {
 public:
  Snapshot()
      : p_vertex_(nullptr),
        xCenter_(0),
        yCenter_(0),
        zCenter_(0),
        vertex_amount_(0.0){};

  explicit Snapshot(const double *p_vertex, int vertex_amount, double xCenter,
           double yCenter, double zCenter);
  Snapshot(Snapshot &source) noexcept;
  Snapshot(Snapshot &&source) noexcept;
  ~Snapshot();
  void Restore(Model *model) const noexcept;

 private:
  double *p_vertex_;
  double xCenter_;
  double yCenter_;
  double zCenter_;
  int vertex_amount_;
};

}  // namespace s21

#endif  // SNAPSHOT_H
