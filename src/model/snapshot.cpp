#include "snapshot.h"

namespace s21 {

Snapshot::Snapshot(const double *p_vertex, int vertex_amount, double xCenter,
                   double yCenter, double zCenter) {
  this->p_vertex_ = new double[3 * vertex_amount];

  for (int i = 0; i < 3 * vertex_amount; i++) {
    this->p_vertex_[i] = p_vertex[i];
  }

  xCenter_ = xCenter;
  yCenter_ = yCenter;
  zCenter_ = zCenter;

  vertex_amount_ = vertex_amount;
};

Snapshot::Snapshot(Snapshot &source) noexcept {
  this->p_vertex_ = new double[source.vertex_amount_];
  for (int i = 0; i < 3 * source.vertex_amount_; i++) {
    this->p_vertex_[i] = source.p_vertex_[i];
  }

  this->xCenter_ = source.xCenter_;
  this->yCenter_ = source.yCenter_;
  this->zCenter_ = source.zCenter_;
  this->vertex_amount_ = source.vertex_amount_;

  delete[] source.p_vertex_;
  source.p_vertex_ = nullptr;
}

Snapshot::Snapshot(Snapshot &&source) noexcept {
  this->p_vertex_ = source.p_vertex_;
  source.p_vertex_ = nullptr;

  this->xCenter_ = source.xCenter_;
  this->yCenter_ = source.yCenter_;
  this->zCenter_ = source.zCenter_;
  this->vertex_amount_ = source.vertex_amount_;
}

Snapshot::~Snapshot() {
  if (p_vertex_) {
    delete[] p_vertex_;
    p_vertex_ = nullptr;
  }
}

void Snapshot::Restore(Model *model) const noexcept {
  model->Return(this->p_vertex_, this->xCenter_, this->yCenter_,
                this->zCenter_);
};

}  // namespace s21
