#include "matrix_proc.h"

void add_scaled (int row1, int row2, double scalar,
        int nrows, int ncols, double matrix[nrows][ncols]) {
    for (int j = 0; j < ncols; j++)
        matrix[row1][j] += scalar * matrix[row2][j];
}

int leading_pos (int row, int nrows, int ncols,
        double matrix[nrows][ncols]) {
    for (int j = 0; j < ncols; j++) {
        if (matrix[row][j] != 0)
            return j;
    }
    return -1; // leading value not found
}

void scale_row (int row, double scalar,
        int nrows, int ncols, double matrix[nrows][ncols]) {
    if (scalar == 0) {
        return;
    } else {
        for (int j = 0; j < ncols; j++) {
            matrix[row][j] = matrix[row][j] * scalar;
        }
    }
}

void swap_rows (int row1, int row2,
        int nrows, int ncols, double matrix[nrows][ncols]) {
    for (int j = 0; j < ncols; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}
