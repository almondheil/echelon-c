#include "matrix_proc.h"

/* add_scaled
 *
 * Add a scaled version of row2 to row1, modifying the values in row1.
 *
 * preconditions: row1 and row2 are valid rows of matrix
 *                scalar is any double, including zero (it won't do anything).
 *                nrows and ncols are the number of rows and columns in matrix
 *                matrix is an array of doubles
 * postcondition: returns nothing, but modifies the values in row1.
 */
void 
add_scaled (int row1, int row2, double scalar, int nrows, int ncols, double matrix[nrows][ncols])
{
    for (int j = 0; j < ncols; j++) {
        matrix[row1][j] = matrix[row1][j] + (scalar * matrix[row2][j]);
    }
}

/* leading_pos
 *
 * Locate the leading entry in a row and return what column it is in.
 *
 * preconditions: row is a valid index of matrix
 *                nrows and ncols are the number of rows and columns in matrix
 *                matrix is an array of doubles
 * postcondition: returns what column the leading value is in.
 *                start at index 0. if there is no leading entry, returns -1
 */
int
leading_pos (int row, int nrows, int ncols, double matrix[nrows][ncols])
{
    for (int j = 0; j < ncols; j++) {
        if (matrix[row][j] != 0)
            return j;
    }
    return -1; // leading value not found
}

/* scale_row
 *
 * Modify a row by multiplying all values by a nonzero scalar.
 *
 * preconditions: row is a valid index of matrix
 *                scalar is a nonzero double
 *                nrows and ncols are the number of rows and columns in matrix
 *                matrix is an array of doubles
 * postcondition: returns nothing, but modifies the values in row.
 */
void
scale_row (int row, double scalar, int nrows, int ncols, double matrix[nrows][ncols])
{
    if (scalar == 0) {
        return;
    } else {
        for (int j = 0; j < ncols; j++) {
            matrix[row][j] = matrix[row][j] * scalar;
        }
    }
}

/* swap_rows
 *
 * Change the positions of two rows by swapping each of their values
 *
 * preconditions: row1 and row2 are valid indices of matrix
 *                nrows and ncols are the number of rows and columns in matrix
 *                matrix is an array of doubles
 * postcondition: the values for each column of row1 and row2 are swapped
 */
void
swap_rows (int row1, int row2, int nrows, int ncols, double matrix[nrows][ncols])
{
    for (int j = 0; j < ncols; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}
