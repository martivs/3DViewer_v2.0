#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

#include "../controller/controller.h"
#include "../model/observable.h"
#include "../model/observer.h"

using namespace s21;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, Observer {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(Observable *observable, Controller *controller) : MainWindow() {
    observable_ = observable;
    observable_->AddObserver(this);
    controller_ = controller;
  };

  ~MainWindow();
  void Update();  // observer

 private:
  Ui::MainWindow *ui;
  Observable *observable_;
  Controller *controller_;
  bool flag_open;  // open file

  // reset transformation tools
  void set_tools();
  void reset_tools();

 private slots:

  // open file
  void on_pushButton_1_clicked();

  // style
  void on_radio_vertex_clicked(bool checked);
  void on_radio_facet_clicked(bool checked);
  void on_radio_full_clicked(bool checked);
  void on_dial_facet_valueChanged(int value);
  void on_dial_vertex_valueChanged(int value);
  void on_checkBox_dashed_clicked();
  void on_checkBox_round_clicked();

  // set colors
  void on_pushButton_setColor_clicked();
  void on_pushButton_setColor_2_clicked();
  void on_pushButton_setColor_3_clicked();

  // reset basis
  void on_pushButton_resetBasis_clicked();
  void lock_resetBasis();  // slot for signal

  // transformations
  void on_pushButton_go_clicked();
  void on_pushButton_rotate_clicked();
  void on_pushButton_scale_clicked();

  // cancel transformation
  void on_pushButton_resetMove_clicked();
  void on_pushButton_rotateReset_clicked();
  void on_pushButton_moveReset_clicked();

  // central projection
  void on_checkBox_projection_clicked(bool checked);

  // screenshots
  void on_pushButton_bmp_clicked();
  void on_pushButton_jpg_clicked();
  void on_pushButton_gif_clicked();
};

#endif  // MAINWINDOW_H
