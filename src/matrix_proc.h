#ifndef __MATRIX_PROC_H__
#define __MATRIX_PROC_H__

/* Add a scaled version of row2 to row1, modifying the values in row1.
 *
 * pre:  matrix is initialized
 *       row1 and row2 are rows within matrix (0 <= x < nrows)
 * post: modifies matrix such that row1 += (scalar * row2)
 */
void add_scaled (int row1, int row2, double scalar,
		int nrows, int ncols, double matrix[nrows][ncols]);

/* Return the location of the leading entry of a row.
 *
 * pre:  matrix is initialized
 *       row is a valid row (0 <= row <= nrows)
 * post: returns the index of the leading entry of the row 
 *       (0 <= index <= ncols), or -1 if there is no leading entry
 */
int leading_pos (int row, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

/* pre: row is a row in the matrix
 *                scalar is a nonzero double
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * post: row = (scalar * row) */
void scale_row (int row, 
		double scalar, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

/* pre: row1 and row2 are rows in the matrix
 *                nrows and ncols are the dimensions of the matrix
 *                matrix is an array of doubles
 * post: row1 and row2 swap all their values */
void swap_rows (int row1, int row2, 
		int nrows, int ncols, 
		double matrix[nrows][ncols]);

#endif
