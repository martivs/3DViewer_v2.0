#include "mainwindow.h"

#include <stdlib.h>

#include <string>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // open file
  flag_open = false;

  // hide checkbox projection
  ui->checkBox_projection->hide();

  // for hide button Reset on glwidget
  ui->pushButton_resetBasis->setVisible(false);
  connect(ui->glWidget, SIGNAL(basis_view_changed()), this,
          SLOT(lock_resetBasis()));

  // lock return & screenshot
  ui->pushButton_resetMove->setEnabled(false);
  ui->pushButton_bmp->setEnabled(false);
  ui->pushButton_jpg->setEnabled(false);
  ui->pushButton_gif->setEnabled(false);

  // set tools from set
  set_tools();

  ui->pushButton_gif->setVisible(false);  // ¯\_(ツ)_/¯
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Update() { ui->glWidget->update(); }

// open file
void MainWindow::on_pushButton_1_clicked() {
  QString q_file_name = QFileDialog::getOpenFileName();

  if (!q_file_name.isEmpty()) {
    std::string cpp_file_name = q_file_name.toStdString();
    const char *c_file_name = cpp_file_name.c_str();

    if (controller_->Validate(c_file_name)) {
      QMessageBox::critical(this, nullptr, "Incorrect file!");
    } else {
      // second open
      if (flag_open) {
        // free model
        controller_->FreeModel();
      } else {
        flag_open = true;
      }
      // reset spinboxes
      reset_tools();
      // parsing & preparation model
      if (controller_->Parser(c_file_name)) {
        QMessageBox::critical(this, nullptr, "Parser error!");
      } else {
        controller_->PreparationModel();

        // filename to statusbar
        ui->statusbar->showMessage(QString(" file: %1").arg(q_file_name));

        // model name on glwidget
        if (controller_->GetModelName()) {
          ui->label_modelName->setText(controller_->GetModelName());
        } else {
          ui->label_modelName->setText("no name");
        }

        // amount on glwidget
        ui->label_vertex_amount->setText(
            QString("vertex: %1").arg(controller_->GetModelVertexAmount()));
        ui->label_facet_amount->setText(
            QString("facet: %1").arg(controller_->GetModelFacetAmount()));

        // set glwidget fields
        ui->glWidget->SetVertex(controller_->GetModelVertex());
        ui->glWidget->SetVertexAmount(controller_->GetModelVertexAmount());
        ui->glWidget->SetFacet(controller_->GetModelFacet());
        ui->glWidget->SetFacetAmount(controller_->GetModelFacetAmount());
        ui->glWidget->SetDiametr(controller_->GetModelDiametr());

        // reset basis and show
        ui->checkBox_projection->show();
        ui->pushButton_resetBasis->hide();
        ui->pushButton_bmp->setEnabled(true);
        ui->pushButton_jpg->setEnabled(true);
        ui->pushButton_gif->setEnabled(true);
        ui->glWidget->ResetBasis();
      }
    }
  }
}

// ----------------------------------------------------------------- style
void MainWindow::on_radio_vertex_clicked(bool checked) {
  if (checked) {
    ui->glWidget->set.flag_vertex_ = true;
    ui->glWidget->set.flag_facet_ = false;
    ui->glWidget->set.flag_full_ = false;
  }

  ui->glWidget->update();
}

void MainWindow::on_radio_facet_clicked(bool checked) {
  if (checked) {
    ui->glWidget->set.flag_vertex_ = false;
    ui->glWidget->set.flag_facet_ = true;
    ui->glWidget->set.flag_full_ = false;
  }

  ui->glWidget->update();
}

void MainWindow::on_radio_full_clicked(bool checked) {
  if (checked) {
    ui->glWidget->set.flag_vertex_ = true;
    ui->glWidget->set.flag_facet_ = true;
    ui->glWidget->set.flag_full_ = true;
  }

  ui->glWidget->update();
}

void MainWindow::on_dial_facet_valueChanged(int value) {
  ui->glWidget->set.w_facet_ = (float)value;
  ui->glWidget->update();
}

void MainWindow::on_dial_vertex_valueChanged(int value) {
  ui->glWidget->set.w_vertex_ = (float)value;
  ui->glWidget->update();
}

void MainWindow::on_checkBox_dashed_clicked() {
  if (ui->checkBox_dashed->isChecked()) {
    ui->glWidget->set.flag_dashed_ = true;
    ui->glWidget->update();
  } else {
    ui->glWidget->set.flag_dashed_ = false;
    ui->glWidget->update();
  }
}

void MainWindow::on_checkBox_round_clicked() {
  if (ui->checkBox_round->isChecked()) {
    ui->glWidget->set.flag_round_ = true;
    ui->glWidget->update();
  } else {
    ui->glWidget->set.flag_round_ = false;
    ui->glWidget->update();
  }
}

// ----------------------------------------------------------------- colors
void MainWindow::on_pushButton_setColor_clicked() {
  QColor tmp =
      QColorDialog::getColor(Qt::white, this, tr("Select color lapochka!"));
  if (tmp.isValid()) {
    ui->glWidget->set.color_facet_ = tmp;
    ui->glWidget->update();
  }
}

void MainWindow::on_pushButton_setColor_2_clicked() {
  QColor tmp =
      QColorDialog::getColor(Qt::white, this, tr("Select color lapochka!"));
  if (tmp.isValid()) {
    ui->glWidget->set.color_vertex_ = tmp;
    ui->glWidget->update();
  }
}

