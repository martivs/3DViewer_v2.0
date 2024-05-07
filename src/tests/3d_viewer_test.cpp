#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../model/model.h"

#define FILE_NAME "tests/cube.obj"
#define BAD_FILE_NAME_1 "tests/cube_bad_1.obj"
#define BAD_FILE_NAME_2 "tests/cube_bad_2.obj"
#define EPS 1E-05

using namespace s21;

struct ModelTest : public testing::Test {
  Model *model;
  void SetUp() { model = new Model(); }
  void TearDown() { delete model; }
};

TEST_F(ModelTest, ValidateFile) { ASSERT_FALSE(model->Validate(FILE_NAME)); }

TEST_F(ModelTest, ValidateBadFile1) {
  ASSERT_EQ(model->Validate(BAD_FILE_NAME_1), 1);
}

TEST_F(ModelTest, ValidateBadFile2) {
  ASSERT_EQ(model->Validate(BAD_FILE_NAME_2), 2);
}

TEST_F(ModelTest, ValidateNoExFile) {
  ASSERT_EQ(model->Validate("File doesn't exist"), 3);
}

TEST_F(ModelTest, ParserVertex) {
  double p_vertex[24];
  p_vertex[0] = 1;
  p_vertex[1] = -1;
  p_vertex[2] = -1;
  p_vertex[3] = 1;
  p_vertex[4] = -1;
  p_vertex[5] = 1;
  p_vertex[6] = -1;
  p_vertex[7] = -1;
  p_vertex[8] = 1;
  p_vertex[9] = -1;
  p_vertex[10] = -1;
  p_vertex[11] = -1;
  p_vertex[12] = 1;
  p_vertex[13] = 1;
  p_vertex[14] = -1;
  p_vertex[15] = 1;
  p_vertex[16] = 1;
  p_vertex[17] = 1;
  p_vertex[18] = -1;
  p_vertex[19] = 1;
  p_vertex[20] = 1;
  p_vertex[21] = -1;
  p_vertex[22] = 1;
  p_vertex[23] = -1;

  model->Parser(FILE_NAME);
  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
  model->RecalcModel();
  model->FreeModel();
}

TEST_F(ModelTest, ParserFacet) {
  int p_facet[72];
  p_facet[0] = 1;
  p_facet[1] = 2;
  p_facet[2] = 2;
  p_facet[3] = 3;
  p_facet[4] = 3;
  p_facet[5] = 1;
  p_facet[6] = 7;
  p_facet[7] = 6;
  p_facet[8] = 6;
  p_facet[9] = 5;
  p_facet[10] = 5;
  p_facet[11] = 7;
  p_facet[12] = 4;
  p_facet[13] = 5;
  p_facet[14] = 5;
  p_facet[15] = 1;
  p_facet[16] = 1;
  p_facet[17] = 4;
  p_facet[18] = 5;
  p_facet[19] = 6;
  p_facet[20] = 6;
  p_facet[21] = 2;
  p_facet[22] = 2;
  p_facet[23] = 5;
  p_facet[24] = 2;
  p_facet[25] = 6;
  p_facet[26] = 6;
  p_facet[27] = 7;
  p_facet[28] = 7;
  p_facet[29] = 2;
  p_facet[30] = 0;
  p_facet[31] = 3;
  p_facet[32] = 3;
  p_facet[33] = 7;
  p_facet[34] = 7;
  p_facet[35] = 0;
  p_facet[36] = 0;
  p_facet[37] = 1;
  p_facet[38] = 1;
  p_facet[39] = 3;
  p_facet[40] = 3;
  p_facet[41] = 0;
  p_facet[42] = 4;
  p_facet[43] = 7;
  p_facet[44] = 7;
  p_facet[45] = 5;
  p_facet[46] = 5;
  p_facet[47] = 4;
  p_facet[48] = 0;
  p_facet[49] = 4;
  p_facet[50] = 4;
  p_facet[51] = 1;
  p_facet[52] = 1;
  p_facet[53] = 0;
  p_facet[54] = 1;
  p_facet[55] = 5;
  p_facet[56] = 5;
  p_facet[57] = 2;
  p_facet[58] = 2;
  p_facet[59] = 1;
  p_facet[60] = 3;
  p_facet[61] = 2;
  p_facet[62] = 2;
  p_facet[63] = 7;
  p_facet[64] = 7;
  p_facet[65] = 3;
  p_facet[66] = 4;
  p_facet[67] = 0;
  p_facet[68] = 0;
  p_facet[69] = 7;
  p_facet[70] = 7;
  p_facet[71] = 4;

  model->Parser(FILE_NAME);

  for (size_t i = 0; i < 72; i++) {
    ASSERT_EQ(model->GetFacetArray()[i], p_facet[i]);
  }

  model->RecalcModel();
  model->FreeModel();
}

