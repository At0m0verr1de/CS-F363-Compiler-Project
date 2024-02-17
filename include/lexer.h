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

// Define token types enum
enum lookup
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
    TK_RETURN,
};

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