void MainWindow::on_pushButton_setColor_3_clicked() {
  QColor tmp =
      QColorDialog::getColor(Qt::white, this, tr("Select color lapochka!"));
  if (tmp.isValid()) {
    ui->glWidget->set.color_background_ = tmp;
    ui->glWidget->update();
  }
}

// ----------------------------------------------------------------- basis
// slot for signal from glvidget
void MainWindow::lock_resetBasis() {
  ui->pushButton_resetBasis->setVisible(true);
}

void MainWindow::on_pushButton_resetBasis_clicked() {
  ui->glWidget->ResetBasis();
  ui->pushButton_resetBasis->setVisible(false);
}

// -----------------------------------------------------------------
// transformations
void MainWindow::on_pushButton_go_clicked() {
  if ((ui->doubleSpinBox_xMove->value() != 0.0) ||
      (ui->doubleSpinBox_yMove->value() != 0.0) ||
      (ui->doubleSpinBox_zMove->value() != 0.0)) {
    controller_->MoveModel(ui->doubleSpinBox_xMove->value(),
                           ui->doubleSpinBox_yMove->value(),
                           ui->doubleSpinBox_zMove->value());
  }

  ui->pushButton_resetMove->setEnabled(true);
}

void MainWindow::on_pushButton_rotate_clicked() {
  // around Ox
  if (ui->spinBox_xRotate->value()) {
    controller_->RotateModel(OX_ROTATE,
                             ui->spinBox_xRotate->value() * M_PI / 180.00);
  }

  // around Oy
  if (ui->spinBox_yRotate->value()) {
    controller_->RotateModel(OY_ROTATE,
                             ui->spinBox_yRotate->value() * M_PI / 180.00);
  }

  // around Oz
  if (ui->spinBox_zRotate->value()) {
    controller_->RotateModel(OZ_ROTATE,
                             ui->spinBox_zRotate->value() * M_PI / 180.00);
  }

  // enable "Return" button
  if (ui->spinBox_xRotate->value() || ui->spinBox_yRotate->value() ||
      ui->spinBox_zRotate->value()) {
    ui->pushButton_resetMove->setEnabled(true);
  }
}

void MainWindow::on_pushButton_scale_clicked() {
  if (ui->doubleSpinBox_Scale->value() == 0.0) {
    QMessageBox::information(this, "I think that...",
                             "the coefficients should be non-zero");
  } else {
    controller_->ScallingModel(ui->doubleSpinBox_Scale->value());
    ui->pushButton_resetMove->setEnabled(true);
  }
}

// ----------------------------------------------------------------- reset tools
void MainWindow::on_pushButton_moveReset_clicked() {
  ui->doubleSpinBox_xMove->setValue(0.0);
  ui->doubleSpinBox_yMove->setValue(0.0);
  ui->doubleSpinBox_zMove->setValue(0.0);
}

void MainWindow::reset_tools() {
  ui->doubleSpinBox_xMove->setValue(0.0);
  ui->doubleSpinBox_yMove->setValue(0.0);
  ui->doubleSpinBox_zMove->setValue(0.0);

  ui->spinBox_xRotate->setValue(0);
  ui->spinBox_yRotate->setValue(0);
  ui->spinBox_zRotate->setValue(0);

  ui->doubleSpinBox_Scale->setValue(0.0);
}

void MainWindow::on_pushButton_rotateReset_clicked() {
  ui->spinBox_xRotate->setValue(0);
  ui->spinBox_yRotate->setValue(0);
  ui->spinBox_zRotate->setValue(0);
}

// ----------------------------------------------------------------- set tools
void MainWindow::set_tools() {
  // mode view
  if (ui->glWidget->set.flag_full_) {
    ui->radio_full->setChecked(true);
  } else {
    if (ui->glWidget->set.flag_facet_) {
      ui->radio_facet->setChecked(true);
    } else {
      ui->radio_vertex->setChecked(true);
    }
  }

  // vertex & facet properties
  if (ui->glWidget->set.flag_dashed_) ui->checkBox_dashed->setChecked(true);
  if (ui->glWidget->set.flag_round_) ui->checkBox_round->setChecked(true);

  ui->dial_facet->setValue(ui->glWidget->set.w_facet_);
  ui->dial_vertex->setValue(ui->glWidget->set.w_vertex_);

  if (ui->glWidget->set.flag_projection_)
    ui->checkBox_projection->setChecked(true);
}

// ----------------------------------------------------------------- return
// model
void MainWindow::on_pushButton_resetMove_clicked() {
  controller_->ReturnModel();
  ui->pushButton_resetMove->setEnabled(false);
}

void MainWindow::on_checkBox_projection_clicked(bool checked) {
  ui->glWidget->set.flag_projection_ = checked;
  ui->glWidget->update();
}

// ----------------------------------------------------------------- screenshot
void MainWindow::on_pushButton_bmp_clicked() {
  QString filename =
      QFileDialog::getSaveFileName(this, "screenshot.bmp", NULL, "BMP (*.bmp)");
  if (!filename.isNull()) {
    this->ui->glWidget->grabFramebuffer().save(filename, NULL, -1);
  }
}

void MainWindow::on_pushButton_jpg_clicked() {
  QString filename = QFileDialog::getSaveFileName(this, "screenshot.jpg", NULL,
                                                  "JPEG (*.jpg)");
  if (!filename.isNull()) {
    this->ui->glWidget->grabFramebuffer().save(filename, NULL, -1);
  }
}

void MainWindow::on_pushButton_gif_clicked() {
  QImage captireImage;

  ui->glWidget->makeCurrent();

  QPixmap pixmap = QPixmap::fromImage(ui->glWidget->grabFramebuffer());
  captireImage = pixmap.toImage();

  //    QtGiImage gif;
}
