
#include <pch.h>

#include "gtest/gtest.h"
extern "C" {
#include <math_test.h>
}


TEST(Math_Test, Matrix_Create) {

	float values[] = {	1.0,	2.0,	3.0,
						2.0,	3.0,	1.0 };

	Matrix* matrix = matrix_create(values, 3, 2);

	EXPECT_EQ(matrix->n_col, 3);
	EXPECT_EQ(matrix->n_row, 2);
	EXPECT_FLOAT_EQ(matrix->values[0], 1.0);
	EXPECT_FLOAT_EQ(matrix->values[1], 2.0);
}



TEST(Math_Test, Matrix_Product) {

	float values_1[] = {	1.0,	2.0,	3.0,
							2.0,	3.0,	1.0 };

	float values_2[] = {	1.0,	2.0,
							1.0,	2.0,
							4.0,	1.0};


	Matrix* matrix_1 = matrix_create(values_1, 3, 2);
	Matrix* matrix_2 = matrix_create(values_2, 2, 3);

	Matrix* matrix_3 = matrix_product(matrix_1, matrix_2);


	EXPECT_FLOAT_EQ(matrix_3->n_col, 2);
	EXPECT_FLOAT_EQ(matrix_3->n_row, 2);
	EXPECT_FLOAT_EQ(matrix_3->values[0], 15);
	EXPECT_FLOAT_EQ(matrix_3->values[1], 9);
	EXPECT_FLOAT_EQ(matrix_3->values[2], 9);
	EXPECT_FLOAT_EQ(matrix_3->values[3], 11);
}




TEST(Math_Test, Matrix_Transpose) {

	float values[] = { 1.0,	2.0,	3.0,
						2.0,	3.0,	1.0 };

	Matrix* matrix = matrix_create(values, 3, 2);


	Matrix* transposed_mat = matrix_transpose(matrix);

	EXPECT_EQ(transposed_mat->n_col, 2);
	EXPECT_EQ(transposed_mat->n_row, 3);
	EXPECT_FLOAT_EQ( matrix_value(transposed_mat, 0, 0), 1.0);
	EXPECT_FLOAT_EQ( matrix_value(transposed_mat, 0, 1), 2.0);
	EXPECT_FLOAT_EQ( matrix_value(transposed_mat, 2, 0), 3.0);
}



TEST(Math_Test, Matrix_Mat3) {

	float values[] = {	1.0,	2.0,	3.0,
						2.0,	3.0,	1.0,
						2.0,	3.0,	1.0};


	Matrix* matrix = matrix_create(values, 3, 3);


	float det = matrix_det3(matrix);

	EXPECT_FLOAT_EQ(det, 0.0);
}

TEST(Math_Test, Matrix_Mat2) {

	float values[] = {	1.0,	2.0,	
						2.0,	3.0};


	Matrix* matrix = matrix_create(values, 2, 2);


	float det = matrix_det2(matrix);

	EXPECT_FLOAT_EQ(det, -1.0);
}