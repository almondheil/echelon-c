#ifndef __MATRIX_PROC_H__
#define __MATRIX_PROC_H__

/* function prototypes, listed in alphabetical order */

/* preconditions: row1 and row2 are rows in the matrix
 *                scalar is a floating point value
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: row1 = (row1 + (scalar * row2)) */
void add_scaled (int row1, int row2, 
		double scalar, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

/* preconditions: row is a row in the matrix
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: returns the index of the first nonzero entry in row
 *                (-1 if no leading entry exists) */
int leading_pos (int row, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

/* preconditions: row is a row in the matrix
 *                scalar is a nonzero double
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: row = (scalar * row) */
void scale_row (int row, 
		double scalar, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

/* preconditions: row1 and row2 are rows in the matrix
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: row1 and row2 swap all their values */
void swap_rows (int row1, int row2, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

#endif
