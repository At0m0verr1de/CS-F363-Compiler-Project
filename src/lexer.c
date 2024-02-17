// ---------------------------------------- Header Files -----------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"
#include "../include/dict.h"

char getNextChar(twinBuffer *B)
{
    // Check if currentPosition exceeds bufferSize
    if (B->currentPosition >= MAX_BUFFER_SIZE)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;

        // Refill buffer from file
        size_t bytes = fread(B->currentBuffer == 0 ? B->buffer0 : B->buffer1, sizeof(char), MAX_BUFFER_SIZE, B->fp);
        if (bytes < MAX_BUFFER_SIZE)
        {
            if (B->currentBuffer == 0)
            {
                B->buffer0[bytes] = '\0';
            }
            else
            {
                B->buffer1[bytes] = '\0';
            }
        }
        // Reset currentPosition
        B->currentPosition = B->currentPosition % MAX_BUFFER_SIZE;
    }
    else if (B->currentPosition < 0)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;
        // Reset currentPosition
        B->currentPosition = (B->currentPosition + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
    }
    // Return next character from the current buffer
    return B->currentBuffer == 0 ? B->buffer0[B->currentPosition++] : B->buffer1[B->currentPosition++];
}

void initTwinBuffer(twinBuffer *B, FILE *fp)
{
    B->fp = fp;
    B->currentPosition = 0;
    B->currentBuffer = 0;
    B->lineNumber = 1;

    memset(B->buffer0, '\0', MAX_BUFFER_SIZE);
    memset(B->buffer1, '\0', MAX_BUFFER_SIZE);

    // Fill buffers from file
    fread(B->buffer0, sizeof(char), MAX_BUFFER_SIZE, fp);
}

// Function to get next token
TokenInfo getNextToken(twinBuffer *B, FILE *fp, struct Dictionary *lookup)
{
    int state = 0;
    TokenInfo token;
    token.end = false;
    token.lexeme[0] = '\0';
    token.type[0] = '\0';
    char currentChar;

    while (1)
    {
        currentChar = getNextChar(B);
        if (currentChar == '\0')
            token.end = true;
        else if (currentChar == '\n')
            B->lineNumber++;
        strncat(token.lexeme, &currentChar, 1);

        switch (state)
        {
        case 0: // START STATE
            if (currentChar == '\0')
            {
                // strcat(token.type, "TK_EOF");
                // token.lineNumber = B->lineNumber;
                return token;
            }

            else if (currentChar == '\n' || currentChar == '\t' || currentChar == ' ')
            {
                // is state 37 being handled?
                token.lexeme[0] = '\0';
            }

            else if (currentChar == '[')
            {
                strcat(token.type, "TK_SQL");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == ']')
            {
                strcat(token.type, "TK_SQR");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == ',')
            {
                strcat(token.type, "TK_COMMA");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == ';')
            {
                strcat(token.type, "TK_SEM");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == ':')
            {
                strcat(token.type, "TK_COLON");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '.')
            {
                strcat(token.type, "TK_DOT");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '(')
            {
                strcat(token.type, "TK_OP");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == ')')
            {
                strcat(token.type, "TK_CL");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '+')
            {
                strcat(token.type, "TK_PLUS");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '-')
            {
                strcat(token.type, "TK_MINUS");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '*')
            {
                strcat(token.type, "TK_MUL");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '/')
            {
                strcat(token.type, "TK_DIV");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '~')
            {
                strcat(token.type, "TK_NOT");
                token.lineNumber = B->lineNumber;
                return token;
            }

            else if (currentChar == '>')
            {
                state = 1;
            }
            else if (currentChar == '@')
            {
                state = 4;
            }
            else if (currentChar == '!')
            {
                state = 7;
            }
            else if (currentChar == '=')
            {
                state = 9;
            }
            else if (currentChar == '%')
            {
                state = 24;
            }
            else if (currentChar == '&')
            {
                state = 26;
            }
            else if (currentChar == '#')
            {
                state = 29;
            }
            else if (currentChar == '_')
            {
                state = 32;
            }
            break;

        case 1:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_GE");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            { // Retraction State Code
                strcat(token.type, "TK_GT");
                token.lineNumber = (currentChar == '\n') ? --B->lineNumber : B->lineNumber;
                token.lexeme[0] = '>';
                token.lexeme[1] = '\0';
                B->currentPosition--;
                return token;
            }
            break;

        case 4:
            if (currentChar == '@')
            {
                state = 5;
            }
            break;

        case 5:
            if (currentChar == '@')
            {
                strcat(token.type, "TK_OR");
                token.lineNumber = B->lineNumber;
                return token;
            }
            break;

        case 7:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_NE");
                token.lineNumber = B->lineNumber;
                return token;
            }
            break;

        case 9:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_EQ");
                token.lineNumber = B->lineNumber;
                return token;
            }
            break;

        case 24:
            if (currentChar == '\n')
            {
                strcat(token.type, "TK_COMMENT");
                token.lexeme[strlen(token.lexeme) - 1] = '\0';
                token.lineNumber = B->lineNumber - 1;
                return token;
            }
            else if (currentChar == '\0')
            {
                strcat(token.type, "TK_COMMENT");
                token.lineNumber = B->lineNumber;
                B->currentPosition--;
                return token;
            }
            break;

        case 26:
            if (currentChar == '&')
            {
                state = 27;
            }
            break;

        case 27:
            if (currentChar == '&')
            {
                strcat(token.type, "TK_AND");
                token.lineNumber = B->lineNumber;
                return token;
            }
            break;

        case 29:
            if (isalpha(currentChar) && tolower(currentChar) == currentChar)
            {
                state = 30;
            }
            break;

        case 30:
            if (isalpha(currentChar) && tolower(currentChar) == currentChar)
            {
                state = 30;
            }
            else
            {
                strcat(token.type, "TK_RUID");
                token.lineNumber = (currentChar == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;
            }
            break;

        case 32:
            if (isalpha(currentChar))
            {
                state = 33;
            }
            break;

        case 33:
            if (isalpha(currentChar))
            {
                state = 33;
            }
            else if (isdigit(currentChar))
            {
                state = 34;
            }
            else
            {
                // retract
                strcat(token.type, "TK_FUNID");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;

                // _main lookup
                // size constraint 30 ERROR
            }
            break;

        case 34:
            if (isdigit(currentChar))
            {
                state = 34;
            }
            else
            {
                // retract
                strcat(token.type, "TK_FUNID");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;

                // size constraint 30 ERROR
            }
            break;
        }
    }
}

// ---------------------------------------- Main Function -----------------------------------------

void lexer(FILE *fp)
{
    // Open source code file
    if (fp == NULL)
    {
        perror("Error opening file");
    }

    struct Dictionary *dict = initLookupTable();

    twinBuffer *B = (twinBuffer *)malloc(sizeof(twinBuffer));
    initTwinBuffer(B, fp);

    // Get next token and process
    TokenInfo token;
    do
    {
        token = getNextToken(B, fp, dict);
        if (token.lexeme[0] == '\0')
            break;
        printf("Line no. %d     Lexeme %s     Token %s\n", token.lineNumber, token.lexeme, token.type);
    } while (!token.end);

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

// Line No 6 : Error: Unknown Symbol <|>
// Line no: 6 : Error: Unknown pattern <&&>
// Line No 8: Error :Variable Identifier is longer than the prescribed length of 20 characters.
// Line no: 9 : Error: Unknown pattern <5000.7>
