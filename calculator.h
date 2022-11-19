#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

/* preconditions: nrows and ncols are the dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns exit status--if true, in echelon form. */
bool echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);

/* preconditions: nrows and ncols are the dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns exit status--if true, in reduced echelon form */
bool reduced_echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);

#endif
