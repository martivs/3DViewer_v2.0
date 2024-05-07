#include "myglwidget.h"

myglwidget::myglwidget(QWidget *parent) : QOpenGLWidget(parent) {
  // for mouse events
  flag_basis_ = false;

  // angles
  xRot_ = 0.0;
  yRot_ = 0.0;

  coef_diameter_ = 1.2;

  //  init. fields
  p_vertex_ = nullptr;
  vertex_amount_ = 0;
  p_facet_ = nullptr;
  facet_amount_ = 0;

  // load settings :)
  LoadSettings();
}

myglwidget::~myglwidget() { SaveSettings(); }

// ----------------------------------------------------------- pravate
// GL trinity
void myglwidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);   // depth buffer
  glEnable(GL_CULL_FACE);    // do not draw backs
  glEnable(GL_LINE_SMOOTH);  // smooth lines
}

// void myglwidget::resizeGL(int w, int h) { ;}

void myglwidget::paintGL() {
  // background
  glClearColor(set.color_background_.redF(), set.color_background_.greenF(),
               set.color_background_.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color & buffer

  // projection
  glMatrixMode(GL_MODELVIEW);  // glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (set.flag_projection_) {
    glFrustum(-diameter_ * 0.7, diameter_ * 0.7, -diameter_ * 0.7,
              diameter_ * 0.7, diameter_, diameter_ * 15);
    glTranslated(0, 0, -diameter_ * 2);

  } else {
    glOrtho(-diameter_ * coef_diameter_, diameter_ * coef_diameter_,
            -diameter_ * coef_diameter_, diameter_ * coef_diameter_,
            -diameter_ * coef_diameter_ * 10, diameter_ * coef_diameter_ * 10);
  }

  // rotate basis
  glRotated(xRot_, 1, 0, 0);  // rotate basis around Ox
  glRotated(yRot_, 0, 1, 0);  // rotate basis around Oy

  draw_model_();
}

void myglwidget::draw_model_() {
  glVertexPointer(3, GL_DOUBLE, 0, p_vertex_);
  glEnableClientState(GL_VERTEX_ARRAY);

  if (set.flag_vertex_) {
    if (set.flag_round_) {
      glEnable(GL_POINT_SMOOTH);  // round dots on
    } else {
      glDisable(GL_POINT_SMOOTH);  // round dots off
    }
    glColor3f(set.color_vertex_.redF(), set.color_vertex_.greenF(),
              set.color_vertex_.blueF());
    glPointSize(set.w_vertex_);                  // dots size
    glDrawArrays(GL_POINTS, 0, vertex_amount_);  // attention!
  }

  if (set.flag_facet_) {
    if (set.flag_dashed_) {
      glEnable(GL_LINE_STIPPLE);  // dashed facets on
      glLineStipple(2, 0x3333);
    } else {
      glDisable(GL_LINE_STIPPLE);  // dashed facets off
    }
    glColor3f(set.color_facet_.redF(), set.color_facet_.greenF(),
              set.color_facet_.blueF());
    glLineWidth(set.w_facet_);  // facets size
    glDrawElements(GL_LINES, facet_amount_, GL_UNSIGNED_INT, p_facet_);
  }

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

// mouse events
void myglwidget::mousePressEvent(QMouseEvent *m) {
  if (m->buttons() & Qt::LeftButton) {
    flag_basis_ = 0;
    mouse_position_ = m->pos();
  } else if (m->buttons() & Qt::RightButton) {
    flag_basis_ = 1;
  }
}

void myglwidget::mouseMoveEvent(QMouseEvent *m) {
  if (!flag_basis_) {
    xRot_ += (0.05 / M_PI) * (m->pos().y() - mouse_position_.y());
    yRot_ += (0.05 / M_PI) * (m->pos().x() - mouse_position_.x());
    emit basis_view_changed();
  }

  this->update();
}

// ----------------------------------------------------------------- save & load
// settings
void myglwidget::SaveSettings() noexcept {
  FILE *file = fopen(SETT_FILE_NAME, "wb");

  if (fwrite(&set, sizeof(settings), 1, file) != 1)
    QMessageBox::critical(this, nullptr, "File write error!");

  if (file != NULL) fclose(file);
}

void myglwidget::LoadSettings() noexcept {
  FILE *file = NULL;

  if ((file = fopen(SETT_FILE_NAME, "rb")) == NULL) {
    loadSettingsDefault();
  } else {
    if (fread(&set, sizeof(settings), 1, file) != 1) {
      QMessageBox::critical(
          this, nullptr,
          "File read error!\nThe default settings will be loaded");
      loadSettingsDefault();
    }
  }

  if (file != NULL) fclose(file);
}

void myglwidget::loadSettingsDefault() noexcept {
  set.flag_facet_ = true;
  set.flag_vertex_ = false;
  set.flag_full_ = false;
  set.flag_dashed_ = false;
  set.flag_round_ = false;
  set.w_facet_ = 1.0;
  set.w_vertex_ = 2.0;
  set.flag_projection_ = false;
  set.color_vertex_ = QColor(0, 255, 0);
  set.color_facet_ = QColor(0, 255, 0);
  set.color_background_ = QColor(0, 0, 0);
}

// ----------------------------------------------------------------- reset
void myglwidget::ResetBasis() {
  xRot_ = 0.0;
  yRot_ = 0.0;
  update();
}

// ----------------------------------------------------------------- setters
void myglwidget::SetVertex(const double *ptr) noexcept { p_vertex_ = ptr; }

void myglwidget::SetVertexAmount(int n) noexcept { vertex_amount_ = n; }

void myglwidget::SetFacet(const int *ptr) noexcept { p_facet_ = ptr; }

void myglwidget::SetFacetAmount(int n) noexcept { facet_amount_ = n; }

void myglwidget::SetDiametr(double d) noexcept { diameter_ = d; };
