#include <math_test.h>



Matrix* matrix_create(float values[], int n_col, int n_row) {

	/// ---------------
	/// ALLOCATE MEMORY
	/// ---------------
	/// Allocate memory for: Matrix struct + values array
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix) + n_col * n_row * sizeof(float));
	if (matrix == NULL) return NULL;

	// ---------------
	// SAVE BASIC DATA
	// ---------------
	matrix->n_col = n_col;
	matrix->n_row = n_row;
	matrix->values = (float*)(matrix + 1);

	/// ------------------
	/// Save matrix values
	/// ------------------
	float* next_value = matrix->values;


	for (int i = 0; i < n_row * n_col; i++) { 
		*next_value = values[i];
		next_value++;
	}

	return matrix;
}



float matrix_value(Matrix* matrix, int row, int col) {
	return matrix->values[row * matrix->n_col + col];
}







Matrix* matrix_product(Matrix* matrix_1, Matrix* matrix_2) {
	/// ------------------------
	/// CHECK MATRIX DIMMENSIONS
	/// ------------------------
	/// Check that multiplication is possible:
	///		n_cols mat_1 == n_rows mat_2
	
	if (matrix_1->n_col != matrix_2->n_row) return NULL;

	/// ---------------------------
	/// Create array for the results
	/// ---------------------------

	int new_mat_rows = matrix_1->n_row;
	int new_mat_cols = matrix_2->n_col;
	float* new_values = (float*)malloc(new_mat_rows * new_mat_cols * sizeof(float));


	/// -------------------
	/// Calculate new values
	/// -------------------

	float value;
	for (int i = 0; i < new_mat_rows; i++) {
		for (int j = 0; j < new_mat_rows; j++) {
			value = 0;

			for (int k = 0; k < matrix_1->n_col; k++) {
				value += matrix_value(matrix_1,i, k) * matrix_value(matrix_2, k, j);
				
			}
			new_values[new_mat_cols * i + j] = value;

		}
	}

	Matrix* new_mat = matrix_create(new_values, new_mat_cols, new_mat_rows);
	free(new_values);

	return new_mat;

}


Matrix* matrix_transpose(Matrix* matrix) {
	int new_matrix_row = matrix->n_col;
	int new_matrix_col = matrix->n_row;

	float* values = (float*)malloc(sizeof(float) * matrix->n_row * matrix->n_col);

	/// ---------------------
	/// GET TRANSPOSED VALUES
	/// ---------------------

	for (int i = 0; i < new_matrix_row; i++) {
		for (int j = 0; j < new_matrix_col; j++) {

			values[i * new_matrix_col + j] = matrix_value(matrix, j, i);

		}
	}

	/// -----------------------------
	/// CREATE NEW MATRIX WITH VALUES
	/// -----------------------------
	
	Matrix* new_mat = matrix_create(values, new_matrix_col, new_matrix_row);
	free(values);

	return new_mat;


}





float matrix_det3(Matrix* matrix) {

	float side_1 =	  matrix_value(matrix, 0, 0) * matrix_value(matrix, 1, 1) * matrix_value(matrix, 2, 2)
					+ matrix_value(matrix, 1, 0) * matrix_value(matrix, 2, 1) * matrix_value(matrix, 0, 2)
					+ matrix_value(matrix, 0, 1) * matrix_value(matrix, 1, 2) * matrix_value(matrix, 2, 0);

	float side_2 =	  matrix_value(matrix, 0, 2) * matrix_value(matrix, 1, 1) * matrix_value(matrix, 2, 0)
					+ matrix_value(matrix, 0, 1) * matrix_value(matrix, 1, 2) * matrix_value(matrix, 2, 0)
					+ matrix_value(matrix, 0, 0) * matrix_value(matrix, 1, 2) * matrix_value(matrix, 2, 1);

	return side_1 - side_2;
}




float matrix_det2(Matrix* matrix) {
	return matrix_value(matrix, 0, 0) * matrix_value(matrix, 1, 1) - matrix_value(matrix, 0, 1) * matrix_value(matrix, 1, 0);
}