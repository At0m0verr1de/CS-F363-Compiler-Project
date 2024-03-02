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
#include "dict.h"
#include "lexerDef.h"

// Function declarations
TokenInfo getNextToken(twinBuffer *B, FILE *fp, struct Dictionary *dict);
char getNextChar(twinBuffer *B);
void initTwinBuffer(twinBuffer *B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
void lexer(FILE *fp);
TokenInfo readTokenFromFile(FILE *file);
void writeTokenToFile(FILE *file, TokenInfo token);