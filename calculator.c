#include <stdio.h>
#include <stdlib.h>
#include "user_io.h"
#include "matrix_proc.h"

/* echelon_form
 *
 * Put a matrix into echelon form. Modifies the original matrix, 
 * so maybe you want to make a copy!
 *
 * precondition:  nrows and ncols are the number of rows and columns in the matrix
 *                matrix is an nrows x ncols array of doubles
 * postcondition: matrix will end up in echelon form
 */
void
echelon_form (int nrows, int ncols, double matrix[nrows][ncols])
{
    printf("initial state of matrix\n");
    print_matrix(nrows, ncols, matrix);

    int last_leading = -1; /* start off with an invalid leading pos */

    /* Go from a matrix to its echelon form */
    for (int i = 0; i < nrows; i++) {
        /* Make a best-effort attempt to get the leading value *just*
         * one column right from the previous one */
        int desired_leading = last_leading + 1;
        int current_leading = leading_pos(i, nrows, ncols, matrix);
        if (current_leading != desired_leading) {
            /* Search for another row that DOES have the desired leading value */
            for (int k = i+1; k < nrows; k++) {
                int k_leading = leading_pos(k, nrows, ncols, matrix);
                if (k_leading < current_leading) {
                    printf("swap R%d <--> R%d\n", i+1, k+1);
                    swap_rows(i, k, nrows, ncols, matrix);
                    current_leading = k_leading;
                    // stop if we get a perfect match early
                    if (k_leading == desired_leading)
                        break;
                }
            }
        }

        /* If we tried and failed to swap, raise an error. 
         * This shouldn't happen, because a column of all zeroes is redundant */
        if (current_leading == -1) {
            continue; // if no leading value, we better not try more stuff
        } else if (current_leading < last_leading) {
            fprintf(stderr, "Could not find a row to swap with.\n");
            exit(EXIT_FAILURE);
        }

        /* Now, try to scale the row so that the leading value is 1 */
        if (matrix[i][current_leading] != 1) {
            double temp = matrix[i][current_leading];
            printf("scale (1/%.2lf) * R%d\n", temp, i+1);
            scale_row(i, 1/temp, nrows, ncols, matrix);
            print_matrix(nrows, ncols, matrix);
        }

        /* Use the newly scaled problem to cancel out that position elsewhere */
        for (int k = i+1; k < nrows; k++) {
            double temp = -1 * matrix[k][current_leading];
            add_scaled(k, i, temp, nrows, ncols, matrix);
            printf("add R%d + (%.2lf * R%d)\n", k+1, temp, i+1);
            print_matrix(nrows, ncols, matrix);
        }
        last_leading = current_leading; // Update the most recent leading position
    }
}

/* reduced_echelon_form
 *
 * Given a matrix in echelon form, reduce it by cancelling out 
 * values above where possible.
 *
 * preconditions: nrows and ncols are the number of rows and columns in matrix
 *                matrix is an array of doubles
 * postcondition: matrix will end up in reduced echelon form
 */
void reduced_echelon_form (int nrows, int ncols, double matrix[nrows][ncols])
{
    /* Cancel out what you can in all the rows above */
    for (int i = nrows-1; i >= 0; i--) {
        /* Make sure the row has a leading value and find what it is */
        /* TODO this is being calculated wront. why? */
        int lead = leading_pos(i, nrows, ncols, matrix);
        if (lead == -1)
            continue; // if there's no leading value there's no point to continue
        
        /* Subtract from every previous row */
        for (int k = i-1; k >= 0; k--) {
            double temp = -1 * matrix[k][lead] / matrix[i][lead]; 
            if (temp != 0) {
                add_scaled(k, i, temp, nrows, ncols, matrix);
                printf("add R%d + (%.2lf * R%d)\n", k+1, temp, i+1);
                print_matrix(nrows, ncols, matrix);
            }
        }
    }
}

