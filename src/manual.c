#include <stdio.h>
#include <ctype.h>        // tolower
#include "user_io.h"      // matrix printing
#include "matrix_proc.h"  // actual row operations
#include "manual.h"

// TODO: This whole file is a mess, and needs to be reworked.
// That's not a project for the moment

int matrix_menu (void) {
    printf("What action to perform?\n"
           "  q - quit\n"
           "  s - swap two rows\n"
           "  a - add rows (optional scale)\n"
           "  m - multiply a row by a scalar\n");

    char ch;
    while (true) { // loop exits on any successful choice
        ch = getchar();

        while (getchar() != '\n')
            ; // chomp any remaining characters

        switch (tolower(ch)) {
            case 'q': // quit
                return 0;
            case 's': // swap
                return 1;
            case 'a': // add
                return 2;
            case 'm': // multiply
                return 3;
        }

        printf("Unrecognized option %c. Please try again.\n", ch);
    }
}

// This function is not included in manual.h, as I only use it in this file
bool scan_row (int * rownum, int nrows)
{
    int scanned;
    do {
        printf("Enter a row number, 0 to %d or -1 to cancel.");
        scanned = scanf("%d", rownum);
        // TODO NO, THIS IS ALL BAD. UGH.

    } while (*rownum != -1);

    while (getchar() != '\n')
        ; // chomp remaining chars

    if (scanned == 0) { // failed to scan anything
        return false;
    } else if (*rownum < 1 || *rownum > nrows) {
        return false;
    }

    return true;
}

bool scan_scalar (double * scalar)
{
    int num, den; // numerator and denominator of function
    int scanned = scanf("%d/%d", &num, &den);
    if (scanned != 2) {
        return false;
    }
    *scalar = num/den; // calculate double representation of that fraction
    if (*scalar == 0) {
        return false;
    }

    return true;
}

int manual_scale (int nrows, int ncols, double matrix[nrows][ncols])
{
    int row;       // which row to scale
    double scalar; // what to scale it by
    
    /* Find which row to scale, with error checking */
    printf("Now scaling a row.\n"
           "Enter the number of a row (starting at 1), or 0 to cancel: ");
    if (!scan_row(&row, nrows)) {
        printf("Error scanning the row.\n");
        return 1;
    }

    /* Find what to scale it by, with error checking */
    printf("Enter the scalar (as a fraction like -1/3): ");
    if (!scan_scalar(&scalar)) {
        printf("Error scanning the scalar.\n");
        return 1;
    }

    /* If we passed all those checks, it's time to scale */
    scale_row(row-1, scalar, nrows, ncols, matrix);
    return 0;
}

int manual_add_scaled (int nrows, int ncols, double matrix[nrows][ncols])
{
    int row1;      // the row that will be changed
    int row2;      // the row to add to it
    double scalar; // the scalar to multiply by
    printf("Now adding a scalar multiple of one row to another.\n");

    /* get the values for row1 and row2 */
    printf("Enter the number of the first row (the one that will be added to): ");
    if (!scan_row(&row1, nrows)) {
        printf("Could not scan the row.\n");
        return 1;
    }
    printf("Now, enter the number of the second row (the one that will be scaled): ");
    if (!scan_row(&row2, nrows)) {
        printf("Could not scan the row.\n");
        return 1;
    }

    /* get the value for scalar */
    printf("Finally, enter the scalar (as a fraction, like 4/1): ");
    if (!scan_scalar(&scalar)) {
        printf("Could bot scan the scalar.\n");
        return 1;
    }

    /* by now all our values are good, so we do the function. */
    add_scaled(row1-1, row2-1, scalar, nrows, ncols, matrix);
    return 0;
}

int manual_swap (int nrows, int ncols, double matrix[nrows][ncols])
{
    int row1;      // the two rows we swap 
    int row2;
    int scanned;   // the number of arguments scanned (validation)
    printf("Now adding a scalar multiple of one row to another.\n");

    /* get which two rows to swap */ 
    printf("Enter the number of the first row to swap: ");
    scanned = scanf("%d", &row1);
    if (scanned == 0) {
        printf("Could not scan the row. Please try again.\n");
        return 1;
    } else if (row1 < 1 || row1 > nrows) {
        printf("Row %d does not exist on the matrix.\n", row1);
        return 1;
    }
    printf("Now, enter the number of the second row to swap: "); 
    scanned = scanf("%d", &row2);
    if (scanned == 0) {
        printf("Could not scan the row. Please try again.\n");
        return 1;
    } else if (row2 < 1 || row2 > nrows) {
        printf("Row %d does not exist on the matrix.\n", row1);
        return 1;
    }

    /* if we make it past the checks we're safe to run the command */
    swap_rows(row1-1, row2-1, nrows, ncols, matrix);
    return 0;
}
