# echelon-c

A simple echelon form calculator. Nice for Linear Algebra homework!

## Probably don't actually use this

I made this as a toy and experiment as much as for solving actual problems, and
I make no guarantee that it actually works outside of the situations I've run
into on homework. As I write this section of the README, I ran into a silly bug
no less than half an hour ago and had ti fix it.

## Compiling 

Just use whatever compiler you want. The compiler needs to be able to access
`<stdio.h>`, `<stdlib.h>`, and `<ctype.h>`, all of which are pretty common to my
knowledge and don't require any special compiler flags.

Example:

```
gcc -o echelon echelon.c
```

## Usage

Run the program, then enter the number of rows and columns in your matrix.

Enter the matrix when the program prompts, separating each cell in a column with
a space like so:

```
1 2 3
4 5 6
7 8 9
```

Alignment doesn't matter, but beware--I don't know how picky scanf is with that
sort of thing, and it will yell at you and break if it doesn't read the input
the way it expects.

The program will automatically calculate the echelon form of your matrix, then
pause to prompt whether or not you also want to find the reduced echelon form
from this point.

