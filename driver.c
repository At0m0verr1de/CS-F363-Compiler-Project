/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/
#include "parser.h"
#include <time.h>

int main(int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
    if (argc != 3)
    {
        printf("Usage: %s <source_code_file> <parse_tree_output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile;
    

    if (inputFile == NULL)
    {
        perror("Error Opening Source Code File");
        return 1;
    }

    // Menu-driven interface
    char option;
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    GRAMMAR grammar = (GRAMMAR)malloc(sizeof(struct Grammar));
    initGrammer(grammar);

    FirstAndFollow faf = ComputeFirstAndFollowSets(grammar);

    Table T = (NODE ***)malloc(MAX_NON_TERMINALS * sizeof(NODE **));
    createParseTable(faf, T);
    parseTree root = NULL;

    
    
    do
    {
        printf("\nOptions:\n");
        printf("0: Exit\n");
        printf("1: Remove comments\n");
        printf("2: Print token list\n");
        printf("3: Parse and print parse tree\n");
        printf("4: Measure execution time\n");
        printf("Enter option: ");
        scanf("%s", &option);
        
        switch (option)
        {
        case '0':
            printf("Exiting...\n");
            break;
        case '1':
            removeComments(argv[1], "clean_source_code.txt"); // Call removeComments function from lexer.c
            break;
        case '2':
            inputFile = fopen(argv[1], "r");
            lexer(inputFile); // prints the tokens to command line
            break;
        case '3':
            root = parseInputSourceCode(argv[1], T);
            printParseTree(root, argv[2]);
            break;
        case '4':
            start_time = clock();
            inputFile = fopen(argv[1], "r");           
            lexer(inputFile);
            root = parseInputSourceCode(argv[1], T);
            // if correct syntactically print parse tree
            printParseTree(root, argv[2]);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU time: %f\n", total_CPU_time);
            printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    } while (option != '0');
    return 0;
}
