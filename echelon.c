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

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 128

/******************************************************************************
 * Function Declarations (by usage)                                           *
 ******************************************************************************/

/* reading values from stdin */
void read_matrix_stdin (int nrows, int ncols, double matrix[nrows][ncols]);
void read_size_stdin (int * nrows, int * ncols);

/* elementary row operations */
void add_scaled (int row1, int row2, double scalar, int nrows, int ncols, double matrix[nrows][ncols]);
void scale_row (int row, double scalar, int nrows, int ncols, double matrix[nrows][ncols]);
void swap_rows (int row1, int row2, int nrows, int ncols, double matrix[nrows][ncols]);

/* automatic calculations */
void echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);
void reduced_echelon_form (int nrows, int ncols, double matrix[nrows][ncols]);

/* matrix utilities */
int leading_pos (int row, int nrows, int ncols, double matrix[nrows][ncols]);
void print_matrix (int nrows, int ncols, double matrix[nrows][ncols]);

/******************************************************************************
 * Function Definitions (main, then alphabetical)                             *
 ******************************************************************************/

bool auto_accept = false;

/* main
 *
 * Prompt the user to enter a matrix and perform reduced echelon
 * calculations on it, printing out the results as you go.
 *
 * preconditions: arg is number of arguments
 *                argv is a char * array of arguments
 * postcondition: returns exit status (0 on success)
 */
int 
main (int argc, char * argv[])
{
    /* Parse arguments */
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != 0; j++) {
                switch (argv[i][j]) {
                    case 'y': 
                        auto_accept = true;
                        break;
                    default:
                        fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
                        return EXIT_FAILURE;
                        break;
                } // switch
            } // for argv[i][j] 
        } // if argv[i][0]
    } // for i < argc

    /* Ask for matrix size, create it, and ask to read values */
    int nrows, ncols;
    read_size_stdin(&nrows, &ncols);
    double matrix[nrows][ncols];
    read_matrix_stdin(nrows, ncols, matrix);

    /* Do the first part of the calculation, then prompt whether
     * they want to do the rest (unless they ran with -y)
     */
    echelon_form(nrows, ncols, matrix);

    if (!auto_accept) {
        printf("Echelon form calculation completed.\n"
                "Want to go to the reduced echelon form? [Y/n]: ");
        char ch = tolower(getchar());
        if (ch != 'n') {
            printf("\n");
            reduced_echelon_form(nrows, ncols, matrix);
            printf("Reduced echelon form calculation completed.\n");
        }
    } else {
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
        /* Make a best-effort attempt to get the leading value *just*
         * one column right from the previous one */
        int desired_leading = last_leading + 1;
        int current_leading = leading_pos(i, nrows, ncols, matrix);
        if (current_leading != desired_leading) {
            /* Search for another row that DOES have the desired leading value */
            for (int k = i+1; k < nrows; k++) {
                int k_leading = leading_pos(k, nrows, ncols, matrix);
                if (k_leading < current_leading) {
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


/* read_matrix_stdin
 *
 * Read the values of an initialized matrix from stdin.
 *
 * preconditions: nrows and ncols represent the size of matrix
 *                matrix is a 2d array of doubles of that size
 * postcondition: matrix will have user values added to it
 */
void
read_matrix_stdin (int nrows, int ncols, double matrix[nrows][ncols])
{
    int num_read;

    printf("Next, enter the matrix. Put spaces between columns,\n"
           "and press enter for each row.\n"
           "Here is an example:\n\n"
           "3 4 5\n6 7 8\n1 9 0\n\n"
           "Enter your matrix below.\n\n");

    /* Scan doubles into the array */
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            num_read = scanf("%lf", &matrix[i][j]);
            if (num_read == 0) {
                printf("Could not read a value.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    while (getchar() != '\n') // clear out the buffer to be a good citizen 
        ;

    printf("\n"); // formatting before we go into the calculations
}


/* read_size_stdin
 *
 * Prompt the user for how many rows and columns are in their matrix,
 * and copy this to variables
 *
 * preconditions: *nrows and *ncols are pointers to integers
 * postcondition: the number of rows and cols the user inputs will be
 *                stored in *nrows and *ncols
 */
void
read_size_stdin (int * nrows, int * ncols)
{
    int num_read;

    printf("This program will calculate the echelon form\n"
           "of a matrix that you input.\n"
           "First, enter the size of your matrix:\n");

    printf("Number of rows? ");
    num_read = scanf("%d", nrows);
    if (num_read == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        exit(EXIT_FAILURE);
    }

    printf("Number of columns? ");
    num_read = scanf("%d", ncols);
    if (num_read == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        exit(EXIT_FAILURE);
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
