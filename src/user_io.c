#include <stdio.h>
#include <stdlib.h>
#include "user_io.h"

void print_matrix (int nrows, int ncols, double matrix[nrows][ncols])
{
    for (int i = 0; i < ncols+2; i++)
        printf("*****");
    printf("\n");
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            if (matrix[i][j] == -0) {
                matrix[i][j] = 0.0;
            }
            printf("%5.4lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool read_matrix_stdin (int nrows, int ncols, double matrix[nrows][ncols])
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

bool read_size_stdin (int * nrows, int * ncols)
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

