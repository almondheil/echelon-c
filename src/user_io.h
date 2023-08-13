#ifndef __USER_IO_H__
#define __USER_IO_H__

#include <stdbool.h>

/* Initialize a matrix from stdin.
 *
 * pre:  none
 * post: reads values from stdin into matrix
 */
bool read_matrix_stdin (int nrows, int ncols,
		       double matrix[nrows][ncols]);

/* Read the dimensions of an array from stdin.
 *
 * pre:  none 
 * post: nrows and ncols will reflect the user's input
 */
bool read_size_stdin (int* nrows, int* ncols);

/* Print a matrix.
 *
 * pre:  matrix is initialized
 * post: none
 */
void print_matrix (int nrows, int ncols, double matrix[nrows][ncols]);

#endif
