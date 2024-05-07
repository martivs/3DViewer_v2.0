#include "controller.h"

namespace s21 {

    Controller::Controller() {}

    int Controller::Validate(const char *file_name) const noexcept {
      return model_->Validate(file_name);
    }

    int Controller::Parser(const char *file_name) const noexcept {
      return model_->Parser(file_name);
    }

    void Controller::FreeModel() const noexcept { model_->FreeModel(); }

    void Controller::PreparationModel() noexcept {
      model_->RecalcModel();
      MakeSnapshot();
    }

    // model info
    const char *Controller::GetModelName() const noexcept {
      return model_->GetName();
    }

    int Controller::GetModelVertexAmount() const noexcept {
      return model_->GetVertexAmount();
    }

    int Controller::GetModelFacetAmount() const noexcept {
      return model_->GetFacetAmount();
    }

    const double *Controller::GetModelVertex() const noexcept {
      return model_->GetVertexArray();
    }

    const int *Controller::GetModelFacet() const noexcept {
      return model_->GetFacetArray();
    }

    double Controller::GetModelDiametr() const noexcept {
      return model_->GetDiametr();
    }

    // snapshot
    void Controller::MakeSnapshot() noexcept {
      if (snapshot_) delete snapshot_;
      snapshot_ = new Snapshot(model_->GetVertexArray(), model_->GetVertexAmount(),
                               model_->GetXCenter(), model_->GetYCenter(),
                               model_->GetZCenter());
    }

    // transformations
    void Controller::MoveModel(double xMove, double yMove,
                               double zMove) const noexcept {
      MoveCommand(model_, xMove, yMove, zMove).Execute();
    }

    void Controller::RotateModel(int line, double angle) const noexcept {
      model_->Rotate(line, angle);
    }

    void Controller::ScallingModel(double k) const noexcept {
      this->model_->Scaling(k);
    }

    void Controller::ReturnModel() const noexcept {
      snapshot_->Restore(this->model_);
    };

}  // namespace s21
