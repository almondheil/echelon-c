#------------------------------------------------------------------------------
# File:         Makefile
# Author:       almond heil
# Created:      2022-11-09 
# Last revised: 2023-08-12 
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

.PHONY: all
all: echelon

echelon: src/main.o src/automatic.o src/manual.o src/user_io.o src/matrix_proc.o
	$(CC) -o $@ $^

main.o: main.c automatic.h user_io.h 
	$(CC) $(CFLAGS) -c $<

automatic.o: automatic.c automatic.h matrix_proc.h user_io.h
	$(CC) $(CFLAGS) -c $<

manual.o: manual.c manual.h matrix_proc.h user_io.h
	$(CC) $(CFLAGS) -c $<

user_io.o: user_io.c user_io.h
	$(CC) $(CFLAGS) -c $<

matrix_proc.o: matrix_proc.c matrix_proc.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f echelon src/*.o *~ core*
