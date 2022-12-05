#ifndef __MANUAL_H__
#define __MANUAL_H__

#include <stdbool.h>

int matrix_menu (int nrows, int ncols, double matrix[nrows][ncols]);
/* preconditions: nrows and ncols are dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns 0 on success, 1 on failure */

int manual_scale (int nrows, int ncols, double matrix[nrows][ncols]);
/* preconditions: nrows and ncols are dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns 0 on success, 1 on failure */

int manual_add_scaled (int nrows, int ncols, double matrix[nrows][ncols]);
/* preconditions: nrows and ncols are dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns 0 on success, 1 on failure */

int manual_swap (int nrows, int ncols, double matrix[nrows][ncols]);
/* preconditions: nrows and ncols are dimensions of matrix
 *                matrix is an array of doubles
 * postcondition: returns 0 on success, 1 on failure */


#endif
