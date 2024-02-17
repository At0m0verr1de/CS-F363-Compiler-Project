#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dict.h"

// Define necessary constants and structures
#define MAX_LEXEME_SIZE 100
#define MAX_TOKEN_SIZE 15
#define MAX_BUFFER_SIZE 5

// Token information structure
typedef struct
{
    char type[MAX_TOKEN_SIZE];
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
    bool end;
} TokenInfo;

// Define structure for twin buffer
typedef struct
{
    char buffer0[MAX_BUFFER_SIZE];
    char buffer1[MAX_BUFFER_SIZE];
    int currentBuffer;   // Indicates the currently active buffer (0 or 1)
    int currentPosition; // Current position in the currently active buffer
    int lineNumber;
    FILE *fp; // File pointer for input stream
} twinBuffer;

// Function declarations
TokenInfo getNextToken(twinBuffer *B, FILE *fp, struct Dictionary *dict);
char getNextChar(twinBuffer *B);
void initTwinBuffer(twinBuffer *B, FILE *fp);
// void removeComments(char *testcaseFile, char *cleanFile);
#endif /* LEXER_H */
