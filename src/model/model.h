#ifndef MODEL_H
#define MODEL_H

#include "observable.h"

#define OX_ROTATE 1
#define OY_ROTATE 2
#define OZ_ROTATE 3

namespace s21 {

class Model : public Observable {
 public:
  Model()
      : name_(nullptr),
        p_vertex_(nullptr),
        vertex_amount_(0),
        p_facet_(nullptr),
        facet_amount_(0),
        xCenter_(0.0),
        yCenter_(0.0),
        zCenter_(0.0),
        diameter_(0.0){};

  ~Model();

  int Parser(const char *file_name) noexcept;  // returns 0 -- OK

  int Validate(const char *file_name) const noexcept;
  // returns 0 -- OK
  // returns 1 -- есть строки, начинающиеся НЕ с "#", "o ", "v ", "f ", "vt ",
  // "vn ", "vp", "g " или внутри строк с правильным началом есть ошибки (буквы
  // среди цифр, неправильный формат) returns 2 -- нет строк с вершинами ("v ")
  // returns 3 - нет файла

  // getters
  const double *GetVertexArray() const noexcept;
  const int *GetFacetArray() const noexcept;
  const char *GetName() const noexcept;
  int GetVertexAmount() const noexcept;
  int GetFacetAmount() const noexcept;
  int GetDiametr() const noexcept;

  double GetXCenter() const noexcept;
  double GetYCenter() const noexcept;
  double GetZCenter() const noexcept;

  // transformations
  void Move(double xMove, double yMove, double zMove) noexcept;
  void Rotate(int line, double angle) noexcept;
  void Scaling(double k) noexcept;
  void Return(const double *p_vertex, double xCenter, double yCenter,
              double zCenter) noexcept;

  // recalc etc
  void RecalcModel() noexcept;
  void FreeModel() noexcept;

 private:
  char *name_;
  double *p_vertex_;
  int vertex_amount_;
  int *p_facet_;
  int facet_amount_;
  double xCenter_;
  double yCenter_;
  double zCenter_;

  // for widget margins
  double diameter_;

  // parser
  void AddVertex(double coord_x, double coord_y, double coord_z) noexcept;
  void AddFacet(int *tmp_int, int counter) noexcept;

  // validator
  int CheckCorrectF(char *buf) const noexcept;
  int CheckCorrectV(char *buf) const noexcept;
  int CheckCorrectDigitsFVar1(char input_string[]) const noexcept;
  int CheckCorrectDigitsFVar2(char input_string[]) const noexcept;
  int CheckCorrectDigits(char input_string[]) const noexcept;
};

}  // namespace s21

#endif  // MODEL_H
