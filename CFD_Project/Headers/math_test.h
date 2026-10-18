#pragma once
#include <stdlib.h>



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


float matrix_det3(Matrix* matrix);
float matrix_det2(Matrix* matrix);