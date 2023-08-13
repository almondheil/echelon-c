#ifndef __MANUAL_H__
#define __MANUAL_H__

#include <stdbool.h>

/* Print a menu asking the user what they would like to do with the matrix.
 * Reprompts until they give a valid answer.
 * 
 * pre:  none
 * post: returns the integer value of the option the user chose
 */
int matrix_menu(void);

/* Manually prompt and validate input for scaling a row.
 * 
 * Reprompts until a valid answer is chosen.
 *
 * pre:  matrix is initialized
 * post: matrix will be modified such that row1 *= scalar
 */
int manual_scale (int nrows, int ncols, double matrix[nrows][ncols]);

/* Manually prompt and validate input for adding a scaled version of one row
 * to another.
 * 
 * Reprompts until a valid answer is chosen.
 *
 * pre:  matrix is initialized
 * post: matrix will be modified such that row1 += (scalar * row2)
 */
int manual_add_scaled (int nrows, int ncols, double matrix[nrows][ncols]);

/* Manually prompt and validate input for swapping two rows.
 *
 * Reprompts until a valid answer is chosen.
 *
 * pre:  matrix is initialized
 * post: matrix will be modified such that rows are swapped
 */
int manual_swap (int nrows, int ncols, double matrix[nrows][ncols]);


#endif
