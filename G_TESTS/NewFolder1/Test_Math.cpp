
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




TEST(Math_Test, Calc_Volume) {

	// QUAD4

	float points_QUAD4[] = { 0.0, 0.0, 4.0, 0.0, 3.0, 2.0, 1.0, 2.0 };

	Matrix* points_matrix = matrix_create(points_QUAD4, 2, 4);

	float volume_QUAD4 = calc_volume(points_matrix, QUAD4);

	EXPECT_FLOAT_EQ(volume_QUAD4, 6.0);
	free(points_matrix);


	// HEX8
	float points_HEX8[] = {  0,0,0,  1,0,0,  1,1,0,  0,1,0,
							 0,0,1,  1,0,1,  1,1,1,  0,1,1 };

	points_matrix = matrix_create(points_HEX8, 3, 8);

	float volume_HEX8 = calc_volume(points_matrix, HEX8);

	EXPECT_FLOAT_EQ(volume_HEX8, 1.0);

	// TRI3
	float points_TRI3[] = { 0,0,  1,0,  0,1 };

	points_matrix = matrix_create(points_TRI3, 2, 3);

	float volume_TRI3 = calc_volume(points_matrix, TRI3);

	EXPECT_FLOAT_EQ(volume_TRI3, 0.5);

	// TET4
	float points_TET4[] = {		0,0,0,		1,0,0,		0,1,0,		0,0,1};
	points_matrix = matrix_create(points_TET4, 3, 4);

	float volume_TET4 = calc_volume(points_matrix, TET4);

	EXPECT_FLOAT_EQ(volume_TET4, 0.16666667);
}



TEST(Math_Test, Calc_Area) {
	float points[] = {  0,0,0,  1,0,0,  1,1,0,  0,1,0};

	Matrix* points_matrix = matrix_create(points, 3, 4);

	float area = calc_area(points_matrix, HEX8);

	EXPECT_FLOAT_EQ(area, 1.0);
}