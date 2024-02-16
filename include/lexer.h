#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

// Define necessary constants and structures
#define MAX_LEXEME_SIZE 100
#define END_OF_FILE 0
#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

// Define token types enum
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
    TK_RETURN,
    TK_COMMENT
};

// Token information structure
typedef struct
{
    enum TokenType type;
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
} TokenInfo;

// Define structure for twin buffer
typedef struct
{
    char buffer0[MAX_BUFFER_SIZE];
    char buffer1[MAX_BUFFER_SIZE];
    int currentBuffer;   // Indicates the currently active buffer (0 or 1)
    int bufferSize;      // Size of each buffer
    int currentPosition; // Current position in the currently active buffer
    FILE *fp;            // File pointer for input stream
} twinBuffer;

// Function declarations
TokenInfo getNextToken(twinBuffer *B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
char getNextChar(twinBuffer *B);
// twinBuffer initTwinBuffer(twinBuffer *B, FILE *fp, int bufferSize);

#endif /* LEXER_H */
