#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	size_t size_row;
	size_t size_column;
	double** matrix;
} matrix_t;

void print_matrix(const matrix_t* matrix)
{
	if (matrix == NULL)
		exit(-2);

	for (size_t i = 0; i < matrix->size_row; i++)
	{
		for (size_t j = 0; j < matrix->size_column; j++)
		{
			printf("%f ", matrix->matrix[i][j]);
		}
		printf("\n");
	}
}

matrix_t* init_matrix(const size_t size_row, const size_t size_column)
{
	matrix_t* return_matrix = (matrix_t*)malloc(sizeof(matrix_t));
	double** matrix_value_tmp = (double**)calloc(sizeof(double*), size_row);
	for (size_t i = 0; i < size_row; i++)
		matrix_value_tmp[i] = (double*)calloc(sizeof(double), size_column);

	return_matrix->size_row = size_row;
	return_matrix->size_column = size_column;
	return_matrix->matrix = matrix_value_tmp;

	return return_matrix;
}

void free_matrix(matrix_t* matrix)
{
	if (matrix != NULL)
	{
		free(matrix->matrix);
		free(matrix);
	}
}

double determinant_matrix(const matrix_t* matrix)
{
	if (matrix->size_row != matrix->size_column)
		return (double)INT32_MAX;

	if (matrix->size_row == 2)
	{
		return (matrix->matrix[0][0] * matrix->matrix[1][1]) - (matrix->matrix[0][1] * matrix->matrix[1][0]);
	}
	else
	{
		double determinant = 0;
		matrix_t* matrix_tmp = init_matrix(matrix->size_row - 1, matrix->size_column - 1);
		for (size_t i_top_matrix = 0; i_top_matrix < matrix->size_row; i_top_matrix++)
		{
			size_t row_tmp = 0, column_tmp = 0;
			for (size_t row = 0; row < matrix->size_row; row++)
			{
				if (row == 0) continue;
				for (size_t column = 0; column < matrix->size_column; column++)
				{
					if (column != i_top_matrix)
					{
						if (column_tmp == matrix_tmp->size_row)
						{
							row_tmp++;
							column_tmp = 0;
						}
						matrix_tmp->matrix[row_tmp][column_tmp++] = matrix->matrix[row][column];
					}
				}
			}
			determinant +=
				matrix->matrix[0][i_top_matrix] * determinant_matrix(matrix_tmp) * pow(-1, (double)i_top_matrix);
		}
		free_matrix(matrix_tmp);
		return determinant;
	}
}

// TODO: make for 4x4 matrix and bigger
double minor_matrix(const matrix_t* matrix, const size_t row, const size_t column)
{
	if (matrix->size_row != matrix->size_column || matrix->size_row > 3)
		return (double)INT32_MAX;

	matrix_t* matrix_return = init_matrix(matrix->size_row - 1, matrix->size_column - 1);
	size_t row_tmp = 0, column_tmp = 0;
	for (size_t matrix_row = 0; matrix_row < matrix->size_row; matrix_row++)
	{
		for (size_t matrix_column = 0; matrix_column < matrix->size_column; matrix_column++)
		{
			if (matrix_row != row && matrix_column != column)
			{
				if (column_tmp == matrix_return->size_row)
				{
					row_tmp++;
					column_tmp = 0;
				}
				matrix_return->matrix[row_tmp][column_tmp++] = matrix->matrix[matrix_row][matrix_column];
			}
		}
	}

	double result = determinant_matrix(matrix_return);

	free_matrix(matrix_return);

	return result;
}

matrix_t* transpose_matrix(const matrix_t* matrix)
{
	if (matrix == NULL)
		return NULL;

	matrix_t* return_matrix = init_matrix(matrix->size_column, matrix->size_row);

	for (size_t x = 0; x < return_matrix->size_row; x++)
	{
		for (size_t y = 0; y < return_matrix->size_column; y++)
			return_matrix->matrix[x][y] = matrix->matrix[y][x];
	}
	return return_matrix;
}

matrix_t* multiply_matrix(const matrix_t* matrix_a, const matrix_t* matrix_b)
{
	matrix_t* result_matrix = init_matrix(matrix_a->size_row, matrix_b->size_column);
	matrix_t* transpose_matrix_b = transpose_matrix(matrix_b);

	for (size_t row_matrix_a = 0; row_matrix_a < matrix_a->size_row; row_matrix_a++)
	{
		for (size_t row_T_matrix_b = 0; row_T_matrix_b < transpose_matrix_b->size_row; row_T_matrix_b++)
		{
			double tmp_sum = 0;
			for (size_t i = 0; i < transpose_matrix_b->size_column; i++)
			{
				tmp_sum += matrix_a->matrix[row_matrix_a][i] * transpose_matrix_b->matrix[row_T_matrix_b][i];
			}
			result_matrix->matrix[row_matrix_a][row_T_matrix_b] = tmp_sum;
		}
	}

	free(transpose_matrix_b);

	return result_matrix;
}

void multiply_by_value_matrix(const double number, matrix_t* matrix)
{
	for (size_t row = 0; row < matrix->size_row; row++)
	{
		for (size_t column = 0; column < matrix->size_column; column++)
		{
			matrix->matrix[row][column] *= number;
		}
	}
}

matrix_t* inverse_matrix(const matrix_t* matrix)
{
	const double coficient = 1 / determinant_matrix(matrix);
	matrix_t* adj_matrix = init_matrix(matrix->size_row, matrix->size_column);

	for (size_t row = 0; row < adj_matrix->size_row; row++)
	{
		for (size_t column = 0; column < adj_matrix->size_column; column++)
		{
			adj_matrix->matrix[row][column] =
				pow(-1, (double)row + (double)column) * minor_matrix(matrix, row, column);
		}
	}

	adj_matrix = transpose_matrix(adj_matrix);
	multiply_by_value_matrix(coficient, adj_matrix);

	return adj_matrix;
}

matrix_t* solve_equation(const matrix_t* X, const matrix_t* B)
{
	return multiply_matrix(inverse_matrix(X), B);
}