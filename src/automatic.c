#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "user_io.h"
#include "matrix_proc.h"

bool auto_echelon (int nrows, int ncols, double matrix[nrows][ncols])
{
    bool success = false;
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
                    print_matrix(nrows, ncols, matrix);
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
            return success;
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
            printf("add R%d + (%.2lf * R%d)\n", k+1, temp, i+1);
            add_scaled(k, i, temp, nrows, ncols, matrix);
            print_matrix(nrows, ncols, matrix);
        }
        last_leading = current_leading; // Update the most recent leading position
    }
    success = true;
    return success;
}

bool auto_reduced_echelon (int nrows, int ncols, double matrix[nrows][ncols])
{
    bool success = false;
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
    success = true;
    return success;
}

