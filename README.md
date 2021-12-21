# linux-programming-interface-exercises
Solutions to exercises from the Linux Programming Interface book.

# compiling
In order to compile the solutions, you need headers and built library from https://man7.org/tlpi/code/index.html

Assuming that headers and the built library named `libtlpi.a` are located in the `lib` folder in the project root:

```
cd fileio/5-further-details
gcc -I../../lib exercise_3_atomic_append.c -ltlpi -L../../lib
```
