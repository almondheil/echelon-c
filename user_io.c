#include <stdio.h>
#include <stdlib.h>
#include "user_io.h"

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
bool
read_matrix_stdin (int nrows, int ncols, double matrix[nrows][ncols])
{
    bool success = false;
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
                return success;
            }
        }
    }
    while (getchar() != '\n') // clear out the buffer to be a good citizen 
        ;

    printf("\n"); // formatting before we go into the calculations
    success = true;
    return success;
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
bool
read_size_stdin (int * nrows, int * ncols)
{
    bool success = false;
    int num_read;

    printf("This program will calculate the echelon form\n"
           "of a matrix that you input.\n"
           "First, enter the size of your matrix:\n");

    printf("Number of rows? ");
    num_read = scanf("%d", nrows);
    if (num_read == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        return success;
    }

    printf("Number of columns? ");
    num_read = scanf("%d", ncols);
    if (num_read == 0) {
        fprintf(stderr, "Failed to read number of rows. Make sure it was an integer.\n");
        return success;
    }

    success = true;
    return success;
}

