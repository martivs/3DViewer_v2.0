#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QColor>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLWidget>

#define SETT_FILE_NAME "3dvsetsettings"

typedef struct settings {
  bool flag_facet_;
  bool flag_vertex_;
  bool flag_full_;
  bool flag_dashed_;
  bool flag_round_;
  float w_facet_;
  float w_vertex_;
  bool flag_projection_;
  QColor color_vertex_;
  QColor color_facet_;
  QColor color_background_;
} settings;

//

class myglwidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  myglwidget(QWidget *parent = Q_NULLPTR);

  settings set;  // mode view settings

  // ---------------------------------------------- methods
  ~myglwidget();

  // setters
  void SetVertex(const double *ptr) noexcept;
  void SetVertexAmount(int n) noexcept;
  void SetFacet(const int *ptr) noexcept;
  void SetFacetAmount(int n) noexcept;
  void SetDiametr(double d) noexcept;

  // reset
  void ResetBasis();

 private:
  // ---------------------------------------------- fields

  // from model
  const double *p_vertex_;
  int vertex_amount_;
  const int *p_facet_;
  int facet_amount_;
  double diameter_;

  // for widget margins
  double coef_diameter_;

  // rotate basis
  bool flag_basis_;
  double xRot_, yRot_;
  QPoint mouse_position_;

  // ---------------------------------------------- methods
  // GL trinity
  void initializeGL() override;
  void paintGL() override;
  //  resizeGL(int w, int h); // in future

  // draw model
  void draw_model_();

  // preparation model
  void recalc_model_();

  // mouse events
  void mousePressEvent(QMouseEvent *m) override;
  void mouseMoveEvent(QMouseEvent *m) override;

  // save & load settings
  void SaveSettings() noexcept;
  void LoadSettings() noexcept;
  void loadSettingsDefault() noexcept;

 signals:
  void basis_view_changed();
};

#endif  // MYGLWIDGET_H
