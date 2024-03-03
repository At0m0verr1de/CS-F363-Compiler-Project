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
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Define necessary constants and structures
#define MAX_LEXEME_SIZE 100
#define MAX_TOKEN_SIZE 15
#define MAX_BUFFER_SIZE 5
#define TABLE_SIZE 50

typedef struct
{
    char type[MAX_TOKEN_SIZE];
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
    bool end;
} TokenInfo;

typedef struct
{
    char buffer0[MAX_BUFFER_SIZE];
    char buffer1[MAX_BUFFER_SIZE];
    int currentBuffer;   // Indicates the currently active buffer (0 or 1)
    int currentPosition; // Current position in the currently active buffer
    int lineNumber;
    FILE *fp;    // File pointer for input stream
    bool dblret; // double retraction flag
} twinBuffer;

struct KeyValuePair
{
    char *key;
    char *value;
    int len;
};

struct Dictionary
{
    struct KeyValuePair *table[TABLE_SIZE];
};
