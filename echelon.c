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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"  // ref and rref calculations
#include "user_io.h"     // matrix reading and printing

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
