#------------------------------------------------------------------------------
# File:         Makefile
# Author:       Ray Heil
# Created:      19 Nov 2022
# Last revised: 19 Nov 2022
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Acknowledgement
# Format and some sections adapted from an example by Henry Walker, which
# was itself based on an example by Marge Coahran
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Use the clang compiler
#------------------------------------------------------------------------------
CC = clang

#------------------------------------------------------------------------------
# Set compilation flags
#   -ansi       check against the American National Standard for C
#   -g          include debugging info
#   -Wall       give all warnings
#   -std=gnu99  use the gnu99 standard
#------------------------------------------------------------------------------
CFLAGS = -ansi -g -Wall -std=gnu99

#------------------------------------------------------------------------------
# Compilation rules
#------------------------------------------------------------------------------

all: echelon

echelon: echelon.o calculator.o user_io.o matrix_proc.o
	$(CC) -o $@ $^

echelon.o: echelon.c calculator.h user_io.h 
	$(CC) $(CFLAGS) -c $<

calculator.o: calculator.c calculator.h matrix_proc.h user_io.h
	$(CC) $(CFLAGS) -c $<

user_io.o: user_io.c user_io.h
	$(CC) $(CFLAGS) -c $<

matrix_proc.o: matrix_proc.c matrix_proc.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f echelon *.o *~ core*
