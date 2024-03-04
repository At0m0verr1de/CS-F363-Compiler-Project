/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/

#include "lexerDef.h"

// Function declarations
TokenInfo getNextToken(twinBuffer *B, FILE *fp, DictionaryLexer *dict);
char getNextChar(twinBuffer *B);
void initTwinBuffer(twinBuffer *B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
void lexer(FILE *fp);
TokenInfo readTokenFromFile(FILE *file);
void writeTokenToFile(FILE *file, TokenInfo token);

int hashLT(char *key);
DictionaryLexer *createDictionaryLexer();
void insertLexer(DictionaryLexer *dict, char *key, char *value);
char *search(DictionaryLexer *dict, char *key);
DictionaryLexer *initLookupTable();
void freeDictionaryLexer(DictionaryLexer *dict);