TEST_F(ModelTest, ParserVertexAmount) {
  model->Parser(FILE_NAME);
  ASSERT_EQ(model->GetVertexAmount(), 8);
}

TEST_F(ModelTest, ParserFacetAmount) {
  model->Parser(FILE_NAME);
  ASSERT_EQ(model->GetFacetAmount(), 72);
}

TEST_F(ModelTest, Scaling) {
  double p_vertex[24];
  p_vertex[0] = 3;
  p_vertex[1] = -3;
  p_vertex[2] = -3;
  p_vertex[3] = 3;
  p_vertex[4] = -3;
  p_vertex[5] = 3;
  p_vertex[6] = -3;
  p_vertex[7] = -3;
  p_vertex[8] = 3;
  p_vertex[9] = -3;
  p_vertex[10] = -3;
  p_vertex[11] = -3;
  p_vertex[12] = 3;
  p_vertex[13] = 3;
  p_vertex[14] = -3;
  p_vertex[15] = 3;
  p_vertex[16] = 3;
  p_vertex[17] = 3;
  p_vertex[18] = -3;
  p_vertex[19] = 3;
  p_vertex[20] = 3;
  p_vertex[21] = -3;
  p_vertex[22] = 3;
  p_vertex[23] = -3;

  model->Parser(FILE_NAME);
  model->Scaling(3);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

TEST_F(ModelTest, RotateOx) {
  double p_vertex[24];
  p_vertex[0] = 1;
  p_vertex[1] = -0.964491;
  p_vertex[2] = -1.034290;
  p_vertex[3] = 1;
  p_vertex[4] = -1.034290;
  p_vertex[5] = 0.964491;
  p_vertex[6] = -1;
  p_vertex[7] = -1.034290;
  p_vertex[8] = 0.964491;
  p_vertex[9] = -1;
  p_vertex[10] = -0.964491;
  p_vertex[11] = -1.034290;
  p_vertex[12] = 1;
  p_vertex[13] = 1.034290;
  p_vertex[14] = -0.964490;
  p_vertex[15] = 0.999999;
  p_vertex[16] = 0.964491;
  p_vertex[17] = 1.034291;
  p_vertex[18] = -1;
  p_vertex[19] = 0.964491;
  p_vertex[20] = 1.034290;
  p_vertex[21] = -1;
  p_vertex[22] = 1.034290;
  p_vertex[23] = -0.964491;

  model->Parser(FILE_NAME);
  model->Rotate(OX_ROTATE, 2 * M_PI / 180);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }

  model->Return(p_vertex, model->GetXCenter(), model->GetYCenter(),
                model->GetZCenter());
}

