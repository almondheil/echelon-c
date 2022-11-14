/*
 * echelon.c - Matrix echelon form calculator
 * Copyright (C) 2022 Ray Heil
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void add_scaled (int row1, int row2, double scalar, int nrows, int ncols, double matrix[nrows][ncols]);
void echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);
int leading_pos (int row, int nrows, int ncols, double matrix[nrows][ncols]);
void print_matrix (int nrows, int ncols, double matrix[nrows][ncols]);
void reduced_echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);
void swap_rows (int row1, int row2, int nrows, int ncols, double matrix[nrows][ncols]);
void scale_row (int row, double scalar, int nrows, int ncols, double matrix[nrows][ncols]);


/* main
 *
 * Prompt the user to enter a matrix and perform reduced echelon
 * calculations on it, printing out the results as you go.
 *
 * precondition: none  
 * postcondition: returns exit status (0 on success)
 */
int 
main (void)
{
    /* Prompt the user for the size of the matrix */
    int nrows, ncols;
    int numRead;

    printf("This program will calculate the echelon form\n"
           "of a matrix that you input.\n"
           "First, enter the size of your matrix:\n");

    printf("  number of rows : ");
    numRead = scanf("%d", &nrows);
    if (numRead == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        return EXIT_FAILURE;
    }

    printf("  number of cols : ");
    numRead = scanf("%d", &ncols);
    if (numRead == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        return EXIT_FAILURE;
    }

    /* Create a matrix with that many rows and columns */
    double matrix[nrows][ncols];

    printf("Next, enter the matrix. Put spaces between columns, and press enter for each row.\n"
           "Here is an example:\n"
           "  3 4 5\n  6 7 8\n  1 9 0\n"
           "Enter your matrix below.\n\n");

    /* Scan doubles into the array */
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            numRead = scanf("%lf", &matrix[i][j]);
            if (numRead == 0) {
                printf("Could not read a value.\n");
                return EXIT_FAILURE;
            }
        }
    }
    printf("\n");

    while (getchar() != '\n') // clear out buffer as we'll getchar() soon
        ;

    /* No matter what, do the first part of the calculation */
    echelon_form(nrows, ncols, matrix);

    printf("Echelon form calculation completed.\n"
           "Want to go to the reduced echelon form? [Y/n]: ");
    char ch = tolower(getchar());
    if (ch != 'n') {
        printf("\n");
        reduced_echelon_form(nrows, ncols, matrix);
        printf("Reduced echelon form calculation completed.\n");
    }

    // Okay, we're good! Exit with no errors.
    return EXIT_SUCCESS;
}

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
        /* Is there a leading entry where we want it to be?
         * If not, try to swap with a row that has a leading entry in
         * that position. */
        if (leading_pos(i, nrows, ncols, matrix) <= last_leading) {
            for (int k = i+1; k < nrows; k++) {
                if (leading_pos(k, nrows, ncols, matrix) > last_leading) {
                    swap_rows(i, k, nrows, ncols, matrix);
                    print_matrix(nrows, ncols, matrix);
                    break; // immediately stop searching
                }
            }
        }

        /* If we tried and failed to swap, raise an error. 
         * This shouldn't happen, because a column of all zeroes is redundant */
        int leading = leading_pos(i, nrows, ncols, matrix);
        if (leading == -1) {
            continue; // if no leading value, we better not try more stuff
        } else if (leading < last_leading) {
            fprintf(stderr, "Could not find a row to swap with.\n");
            exit(EXIT_FAILURE);
        }

        /* Now, try to scale the row so that the leading value is 1 */
        if (matrix[i][leading] != 1) {
            double temp = matrix[i][leading];
            scale_row(i, 1/temp, nrows, ncols, matrix);
            print_matrix(nrows, ncols, matrix);
        }

        /* Use the newly scaled problem to cancel out that position elsewhere */
        for (int k = i+1; k < nrows; k++) {
            double temp = -1 * matrix[k][leading];
            add_scaled(k, i, temp, nrows, ncols, matrix);
            printf("add R%d + (%.2lf * R%d)\n", k+1, temp, i+1);
            print_matrix(nrows, ncols, matrix);
        }
        last_leading = leading; // Update the most recent leading position
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
    return -1; // not found
}

/* print_matrix
 *
 * Display a matrix by printing out the values.
 *
 * precondition:  nrows and ncols are the number of rows and columns in the matrix
 *                matrix is an nrows x ncols array of doubles
 * postcondition: none, matrix is just printed out.
 */
void 
print_matrix (int nrows, int ncols, double matrix[nrows][ncols])
{
    for (int i = 0; i < ncols+2; i++)
        printf("*****");
    printf("\n");
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            if (matrix[i][j] == -0) {
                matrix[i][j] = 0.0;
            }
            printf("%5.2lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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
            add_scaled(k, i, temp, nrows, ncols, matrix);
            printf("add R%d + (%.2lf * R%d)\n", k+1, temp, i+1);
            print_matrix(nrows, ncols, matrix);
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
    printf("swap R%d <--> R%d\n", row1+1, row2+1);
    for (int j = 0; j < ncols; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
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
        fprintf(stderr, "Warning: Attempted to scale row %d by 0, skipping...\n", row);
    } else {
        printf("scale (1/%.2lf) * R%d\n", scalar, row+1);
        for (int j = 0; j < ncols; j++) {
            matrix[row][j] = matrix[row][j] * scalar;
        }
    }
}
