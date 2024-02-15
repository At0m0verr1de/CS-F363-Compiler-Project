#include <time.h> // For time measurement
#include <stdio.h>
#include "../include/executionTime.h"

// Function to measure execution time
void measureExecutionTime(const char *sourceFile)
{
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    start_time = clock();

    // Invoke lexer and parser here

    end_time = clock();

    total_CPU_time = (double)(end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    printf("Total CPU time: %f\n", total_CPU_time);
    printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
}
