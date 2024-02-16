// ---------------------------------------- Header Files -----------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"

// ------------------------------------------- Macros --------------------------------------------
#define MAX_LEXEME_SIZE 100
#define END_OF_FILE 0
#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

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
twinBuffer B;

// // Initialize the dictionary with lookup table data
// void initializeDictionary()
// {
//     // Add key-value pairs from the lookup table
//     dictionary[0] = {"call", "TK_CALL"};
//     dictionary[1] = {"record", "TK_RECORD"};
//     // Add other key-value pairs here...
//     dictionary[29] = {"return", "TK_RETURN"};
// }

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

// // Function to search for a key in the dictionary
// const char *search(const char *key)
// {
//     unsigned int index = hash(key);
//     while (index < MAX_SIZE && dictionary[index].key != NULL)
//     {
//         if (strcmp(key, dictionary[index].key) == 0)
//         {
//             return dictionary[index].value; // Key found, return corresponding value
//         }
//         index++; // Linear probing to handle collisions
//     }
//     return "Not found"; // Key not found
// }

// ------------------------------------ Function Definitions -------------------------------------

// Function to get next token
TokenInfo getNextToken(twinBuffer *B, FILE *fp)
{
    int state = 0;
    TokenInfo *token = (TokenInfo *)malloc(sizeof(TokenInfo));
    char currentChar;

    while (1)
    {
        printf("yay\n");
        break;
        currentChar = getNextChar(B);
        token.lexeme[0] = '\0';
        strncat(token.lexeme, &currentChar, 1);

        switch (state)
        {
        case 0:
            // state = isdigit(currentChar) ? 1 : 0;
            if (currentChar == '%')
                state = 24;
            else
                printf("lul");
            break;
        case 24:
            if (currentChar == '\n' || currentChar == EOF)
                state = 25;
            break;
        case 25:
            token.type = TK_COMMENT;
            return token;
        }
    }

    return token;
    // Implement getNextChar function to read characters
    // Implement DFA transitions
    // Tokenize lexemes and handle lexical errors
    // Return token information
}

char getNextChar(twinBuffer *B)
{
    // Check if currentPosition exceeds bufferSize
    if (B->currentPosition >= MAX_BUFFER_SIZE)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;
        // Refill buffer from file
        fread(B->currentBuffer == 0 ? B->buffer0 : B->buffer1, sizeof(char), MAX_BUFFER_SIZE, B->fp);
        // Reset currentPosition
        B->currentPosition = B->currentPosition % MAX_BUFFER_SIZE;
    }
    else if (B->currentPosition < 0)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;
        // Reset currentPosition
        B->currentPosition = B->currentPosition % MAX_BUFFER_SIZE;
    }
    // Return next character from the current buffer
    return B->currentBuffer == 0 ? B->buffer0[B->currentPosition++] : B->buffer1[B->currentPosition++];
}

void initTwinBuffer(twinBuffer *B, FILE *fp)
{
    B->fp = fp;
    B->currentPosition = 0;
    B->currentBuffer = 0;

    memset(B->buffer0, '\0', MAX_BUFFER_SIZE);
    memset(B->buffer1, '\0', MAX_BUFFER_SIZE);

    // Fill buffers from file
    fread(B->buffer0, sizeof(char), MAX_BUFFER_SIZE, fp);
    fread(B->buffer1, sizeof(char), MAX_BUFFER_SIZE, fp);
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

void lexer(FILE *fp)
{
    // Open source code file
    if (fp == NULL)
    {
        perror("Error opening file");
    }

    twinBuffer *B = (twinBuffer *)malloc(sizeof(twinBuffer));
    initTwinBuffer(B, fp);

    // Get next token and process
    TokenInfo token;
    do
    {
        token = getNextToken(B, fp);
        printf("Value of TK_COMMENT: %d ", token.type);
        printf("%s\n", token.lexeme);
    } while (token.type != EOF);

    fclose(fp);
    free(B);
}

int main()
{
    // Open source code file
    FILE *fp = fopen("source_code.txt", "r");
    lexer(fp);
    fclose(fp);
    return 0;
}