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
TokenInfo getNextToken(twinBuffer *B, FILE *fp, struct Dictionary *dict)
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
            else if (isdigit(currentChar))
            {
                state = 51;
            }
            else if (currentChar >= 'b' && currentChar <= 'd')
            {
                state = 44;
            }
            else if (currentChar == 'a' || (currentChar >= 'e' && currentChar <= 'z'))
            {
                state = 45;
            }
            else if (currentChar == '<')
            {
                state = 38;
            }
            else
            {
                state = -1;
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
            else
            {
                state = -1;
            }
            break;

        case 5:
            if (currentChar == '@')
            {
                strcat(token.type, "TK_OR");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                state = -1;
            }
            break;

        case 7:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_NE");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                state = -1;
            }
            break;

        case 9:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_EQ");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                state = -1;
            }
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
            else
            {
                state = -1;
            }
            break;

        case 27:
            if (currentChar == '&')
            {
                strcat(token.type, "TK_AND");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                state = -1;
            }
            break;

        case 29:
            if (isalpha(currentChar) && tolower(currentChar) == currentChar)
            {
                state = 30;
            }
            else
            {
                state = -1;
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
            else
            {
                state = -1;
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
                // @aadeesh check code
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;

                char *value = search(dict, token.lexeme);

                if (value)
                {
                    strcat(token.type, value);
                }
                else if (strlen(token.lexeme) < 30)
                {
                    strcat(token.type, "TK_FUNID");
                }
                else
                {
                    printf("Line No %d: Error: Function Identifier is longer than the prescribed length of 30 characters.\n", B->lineNumber);
                    strcat(token.type, "TK_ERROR");
                }

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
                // @aadeesh check code
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;

                char *value = search(dict, token.lexeme);

                if (value)
                {
                    strcat(token.type, value);
                }
                else if (strlen(token.lexeme) < 30)
                {
                    strcat(token.type, "TK_FUNID");
                }
                else
                {
                    printf("Line No %d: Error: Function Identifier is longer than the prescribed length of 30 characters.\n", B->lineNumber);
                    strcat(token.type, "TK_ERROR");
                }

                return token;
            }
            break;

        case 38:
            if (currentChar == '=')
            {
                strcat(token.type, "TK_LE");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '-')
            {
                state = 39;
            }
            else
            {
                // retract and return TK_LT
                strcat(token.type, "TK_LT");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;
            }
            break;

        case 39:
            if (currentChar == '-')
            {
                state = 40;
            }
            else
            {
                // double retract and return TK_LT
                strcat(token.type, "TK_LT");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition -= 2;
                return token;
            }
            break;

        case 40:
            if (currentChar == '-')
            {
                strcat(token.type, "TK_ASSINGOP");
                return token;
            }
            else
            {
                state = -1;
            }
            break;

        case 44:
            if (islower(currentChar))
            {
                state = 45;
            }
            else if (currentChar >= '2' && currentChar <= '7')
            {
                state = 47;
            }
            else
            {
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                char *value = search(dict, token.lexeme);
                if (value)
                {
                    strcat(token.type, value);
                }
                else if (strlen(token.lexeme) < 20)
                {
                    strcat(token.type, "TK_ID");
                }
                else
                {
                    printf("Line No %d: Error: Variable Identifier is longer than the prescribed length of 20 characters.\n", B->lineNumber);
                    strcat(token.type, "TK_ERROR");
                }
                return token;
            }
            break;

        case 45:
            if (islower(currentChar))
            {
                state = 45;
            }
            else
            {
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                char *value = search(dict, token.lexeme);
                if (value)
                {
                    strcat(token.type, value);
                }
                else if (strlen(token.lexeme) < 20)
                {
                    strcat(token.type, "TK_ID");
                }
                else
                {
                    printf("Line No %d: Error: Variable Identifier is longer than the prescribed length of 20 characters.\n", B->lineNumber);
                    strcat(token.type, "TK_ERROR");
                }
                return token;
            }
            break;

        case 47:
            if (currentChar >= 'b' && currentChar <= 'd')
            {
                state = 47;
            }
            else if (currentChar >= '2' && currentChar <= '7')
            {
                state = 48;
            }
            else
            {
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                strcat(token.type, "TK_ID");
                return token;
            }
            break;

        case 48:
            if (currentChar >= '2' && currentChar <= '7')
            {
                state = 48;
            }
            else
            {
                // retract
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;

                if (strlen(token.lexeme) > 20)
                {
                    printf("Line No %d: Error: Variable Identifier is longer than the prescribed length of 20 characters.\n", B->lineNumber);
                    strcat(token.type, "TK_ERROR");
                }
                else
                {
                    strcat(token.type, "TK_ID");
                }
                return token;
            }
            break;

        case 51:
            if (isdigit(currentChar))
            {
                state = 51;
            }
            else if (currentChar == '.')
            {
                state = 53;
            }
            else
            {
                // retract
                strcat(token.type, "TK_NUM");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;
            }
            break;

        case 53:
            if (isdigit(currentChar))
            {
                state = 54;
            }
            else
            {
                // double retract and return TK_NUM
                strcat(token.type, "TK_NUM");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition -= 2;
                return token;
            }
            break;

        case 54:
            if (isdigit(currentChar))
            {
                state = 57;
            }
            else
            {
                state = -1;
            }
            break;

        case 57:
            if (currentChar == 'E')
            {
                state = 58;
            }
            else
            {
                // retract and return TK_RNUM
                strcat(token.type, "TK_RNUM");
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                if (currentChar != '\0')
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                }
                B->currentPosition--;
                return token;
            }
            break;

        case 58:
            if (currentChar == '+' || currentChar == '-')
            {
                state = 59;
            }
            else if (isdigit(currentChar))
            {
                state = 60;
            }
            else
            {
                state = -1;
            }
            break;

        case 59:
            if (isdigit(currentChar))
            {
                state = 60;
            }
            else
            {
                state = -1;
            }
            break;

        case 60:
            if (isdigit(currentChar))
            {
                return token;
            }
            else
            {
                state = -1;
            }
            break;

        case -1:
            if (strlen(token.lexeme) == 2)
            {
                token.lexeme[strlen(token.lexeme) - 1] = '\0';
                // single retraction and return error
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                B->currentPosition--;
                printf("Line No %d : Error: Unknown Symbol <%s>\n", B->lineNumber, token.lexeme);
                strcat(token.type, "TK_ERROR");
                return token;
            }
            else
            {
                // double retract and report error
                token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
                token.lexeme[strlen(token.lexeme) - 2] = '\0';

                B->currentPosition -= 2;

                strcat(token.type, "TK_ERROR");

                printf("Line no: %d : Error: Unknown Pattern <%s>\n", B->lineNumber, token.lexeme);
                return token;
            }
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
        if (!(strcmp(token.type, "TK_ERROR")) || !(strcmp(token.type, "TK_COMMENT")))
            continue;
        printf("Line no. %d     Lexeme %s     Token %s\n", token.lineNumber, token.lexeme, token.type);
    } while (!token.end);

    fclose(fp);
    free(B);
}

int main(int argc, char *argv[])
{
    // Open source code file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1; // Return non-zero exit code indicating error
    }
    lexer(fp);
    fclose(fp);
    return 0;
}
