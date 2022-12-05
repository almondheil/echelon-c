#ifndef __AUTOMATIC_H__
#define __AUTOMATIC_H__

#include <stdbool.h>

/* preconditions: nrows and ncols are the dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns exit status--if true, in echelon form. */
bool auto_echelon (int nrows, int ncols, double matrix[nrows][ncols]);

/* preconditions: nrows and ncols are the dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns exit status--if true, in reduced echelon form */
bool auto_reduced_echelon (int nrows, int ncols, double matrix[nrows][ncols]);

#endif
