#ifndef __USER_IO_H__
#define __USER_IO_H__

#include <stdbool.h>

/* preconditions: nrows and ncols are dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: matrix will be filled with values from stdin */
bool read_matrix_stdin (int nrows, int ncols,
		       double matrix[nrows][ncols]);

/* preconditions: nrows and ncols are pointers to integers
 * postcondition: nrows and ncols will have integers read into them */
bool read_size_stdin (int * nrows, int * ncols);

/* preconditions: nrows and ncols are dimensions of the matrix
 *                matrix is an array of doubles
 * postcondition: matrix will be printed out to stdout */
void print_matrix (int nrows, int ncols,
		  double matrix[nrows][ncols]);

#endif
