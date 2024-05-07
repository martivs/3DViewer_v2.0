#include "model.h"

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace s21 {

Model::~Model() {
  if (name_) free(name_);
  if (p_vertex_) free(p_vertex_);
  if (p_facet_) free(p_facet_);
}

// parser
int Model::Parser(const char *filename) noexcept {
  int flag = 0;
  int error_fscanf = 0;

  FILE *p_file = NULL;

  if ((p_file = fopen(filename, "r")) == NULL) {
    flag++;
  } else {
    setlocale(LC_NUMERIC, "C");
    char char_buffer;  // основной чар, для перебора файла
    double coord_x, coord_y, coord_z;
    char micro_buffer;  // субгеттер для фасетов

    while ((char_buffer = fgetc(p_file)) != EOF) {
      if (char_buffer == 'o') {
        if ((char_buffer = fgetc(p_file)) == ' ') {
          char tmp_var;

          int counter = 2;

          while ((tmp_var = fgetc(p_file)) != '\n') {
            if (this->name_ == NULL) {
              this->name_ = (char *)malloc(sizeof(char) * 2);
              this->name_[0] = tmp_var;
              this->name_[1] = '\0';
            } else {
              this->name_ =
                  (char *)realloc(this->name_, (counter + 1) * sizeof(char));
              this->name_[counter - 1] = tmp_var;
              this->name_[counter++] = '\0';
            }
          }
        }
      } else if (char_buffer == 'v') {
        if ((char_buffer = fgetc(p_file)) == ' ') {
          error_fscanf =
              fscanf(p_file, "%lf%lf%lf", &coord_x, &coord_y, &coord_z);
          if (!error_fscanf) flag++;
          this->vertex_amount_++;
          AddVertex(coord_x, coord_y, coord_z);
        }
      } else if (char_buffer == 'f') {
        int flag_divider =
            0;  // флаг проверяряющий что идет после числа.. разделяет логику
        // считывания фасетов через / и через пробел
        if ((char_buffer = fgetc(p_file)) == ' ') {
          int *tmp_int = NULL;  // массив для сбора интов из строки и передачи в
                                // обработчик фасета
          fseek(p_file, -1, SEEK_CUR);
          int flag_facet = 0;  // определитель пробела
          int digit = 0;  // число для записи в массив интов
          int counter = 0;  // счетчик кол-ва интов в массиве для передачи
          char *tmp_facet =
              NULL;  // массив для считывания инта из файла почарово

          while ((micro_buffer = fgetc(p_file)) != '\n') {
            if (micro_buffer == ' ' || flag_divider) {
              flag_facet = 1;
              flag_divider = 0;
              if ((micro_buffer = fgetc(p_file)) == '\n' ||
                  micro_buffer == ' ' || micro_buffer == '\r') {
                fseek(p_file, -1, SEEK_CUR);
                flag_facet = 0;
                continue;
              } else {
                fseek(p_file, -1, SEEK_CUR);
              }
            }
            if (flag_facet) {
              int size_tmp_facet = 0;  // размер массива чаров, для записи

              while ((micro_buffer = fgetc(p_file)) != '/' &&
                     micro_buffer != ' ' && micro_buffer != '\n' &&
                     micro_buffer != '\r' && micro_buffer != EOF) {
                if (tmp_facet == NULL) {
                  tmp_facet = (char *)malloc(sizeof(char));
                  tmp_facet[size_tmp_facet++] = micro_buffer;
                  flag_facet = 0;
                } else {
                  tmp_facet = (char *)realloc(
                      tmp_facet, sizeof(char) * (size_tmp_facet + 1));
                  tmp_facet[size_tmp_facet++] = micro_buffer;
                  flag_facet = 0;
                }
              }
              if (micro_buffer == ' ')  // считали пробел сразу после числа..
              // дальше идет новое значение
              {
                fseek(p_file, -1, SEEK_CUR);
                flag_divider = 1;
              } else if (micro_buffer ==
                         '\n')  // считали \n возвращаем указатель, что бы выйти
              // из основного цикла
              {
                fseek(p_file, -1, SEEK_CUR);
                flag_divider = 1;
              }
              if (micro_buffer != EOF) {
                tmp_facet = (char *)realloc(
                    tmp_facet, sizeof(char) * (size_tmp_facet + 1));
                tmp_facet[size_tmp_facet] = '\0';
                digit = atoi(tmp_facet);
                if (digit < 0) digit *= -1;
                free(tmp_facet);
                tmp_facet = NULL;
                size_tmp_facet = 0;
                if (tmp_int == NULL) {
                  tmp_int = (int *)malloc(sizeof(int));
                  tmp_int[counter++] = digit;
                } else {
                  tmp_int =
                      (int *)realloc(tmp_int, sizeof(int) * (counter + 1));
                  tmp_int[counter++] = digit;
                }
              }
            }
            if (micro_buffer == EOF) {
              break;
            }
          }
          if (micro_buffer == EOF) {
            break;
          }
          this->facet_amount_ += (counter * 2);
          AddFacet(tmp_int, counter);
          free(tmp_int);
          tmp_int = NULL;
          counter = 0;
        }
        if (micro_buffer == EOF) {
          break;
        }
      }
    }
  }

  if (p_file) fclose(p_file);

  return flag;
}

void Model::AddFacet(int *tmp_int, int counter) noexcept {
  if (this->p_facet_ == NULL) {
    this->p_facet_ = (int *)malloc(sizeof(int) * (this->facet_amount_));
  } else {
    this->p_facet_ =
        (int *)realloc(this->p_facet_, sizeof(int) * (this->facet_amount_));
  }
  int flag_first = 0;
  int i = this->facet_amount_ - counter * 2;
  int j = 0;
  do {
    if (!flag_first) {
      this->p_facet_[i++] = tmp_int[j++] - 1;
      flag_first = 1;
    } else if (j == counter) {
      this->p_facet_[i++] = tmp_int[0] - 1;
    } else {
      this->p_facet_[i++] = tmp_int[j] - 1;
      this->p_facet_[i++] = tmp_int[j++] - 1;
    }

  } while (i < (this->facet_amount_));
}

void Model::AddVertex(double coord_x, double coord_y, double coord_z) noexcept {
  if (this->p_vertex_ == NULL) {
    this->p_vertex_ = (double *)malloc(sizeof(double) * 3);
    this->p_vertex_[0] = coord_x;
    this->p_vertex_[1] = coord_y;
    this->p_vertex_[2] = coord_z;
  } else {
    this->p_vertex_ = (double *)realloc(
        this->p_vertex_, sizeof(double) * ((this->vertex_amount_ + 1) * 3));
    this->p_vertex_[this->vertex_amount_ * 3 - 3] = coord_x;
    this->p_vertex_[this->vertex_amount_ * 3 - 2] = coord_y;
    this->p_vertex_[this->vertex_amount_ * 3 - 1] = coord_z;
  }
}

// validator
int Model::Validate(const char *file_name) const noexcept {
  int is_err = 0;
  FILE *file;
  char buffer[1000];
  int v_count = 0;

  file = fopen(file_name, "r");

  if (file) {
    while (fgets(buffer, sizeof(buffer), file)) {
      if (!strncmp(buffer, "f ", 2) && !is_err)
        is_err = !CheckCorrectF(buffer);
      else if (!strncmp(buffer, "v ", 2) && !is_err) {
        v_count++;
        is_err = !CheckCorrectV(buffer);
      }
    }
    fclose(file);
  } else {
    is_err = 3;
  }
  if (v_count == 0 && !is_err) {
    is_err = 2;
  }
  return is_err;
}

int Model::CheckCorrectF(char *buf) const noexcept {
  // проверка корректности строки, начинающейся с f
  int is_correct = 1;

  char row_splitted_by_space[10][100] = {};
  int i = 0, j = 0;
  char current_string[100];

  while (sscanf(&buf[i], "%s ", current_string) == 1) {
    strcpy(row_splitted_by_space[j], current_string);
    i += strlen(current_string) + 1;
    j++;
  }

  if (j > 5) {
    is_correct = 0;
  } else {
    for (int idx = 1; idx < j; idx++) {
      is_correct = CheckCorrectDigitsFVar1(row_splitted_by_space[idx]) ||
                   CheckCorrectDigitsFVar2(row_splitted_by_space[idx]);
      if (!is_correct) break;
    }
  }

  return is_correct;
}

int Model::CheckCorrectV(char *buf) const noexcept {
  // проверка корректности строки, начинающейся с v
  int is_correct = 1;

  char row_splitted_by_space[10][100] = {};
  int i = 0, j = 0;
  char current_string[100];

  for (size_t i1 = 0; i1 <= strlen(buf); i1++)
    if ((buf[i1] == ' ') && (buf[i1 + 1] == ' ')) {
      for (size_t j1 = i1; j1 <= strlen(buf); j1++) buf[j1] = buf[j1 + 1];
      i1 = i1 - 1;
    }

  while (sscanf(&buf[i], "%s ", current_string) == 1) {
    strcpy(row_splitted_by_space[j], current_string);
    i += strlen(current_string) + 1;
    j++;
  }
  if (j > 5) {
    is_correct = 0;
  } else {
    for (int idx = 1; idx < j; idx++) {
      is_correct = CheckCorrectDigits(row_splitted_by_space[idx]);
      if (!is_correct) break;
    }
  }

  return is_correct;
}

int Model::CheckCorrectDigitsFVar1(char input_string[]) const noexcept {
  int is_correct = 1;

  char *istr;
  char sep[2] = "/";
  istr = strtok(input_string, sep);
  int j = 0;

  while (istr != NULL) {
    char current_string[100];
    strcpy(current_string, istr);
    size_t i = 0;
    int point_count = 0;
    while (strchr("-0123456789.", current_string[i]) &&
           i < strlen(current_string)) {
      if ((current_string[i] == '.')) {
        point_count++;
      }
      i++;
    }
    if (point_count > 0) {
      is_correct = 0;
      break;
    } else if (strlen(current_string) > i) {
      is_correct = 0;
      break;
    } else if (j > 2) {
      is_correct = 0;
      break;
    } else {
      istr = strtok(NULL, sep);
      j++;
    }
  }
  return is_correct;
}

int Model::CheckCorrectDigitsFVar2(char input_string[]) const noexcept {
  int is_correct = 1;
  char current_string[100];
  strcpy(current_string, input_string);
  size_t i = 0;
  int point_count = 0;
  while (strchr("-0123456789.", current_string[i]) &&
         i < strlen(current_string)) {
    if ((current_string[i] == '.')) {
      point_count++;
    }
    i++;
  }
  if (point_count > 0) {
    is_correct = 0;
  } else if (strlen(current_string) > i) {
    is_correct = 0;
  }
  return is_correct;
}

int Model::CheckCorrectDigits(char input_string[]) const noexcept {
  int is_correct = 1;
  char current_string[100];
  strcpy(current_string, input_string);
  size_t i = 0;
  int point_count = 0;
  while (strchr("-0123456789.", current_string[i]) &&
         i < strlen(current_string)) {
    if ((current_string[i] == '.')) {
      point_count++;
    }
    i++;
  }
  if (point_count > 1) {
    is_correct = 0;
  } else if (strlen(current_string) > i) {
    is_correct = 0;
  }
  return is_correct;
}

// getters
const double *Model::GetVertexArray() const noexcept {
  return this->p_vertex_;
};

const int *Model::GetFacetArray() const noexcept { return this->p_facet_; };

int Model::GetVertexAmount() const noexcept { return this->vertex_amount_; };

const char *Model::GetName() const noexcept { return this->name_; }

int Model::GetFacetAmount() const noexcept { return this->facet_amount_; };

int Model::GetDiametr() const noexcept { return diameter_; };

double Model::GetXCenter() const noexcept { return this->xCenter_; }

double Model::GetYCenter() const noexcept { return this->yCenter_; }

double Model::GetZCenter() const noexcept { return this->zCenter_; }

// calc etc
void Model::RecalcModel() noexcept {
  double xMin = p_vertex_[0];
  double xMax = p_vertex_[0];
  double yMin = p_vertex_[1];
  double yMax = p_vertex_[1];
  double zMin = p_vertex_[2];
  double zMax = p_vertex_[2];

  for (int i = 3; i < vertex_amount_ * 3; i += 3) {
    if (p_vertex_[i] < xMin) {
      xMin = p_vertex_[i];
    } else {
      if (p_vertex_[i] > xMax) {
        xMax = p_vertex_[i];
      }
    }
  }

  for (int i = 4; i < vertex_amount_ * 3; i += 3) {
    if (p_vertex_[i] < yMin) {
      yMin = p_vertex_[i];
    } else {
      if (p_vertex_[i] > yMax) {
        yMax = p_vertex_[i];
      }
    }
  }

  for (int i = 5; i < vertex_amount_ * 3; i += 3) {
    if (p_vertex_[i] < zMin) {
      zMin = p_vertex_[i];
    } else {
      if (p_vertex_[i] > zMax) {
        zMax = p_vertex_[i];
      }
    }
  }

  // set center
  xCenter_ = xMin + (xMax - xMin) / 2.0;
  yCenter_ = yMin + (yMax - yMin) / 2.0;
  zCenter_ = zMin + (zMax - zMin) / 2.0;

  // recalc model for camera
  for (int i = 0; i < vertex_amount_ * 3; i += 3) {
    p_vertex_[i] -= xCenter_;
  }

  for (int i = 1; i < vertex_amount_ * 3; i += 3) {
    p_vertex_[i] -= yCenter_;
  }

  for (int i = 2; i < vertex_amount_ * 3; i += 3) {
    p_vertex_[i] -= zCenter_;
  }

  // calc diameter for margins
  diameter_ = xMax - xMin;
  if (diameter_ < yMax - yMin) diameter_ = yMax - yMin;
  if (diameter_ < zMax - zMin) diameter_ = zMax - zMin;

  // reset center
  xCenter_ = 0;
  yCenter_ = 0;
  zCenter_ = 0;
}

// free
void Model::FreeModel() noexcept {
  vertex_amount_ = 0;
  facet_amount_ = 0;

  xCenter_ = 0.0;
  yCenter_ = 0.0;
  zCenter_ = 0.0;

  if (name_) {
    free(name_);
    name_ = nullptr;
  }

  if (p_vertex_) {
    free(p_vertex_);
    p_vertex_ = nullptr;
  }

  if (p_facet_) {
    free(p_facet_);
    p_facet_ = nullptr;
  }
}

void Model::Return(const double *p_vertex, double xCenter, double yCenter,
                   double zCenter) noexcept {
  for (int i = 0; i < 3 * vertex_amount_; i++) {
    this->p_vertex_[i] = p_vertex[i];
  }

  this->xCenter_ = xCenter;
  this->yCenter_ = yCenter;
  this->zCenter_ = zCenter;

  this->NotifyUpdate();
}

// transformations
void Model::Move(double xMove, double yMove, double zMove) noexcept {
  if (xMove != 0.0) {
    for (int i = 0; i < vertex_amount_ * 3; i += 3) {
      p_vertex_[i] += xMove;
    }
    xCenter_ += xMove;
  }

  if (yMove != 0.0) {
    for (int i = 1; i < vertex_amount_ * 3; i += 3) {
      p_vertex_[i] += yMove;
    }
    yCenter_ += yMove;
  }

  if (zMove != 0.0) {
    for (int i = 2; i < vertex_amount_ * 3; i += 3) {
      p_vertex_[i] += zMove;
    }
    zCenter_ += zMove;
  }

  this->NotifyUpdate();
}

void Model::Rotate(int line, double angle) noexcept {
  double temp;

  if (line == OX_ROTATE) {
    for (int i = 1; i < vertex_amount_ * 3; i += 3) {
      temp = p_vertex_[i] - yCenter_;
      // y
      p_vertex_[i] = temp * cos(angle) -
                     (p_vertex_[i + 1] - zCenter_) * sin(angle) + yCenter_;
      // z
      p_vertex_[i + 1] = temp * sin(angle) +
                         (p_vertex_[i + 1] - zCenter_) * cos(angle) + zCenter_;
    }
  }

  if (line == OY_ROTATE) {
    for (int i = 0; i < vertex_amount_ * 3; i += 3) {
      temp = p_vertex_[i] - xCenter_;
      // x
      p_vertex_[i] = temp * cos(angle) -
                     (p_vertex_[i + 2] - zCenter_) * sin(angle) + xCenter_;
      // z
      p_vertex_[i + 2] = temp * sin(angle) +
                         (p_vertex_[i + 2] - zCenter_) * cos(angle) + zCenter_;
    }
  }

  if (line == OZ_ROTATE) {
    for (int i = 0; i < vertex_amount_ * 3; i += 3) {
      temp = p_vertex_[i] - xCenter_;
      // x
      p_vertex_[i] = temp * cos(angle) -
                     (p_vertex_[i + 1] - yCenter_) * sin(angle) + xCenter_;
      // y
      p_vertex_[i + 1] = temp * sin(angle) +
                         (p_vertex_[i + 1] - yCenter_) * cos(angle) + yCenter_;
    }
  }

  this->NotifyUpdate();
}

void Model::Scaling(double k) noexcept {
  for (int i = 0; i < vertex_amount_ * 3; i += 3) {
    p_vertex_[i] = (p_vertex_[i] - xCenter_) * k + xCenter_;
    p_vertex_[i + 1] = (p_vertex_[i + 1] - yCenter_) * k + yCenter_;
    p_vertex_[i + 2] = (p_vertex_[i + 2] - zCenter_) * k + zCenter_;
  }

  this->NotifyUpdate();
};

}  // namespace s21
