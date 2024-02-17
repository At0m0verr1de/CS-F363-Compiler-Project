#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h" // Include the lexer header file

// Function prototypes for other tasks
void printTokenList(const char *filename);
// void parseAndPrintParseTree(const char *sourceFile, const char *parseTreeFile);
void measureExecutionTime(const char *sourceFile);

int main(int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
    // if (argc != 3)
    // {
    //     printf("Usage: %s <source_code_file> <parse_tree_output_file>\n", argv[0]);
    //     return 1;
    // }

    // Display information regarding the implementation status
    printf("Implementation status:      Lexer Completed\n");
    // Add implementation status information here as per your project progress

    // Menu-driven interface
    int option;
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
            printTokenList("clean_source_code.txt"); // Call printTokenList function from lexer.c
            break;
        case 3:
            parseAndPrintParseTree(argv[1], argv[2]); // Call parseAndPrintParseTree function from lexer.c
            break;
        case 4:
            measureExecutionTime(argv[1]);
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    return 0;
}
