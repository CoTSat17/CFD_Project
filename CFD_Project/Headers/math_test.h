#pragma once
#include <stdlib.h>
#include <math.h>




typedef struct Matrix {
	int n_col;
	int n_row;
	float* values;
}Matrix;

/// <summary>
/// Creates a Matrix struct
/// </summary>
/// <param name="values">Array with float values</param>
/// <param name="n_col">Number of columns</param>
/// <param name="n_row">Number of rows</param>
/// <returns>Pointer to the new Matrix</returns>
Matrix* matrix_create(float values[], int n_col, int n_row);


/// <summary>
/// Computes the product of two matrices (matrix_1 * matrix_2).
/// </summary>
/// <param name="matrix_1">Pointer to the left-hand operand matrix. Its column count must match matrix_2's row count.</param>
/// <param name="matrix_2">Pointer to the right-hand operand matrix. Its row count must match matrix_1's column count.</param>
/// <returns>Pointer to a Matrix containing the product, or nullptr on failure or if inputs are incompatible.</returns>
Matrix* matrix_product(Matrix* matrix_1, Matrix* matrix_2);

/// <summary>
/// Returns the value at the specified position in a matrix.
/// </summary>
/// <param name="matrix">Pointer to the Matrix from which to retrieve the value.</param>
/// <param name="row">Row index of the element to retrieve.</param>
/// <param name="col">Column index of the element to retrieve.</param>
/// <returns>The float value stored at the specified row and column. Behavior is undefined if matrix is null or indices are out of range.</returns>
float matrix_value(Matrix* matrix, int row, int col);

/// <summary>
/// 
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix* matrix_transpose(Matrix* matrix);


float matrix_det(Matrix* matrix);
float matrix_det3(Matrix* matrix);
float matrix_det2(Matrix* matrix);




typedef enum Gauss_Data_Types {
	TRI3,
	QUAD4,
	TET4,
	HEX8
}Gauss_Data_Types;

/// <summary>
/// Calculates and returns an array of pointers to Matrix objects based on the provided matrix count and Gauss data type.
/// </summary>
/// <param name="n_matrix">Pointer to an integer specifying the number of matrices to calculate.</param>
/// <param name="type">The Gauss_Data_Types value indicating the type of Gauss data to use in the calculation.</param>
/// <returns>A pointer to an array of Matrix pointers containing the calculated matrices.</returns>
static Matrix** calc_dNg_matrix(int* n_matrix, Gauss_Data_Types type);

/// <summary>
/// Calculates the volume based on the provided positions and Gaussian data type.
/// </summary>
/// <param name="positions">Pointer to a Matrix containing position data. [x,y,z, x,y,z, ...] (3D) // [x,y, x,y, ...] (2D) </param>
/// <param name="type">The Gaussian data type used for the calculation.</param>
/// <returns>The calculated volume as a float.</returns>
float calc_volume(Matrix* positions, Gauss_Data_Types type);




/// <summary>
/// Calculates the area based on the given positions and Gaussian data type.
/// </summary>
/// <param name="positions">Pointer to a Matrix containing the positions used for area calculation.</param>
/// <param name="type">The Gaussian data type specifying the calculation method.</param>
/// <returns>The calculated area as a float.</returns>
float calc_area(Matrix* positions, Gauss_Data_Types type);