TEST_F(ModelTest, RotateOy) {
  double p_vertex[24];
  p_vertex[0] = 1.034290;
  p_vertex[1] = -1;
  p_vertex[2] = -0.964491;
  p_vertex[3] = 0.964491;
  p_vertex[4] = -1;
  p_vertex[5] = 1.034290;
  p_vertex[6] = -1.034290;
  p_vertex[7] = -1;
  p_vertex[8] = 0.964491;
  p_vertex[9] = -0.964491;
  p_vertex[10] = -1;
  p_vertex[11] = -1.034290;
  p_vertex[12] = 1.034290;
  p_vertex[13] = 1;
  p_vertex[14] = -0.964490;
  p_vertex[15] = 0.964490;
  p_vertex[16] = 1;
  p_vertex[17] = 1.034291;
  p_vertex[18] = -1.034290;
  p_vertex[19] = 1;
  p_vertex[20] = 0.964491;
  p_vertex[21] = -0.964491;
  p_vertex[22] = 1;
  p_vertex[23] = -1.034290;

  model->Parser(FILE_NAME);
  model->Rotate(OY_ROTATE, 2 * M_PI / 180);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

TEST_F(ModelTest, RotateOz) {
  double p_vertex[24];
  p_vertex[0] = 1.034290;
  p_vertex[1] = -0.964491;
  p_vertex[2] = -1;
  p_vertex[3] = 1.034290;
  p_vertex[4] = -0.964491;
  p_vertex[5] = 1;
  p_vertex[6] = -0.964491;
  p_vertex[7] = -1.034290;
  p_vertex[8] = 1;
  p_vertex[9] = -0.964491;
  p_vertex[10] = -1.034290;
  p_vertex[11] = -1;
  p_vertex[12] = 0.964491;
  p_vertex[13] = 1.034290;
  p_vertex[14] = -0.999999;
  p_vertex[15] = 0.964490;
  p_vertex[16] = 1.034290;
  p_vertex[17] = 1.000001;
  p_vertex[18] = -1.034290;
  p_vertex[19] = 0.964491;
  p_vertex[20] = 1;
  p_vertex[21] = -1.034290;
  p_vertex[22] = 0.964491;
  p_vertex[23] = -1;

  model->Parser(FILE_NAME);
  model->Rotate(OZ_ROTATE, 2 * M_PI / 180);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

TEST_F(ModelTest, MoveOx) {
  double p_vertex[24];
  p_vertex[0] = 11;
  p_vertex[1] = -1;
  p_vertex[2] = -1;
  p_vertex[3] = 11;
  p_vertex[4] = -1;
  p_vertex[5] = 1;
  p_vertex[6] = 9;
  p_vertex[7] = -1;
  p_vertex[8] = 1;
  p_vertex[9] = 9;
  p_vertex[10] = -1;
  p_vertex[11] = -1;
  p_vertex[12] = 11;
  p_vertex[13] = 1;
  p_vertex[14] = -1;
  p_vertex[15] = 11;
  p_vertex[16] = 1;
  p_vertex[17] = 1;
  p_vertex[18] = 9;
  p_vertex[19] = 1;
  p_vertex[20] = 1;
  p_vertex[21] = 9;
  p_vertex[22] = 1;
  p_vertex[23] = -1;

  model->Parser(FILE_NAME);
  model->Move(10, 0, 0);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

TEST_F(ModelTest, MoveOy) {
  double p_vertex[24];
  p_vertex[0] = 1;
  p_vertex[1] = 9;
  p_vertex[2] = -1;
  p_vertex[3] = 1;
  p_vertex[4] = 9;
  p_vertex[5] = 1;
  p_vertex[6] = -1;
  p_vertex[7] = 9;
  p_vertex[8] = 1;
  p_vertex[9] = -1;
  p_vertex[10] = 9;
  p_vertex[11] = -1;
  p_vertex[12] = 1;
  p_vertex[13] = 11;
  p_vertex[14] = -1;
  p_vertex[15] = 1;
  p_vertex[16] = 11;
  p_vertex[17] = 1;
  p_vertex[18] = -1;
  p_vertex[19] = 11;
  p_vertex[20] = 1;
  p_vertex[21] = -1;
  p_vertex[22] = 11;
  p_vertex[23] = -1;

  model->Parser(FILE_NAME);
  model->Move(0, 10, 0);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

TEST_F(ModelTest, MoveOz) {
  double p_vertex[24];
  p_vertex[0] = 1;
  p_vertex[1] = -1;
  p_vertex[2] = 9;
  p_vertex[3] = 1;
  p_vertex[4] = -1;
  p_vertex[5] = 11;
  p_vertex[6] = -1;
  p_vertex[7] = -1;
  p_vertex[8] = 11;
  p_vertex[9] = -1;
  p_vertex[10] = -1;
  p_vertex[11] = 9;
  p_vertex[12] = 1;
  p_vertex[13] = 1;
  p_vertex[14] = 9;
  p_vertex[15] = 1;
  p_vertex[16] = 1;
  p_vertex[17] = 11;
  p_vertex[18] = -1;
  p_vertex[19] = 1;
  p_vertex[20] = 11;
  p_vertex[21] = -1;
  p_vertex[22] = 1;
  p_vertex[23] = 9;

  model->Parser(FILE_NAME);
  model->Move(0, 0, 10);

  for (size_t i = 0; i < 24; i++) {
    ASSERT_NEAR(model->GetVertexArray()[i], p_vertex[i], EPS);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
