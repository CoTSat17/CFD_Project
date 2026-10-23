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
		for (int j = 0; j < new_mat_cols; j++) {
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




float matrix_det(Matrix* matrix) {
	if (matrix->n_col == 2) {
		return matrix_det2(matrix);
	}
	if (matrix->n_col == 3) {
		return matrix_det3(matrix);
	}

}



float matrix_det2(Matrix* matrix) {
	return matrix_value(matrix, 0, 0) * matrix_value(matrix, 1, 1) - matrix_value(matrix, 0, 1) * matrix_value(matrix, 1, 0);
}







#define S3 0.57735f

/// -------------
/// Tri dN DATA
/// -------------
static const float tri_w_values[] = { 1.0/2.0 };

/// -------------
/// TET4 dN DATA
/// -------------
static const float tet_w_values[] = { 1.0 / 6.0 };

/// -------------
/// QUAD dN DATA
/// -------------

static const float quad_xi_values[]	 =  { -S3, S3, S3,-S3 }; 
static const float quad_eta_values[] =  { -S3,-S3, S3, S3 };
static const float quad_w_values[]   =  { 1,  1,  1,  1 };

static const float quad_dN_values[]     = {-1,-1, 1,-1, 1, 1,-1, 1 };
static const float quad_xi_positions[]  = { 1, 0,-1, 0, 1, 0,-1, 0 };
static const float quad_eta_positions[] = { 0, 1, 0,-1, 0, 1, 0,-1 };


/// -------------
/// HEX dN DATA
/// -------------

static const float hex_xi_values[] =   { -S3, S3, S3,-S3,-S3, S3, S3,-S3 };
static const float hex_eta_values[] =  { -S3,-S3, S3, S3,-S3,-S3, S3, S3 };
static const float hex_zeta_values[] = { -S3,-S3,-S3,-S3, S3, S3, S3, S3 };
static const float hex_w_values[] =   { 1, 1, 1, 1, 1, 1, 1, 1 };

static const float hex_dN_values[] =    {	-1,-1,-1,
											 1,-1,-1,
											 1, 1,-1,
											-1, 1,-1,
											-1,-1, 1,
											 1,-1, 1,
											 1, 1, 1,
											-1, 1, 1};

static const float hex_xi_positions[] = {	 0,-1,-1,
											 0, 1, 1,
											 0, 1, 1,
											 0,-1,-1,
											 0,-1,-1,
											 0, 1, 1,
											 0, 1, 1,
											 0,-1,-1 };


static const float hex_eta_positions[] = {	-1, 0,-1,
											-1, 0,-1,
											 1, 0, 1,
											 1, 0, 1,
											-1, 0,-1,
											-1, 0,-1,
											 1, 0, 1,
											 1, 0, 1 };


static const float hex_zeta_positions[] = { -1,-1, 0,
											-1,-1, 0,
											-1,-1, 0,
											-1,-1, 0,
											 1, 1, 0,
											 1, 1, 0,
											 1, 1, 0,
											 1, 1, 0 };


static Matrix** calc_dNg_matrix(int* n_matrix, Gauss_Data_Types type) {
	switch (type)  {

	case QUAD4: {
		// For QUAD4 a total of 4 points are needed
		*n_matrix = 4;

		// Allocate space for the 4 matixes
		Matrix** matrix_array = (Matrix**)malloc(*n_matrix * sizeof(Matrix*));

		for (int g = 0; g < *n_matrix; g++) {
			int n_values = sizeof(quad_dN_values) / sizeof(quad_dN_values[0]);

			float* values = (float*)malloc(sizeof(float) * n_values);

			float xi_value = quad_xi_values[g];
			float eta_value = quad_eta_values[g];

			for (int i = 0; i < n_values; i++) {
				values[i] = 0.25 * (quad_dN_values[i] + xi_value * quad_xi_positions[i] + eta_value * quad_eta_positions[i]);
			}

			matrix_array[g] = matrix_create(values, 2, 4);
		}

		return matrix_array;
	}
	

	case HEX8: {
		// For HEX8 a total of 8 points are needed
		*n_matrix = 8;

		// Allocate space for the 8 matixes
		Matrix** matrix_array = (Matrix**)malloc(*n_matrix * sizeof(Matrix*));

		for (int g = 0; g < *n_matrix; g++) {
			int n_values = sizeof(hex_dN_values) / sizeof(hex_dN_values[0]);

			float* values = (float*)malloc(sizeof(float) * n_values);

			float xi_value = hex_xi_values[g];
			float eta_value = hex_eta_values[g];
			float zeta_value = hex_zeta_values[g];

			for (int i = 0; i < n_values; i++) {
				values[i] = 0.125* hex_dN_values[i] * ((1 + xi_value * hex_xi_positions[i]) * (1 + eta_value * hex_eta_positions[i]) * (1 + zeta_value * hex_zeta_positions[i]));
			}

			matrix_array[g] = matrix_create(values, 3, 8);
		}

		return matrix_array;
	}
	
	case TRI3: {
		// For TRI3 a total of 1 point is needed
		*n_matrix = 1;
		// Allocate space for the 1 matixes
		Matrix** matrix_array = (Matrix**)malloc(*n_matrix * sizeof(Matrix*));

		float values[] = { -1,-1, 1, 0, 0, 1 };


		matrix_array[0] = matrix_create(values, 2, 3);


		return matrix_array;
	}
	case TET4: {
		// For TET4 a total of 1 point is needed
		*n_matrix = 1;
		// Allocate space for the 1 matixes
		Matrix** matrix_array = (Matrix**)malloc(*n_matrix * sizeof(Matrix*));

		float values[] = { -1,-1,-1,	 1, 0, 0,		 0, 1, 0,	0, 0, 1 };


		matrix_array[0] = matrix_create(values, 3, 4);


		return matrix_array;
	}
	}
}





float calc_volume(Matrix* positions, Gauss_Data_Types type) {

	float result = 0;

	static int n_matrix = 0;
	static Matrix** dN_g;
	static int initialized = 0;
	static Gauss_Data_Types previous_type;

	if (!initialized || previous_type != type) {
		dN_g = calc_dNg_matrix(&n_matrix, type);
		initialized = 1;
		previous_type = type;
	}


	for (int g = 0; g < n_matrix; g++) {

		Matrix* positions_transpose = matrix_transpose(positions);
		Matrix* jacobian = matrix_product(positions_transpose, dN_g[g]);
		float jacobian_det = matrix_det(jacobian);
		if (jacobian_det < 0.0) jacobian_det = -jacobian_det;
		free(positions_transpose);
		free(jacobian);


		switch (type) {
			
			case QUAD4: 
				result += quad_w_values[g] * jacobian_det;
				break;
			case HEX8:
				result += hex_w_values[g] * jacobian_det;
				break;
			case TRI3:
				result += tri_w_values[g] * jacobian_det;
				break;
			case TET4:
				result += tet_w_values[g] * jacobian_det;
				break;
		}



	}

	return result;
}