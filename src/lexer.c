// ---------------------------------------- Header Files -----------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/lexer.h"

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

// Define structure for key-value pair
typedef struct
{
    const char *key;
    const char *value;
} KeyValuePair;

// Define the maximum size of the dictionary
#define MAX_SIZE 30

// Define the dictionary (array of key-value pairs)
KeyValuePair dictionary[MAX_SIZE];

// Initialize the dictionary with lookup table data
void initializeDictionary()
{
    // Add key-value pairs from the lookup table
    dictionary[0] = {"call", "TK_CALL"};
    dictionary[1] = {"record", "TK_RECORD"};
    // Add other key-value pairs here...
    dictionary[29] = {"return", "TK_RETURN"};
}

// Hash function to generate an index from a key
unsigned int hash(const char *key)
{
    unsigned int hashValue = 0;
    while (*key)
    {
        hashValue = (hashValue << 5) + *key++; // Basic hashing algorithm
    }
    return hashValue % MAX_SIZE; // Modulo operation to ensure index is within range
}

// Function to search for a key in the dictionary
const char *search(const char *key)
{
    unsigned int index = hash(key);
    while (index < MAX_SIZE && dictionary[index].key != NULL)
    {
        if (strcmp(key, dictionary[index].key) == 0)
        {
            return dictionary[index].value; // Key found, return corresponding value
        }
        index++; // Linear probing to handle collisions
    }
    return "Not found"; // Key not found
}

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

// Function to read the next character from the input stream
char getNextChar(twinBuffer *B)
{
    // Check if currentPosition exceeds bufferSize
    if (B->currentPosition >= B->bufferSize)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;
        // Refill buffer from file
        B->bufferSize = fread(B->currentBuffer == 0 ? B->buffer1 : B->buffer2, sizeof(char), MAX_BUFFER_SIZE, B->fp);
        // Reset currentPosition
        B->currentPosition = 0;
    }
    // Return next character from the current buffer
    return B->currentBuffer == 0 ? B->buffer1[B->currentPosition++] : B->buffer2[B->currentPosition++];
}

twinBuffer initTwinBuffer(FILE *fp, int bufferSize)
{
    twinBuffer B;
    B.fp = fp;
    B.bufferSize = bufferSize;
    B.currentPosition = 0;
    B.currentBuffer = 0;
    // Initialize buffers to '\0' characters
    memset(B.buffer1, '\0', MAX_BUFFER_SIZE);
    memset(B.buffer2, '\0', MAX_BUFFER_SIZE);
    // Fill buffers from file
    B.bufferSize = fread(B.buffer1, sizeof(char), bufferSize, fp);
    fread(B.buffer2, sizeof(char), bufferSize, fp);
    return B;
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