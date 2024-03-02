/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
// #include "parser.h"
#include <time.h>

int main(int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
    if (argc != 3)
    {
        printf("Usage: %s <source_code_file> <parse_tree_output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");

    if (inputFile == NULL)
    {
        perror("Error Opening Source Code File");
        return 1;
    }
    else if (outputFile == NULL)
    {
        perror("Error opening Parse Tree Output file");
        return 1;
    }

    // Menu-driven interface
    int option;
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    do
    {
        printf("\nOptions:\n");
        printf("0: Exit\n");
        printf("1: Remove comments\n");
        printf("2: Print token list\n");
        printf("3: Parse and print parse tree\n");
        printf("4: Measure execution time\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option)
        {
        case 0:
            printf("Exiting...\n");
            break;
        case 1:
            removeComments(argv[1], "clean_source_code.txt"); // Call removeComments function from lexer.c
            break;
        case 2:
            lexer(argv[1]); // prints the tokens to command line
            break;
        case 3:
            // lexer(argv[1]);
            // parseAndPrintParseTree(argv[1], argv[2]); // Call parseAndPrintParseTree function from lexer.c
            break;
        case 4:
            start_time = clock();
            lexer(argv[1]);
            // parser(argv[1]);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU time: %f\n", total_CPU_time);
            printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
