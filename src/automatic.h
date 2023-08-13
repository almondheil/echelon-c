#ifndef __AUTOMATIC_H__
#define __AUTOMATIC_H__

#include <stdbool.h>

/* Put a matrix into echelon form. Overwrites data.
 *
 * pre:  matrix is initialized 
 * post: returns true if reached echelon form, false otherwise
 */
bool auto_echelon (int nrows, int ncols, double matrix[nrows][ncols]);

/* Given a matrix in echelon form, reduce it by cancelling out 
 * values above where possible.
 *
 * pre:  matrix is initialized and already in echelon form
 * post: returns true if reached reduced echelon form, false otherwise
 */
bool auto_reduced_echelon (int nrows, int ncols, double matrix[nrows][ncols]);

#endif
