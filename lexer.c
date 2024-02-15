// ---------------------------------------- Header Files -----------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ------------------------------------------- Macros --------------------------------------------
#define MAX_LEXEME_SIZE 100
#define END_OF_FILE 0
#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

// ---------------------------------------- Lookup Table -----------------------------------------
enum TokenType
{
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_PARAMETERS,
    TK_WITH,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_DEFINETYPE,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN
};

// ------------------------------------ Structure Definitions ------------------------------------
typedef struct
{
    enum TokenType type;
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
} TokenInfo;

typedef struct
{
    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];
    int currentBuffer;   // Indicates the currently active buffer (0 or 1)
    int bufferSize;      // Size of each buffer
    int currentPosition; // Current position in the currently active buffer
    FILE *fp;            // File pointer for input stream
} twinBuffer;

// ------------------------------------ Function Definitions -------------------------------------

FILE *getStream(FILE *fp) // Stream from file
{
    // Implement efficient technique to populate twin buffer
    // Return file pointer
}

// Function to get next token
TokenInfo getNextToken(twinBuffer B)
{
    // Implement getNextChar function to read characters
    // Implement DFA transitions
    // Tokenize lexemes and handle lexical errors
    // Return token information
}

// Function to remove comments from the input file and create a new file without comments
void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE *inputFile = fopen(testcaseFile, "r");
    if (inputFile == NULL)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *outputFile = fopen(cleanFile, "w");
    if (outputFile == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        fprintf(outputFile, "%s", line); // Write the entire line to the output file
    }

    fclose(inputFile);
    fclose(outputFile);
}

// ---------------------------------------- Main Function -----------------------------------------

int main(int argc, char *argv[])
{
    // Check if filename is provided as command-line argument
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open source code file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Get stream
    FILE *stream = getStream(fp);

    // Get next token and process
    TokenInfo token;
    do
    {
        token = getNextToken(stream);
        // Process token
    } while (token.type != END_OF_FILE);

    // Close file
    fclose(fp);

    return 0;
}