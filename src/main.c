/* main.c - Matrix echelon form calculator
 * Copyright (C) 2022 almond heil
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
#include "automatic.h"  // ref and rref calculations done by the computer
#include "manual.h"     // allow the user to do their own calculations
#include "user_io.h"    // matrix reading and printing

/* Run in automatic mode on a matrix.
 * 
 * pre:  matrix is initialized
 * post: returns 0 on success, nonzero on failure
 */
int automatic_mode(int nrows, int ncols, double matrix[nrows][ncols]);

/* Run in manual ode on a matrix.
 * 
 * pre:  matrix is initialized
 * post: returns 0 on success, nonzero on failure
 */
int manual_mode(int nrows, int ncols, double matrix[nrows][ncols]);

/* Prompt the user to enter a matrix and perform reduced echelon
 * calculations on it, printing out the results as you go.
 *
 * pre:  none, the launching shell will deal with it
 * post: nonzero exit status indicates failure 
 */
int main (int argc, char * argv[]) {
    bool manual = false; // manual mode?
    bool success;        // operation success
    int ret;             // program return status

    /* Parse arguments */
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != 0; j++) {
                switch (argv[i][j]) {
                    case 'h': // help
                        printf("Usage: %s [FLAGS]\n\n"
                               "FLAGS:\n"
                               "  -m  Manually enter row operations\n"
                               "  -a  Automatic calculation (default)\n", argv[0]);
                        return 0;
                    case 'm': // manual mode
                        manual = true;
                        break;
                    case 'a': // automatic mode (default, but can also be set)
                        manual = false;
                        break;
                    default:  // halt on unrecognized option
                        fprintf(stderr, "Unknown option: %s.\n"
                                        "Use %s -h for help.\n", argv[i], argv[0]);
                        return EXIT_FAILURE;
                        break;
                }
            }
        }
    }

    /* Regardless of whether running in automatic or manual mode, 
     * we need to read dimensions and create a matrix accordingly */ 
    int nrows, ncols;
    success = read_size_stdin(&nrows, &ncols);
    if (!success) {
        fprintf(stderr, "Error encountered while reading matrix size\n");
        return EXIT_FAILURE;
    }
    double matrix[nrows][ncols];
    success = read_matrix_stdin(nrows, ncols, matrix);
    if (!success) {
        fprintf(stderr, "Error encountered while reading matrix values\n");
        return EXIT_FAILURE;
    }

    // Run either in manual or automatic mode
    if (manual)
        ret = manual_mode(nrows, ncols, matrix);
    else
        ret = automatic_mode(nrows, ncols, matrix);

    return ret;
}

int automatic_mode(int nrows, int ncols, double matrix[nrows][ncols]) {
    bool success;
    printf("inital state\n");
    print_matrix(nrows, ncols, matrix);

    success = auto_echelon(nrows, ncols, matrix);
    if (!success) {
        fprintf(stderr, "Error encountered in the echelon form. Exiting...\n");
        return EXIT_FAILURE;
    }

    printf("Echelon form calculation completed.\n"
            "Want to go to the reduced echelon form? [Y/n]: ");
    char ch = tolower(getchar());
    if (ch == 'n') {
        printf("\nProgram completed!\n");
        return EXIT_SUCCESS;
    }

    // implicit else for auto_accept and ch == 'n'
    success = auto_reduced_echelon(nrows, ncols, matrix);
    if (!success) {
        fprintf(stderr, "Error encountered in reduced echelon form. Exiting...\n");
        return EXIT_FAILURE;
    }
    printf("Reduced echelon form calculation completed.\n");

    return EXIT_SUCCESS;
}

int manual_mode(int nrows, int ncols, double matrix[nrows][ncols]) {
    int option;
    do {
        print_matrix(nrows, ncols, matrix);
        option = matrix_menu();
        switch (option) {
            case 1: // swap rows
                manual_swap(nrows, ncols, matrix);
                break;
            case 2: // add rows (with scalar)
                manual_add_scaled(nrows, ncols, matrix);
                break;
            case 3: // scale row
                manual_scale(nrows, ncols, matrix);
                break;
        }
    } while (option != 0);

    return EXIT_SUCCESS;
}

