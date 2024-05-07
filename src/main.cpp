#include <QApplication>

#include "mainwindow.h"

// #include "parser.h"

#include "controller/controller.h"
#include "model/model.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Model model;
  Controller controller(&model);
  MainWindow window(&model, &controller);
  window.setFixedSize(1000, 650);
  window.show();

  return a.exec();
}
