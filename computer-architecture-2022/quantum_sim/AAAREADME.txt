This directory contains the basic source code for a simulator for a
quantum computer.  This is being used as an example program in the
Computer Architecture class at Keio's Shonan Fujisawa Campus, fall
2011.

The code was originated by Austin Fowler, and modified by Clare
Horsman and Rod Van Meter.

The primary points of the exercises here are:

1. Amdahl's Law
2. Gustafson's Law
3. Achievable parallelism
4. Basics of parallel programming (using OpenMP)
5. System performance measurement

In the short run, all you really need to look at is the 

#pragma omp

statements in sim.c.
