#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dict.h"
#include "lexerDef.h"

// Function declarations
TokenInfo getNextToken(twinBuffer *B, FILE *fp, struct Dictionary *dict);
char getNextChar(twinBuffer *B);
void initTwinBuffer(twinBuffer *B, FILE *fp);
// void removeComments(char *testcaseFile, char *cleanFile);
