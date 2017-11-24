#ifndef RGUALAN_UTIL
#define RGUALAN_UTIL 1

#pragma once

/**
 * Reads <dim> elements of the <var>
 * from <stream>
 */
extern void readArray(FILE *stream, float *var, const int dim);

/**
 * Creates a text file with a snapshot of the matrix <var>
 */
extern void create_snapshot(char * file_name, float *var, const int ix, const int jx, const int kx);

//extern struct timeval rgu_t[10]; // Maximum 5 simultaneous timers
//extern int rgu_offset;

extern void validateIndex(int i);

extern void startTimer(int i);

extern void stopTimer(int i);

// Return the elapsed time in ms

extern int getElapsedTimeMs(int i);

#endif
