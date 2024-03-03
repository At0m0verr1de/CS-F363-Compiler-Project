/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/

#include "lexer.h"

// -------------------------------- LookUp Table Functions -------------------------------------

int hash(char *str)
{
    char asso_values[] =
        {
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 20, 50,
            0, 10, 0, 0, 25, 20, 20, 50, 10, 5,
            5, 0, 50, 50, 10, 15, 20, 50, 0, 50,
            50, 50, 10, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50};
    int hashValue = strlen(str) + asso_values[(unsigned char)str[1] + 1] + asso_values[(unsigned char)str[strlen(str) - 1]];
    printf("%d\n", hashValue % TABLE_SIZE);
    return hashValue % TABLE_SIZE;
}

struct Dictionary *createDictionary()
{
    struct Dictionary *dict = (struct Dictionary *)malloc(sizeof(struct Dictionary));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        dict->table[i] = NULL;
    }
    return dict;
}

void insert(struct Dictionary *dict, char *key, char *value)
{
    int index = hash(key);
    struct KeyValuePair *newPair = (struct KeyValuePair *)malloc(sizeof(struct KeyValuePair));
    newPair->key = strdup(key); // strdup dynamically allocates memory for the string
    newPair->value = value;
    dict->table[index] = newPair;
}

char *search(struct Dictionary *dict, char *key)
{
    // if (strlen(key) <=10 && strlen(key) >= 2){
    int index = hash(key);
    //} else return NULL:
    if (dict->table[index] != NULL && strcmp(dict->table[index]->key, key) == 0)
    {
        return dict->table[index]->value;
    }
    return NULL; // Key not found
}

struct Dictionary *initLookupTable()
{
    struct Dictionary *dict = createDictionary();

    // Insert default key-value pairs
    insert(dict, "call", "TK_CALL");
    insert(dict, "record", "TK_RECORD");
    insert(dict, "endrecord", "TK_ENDRECORD");
    insert(dict, "else", "TK_ELSE");
    insert(dict, "parameters", "TK_PARAMETERS");
    insert(dict, "with", "TK_WITH");
    insert(dict, "end", "TK_END");
    insert(dict, "while", "TK_WHILE");
    insert(dict, "union", "TK_UNION");
    insert(dict, "endunion", "TK_ENDUNION");
    insert(dict, "as", "TK_AS");
    insert(dict, "type", "TK_TYPE");
    insert(dict, "_main", "TK_MAIN");
    insert(dict, "definetype", "TK_DEFINETYPE");
    insert(dict, "global", "TK_GLOBAL");
    insert(dict, "parameter", "TK_PARAMETER");
    insert(dict, "list", "TK_LIST");
    insert(dict, "input", "TK_INPUT");
    insert(dict, "output", "TK_OUTPUT");
    insert(dict, "int", "TK_INT");
    insert(dict, "real", "TK_REAL");
    insert(dict, "endwhile", "TK_ENDWHILE");
    insert(dict, "if", "TK_IF");
    insert(dict, "then", "TK_THEN");
    insert(dict, "endif", "TK_ENDIF");
    insert(dict, "read", "TK_READ");
    insert(dict, "write", "TK_WRITE");
    insert(dict, "return", "TK_RETURN");

    return dict;
}

// -------------------------------- Lexer Functions -------------------------------------

char getNextChar(twinBuffer *B)
{
    // Check if currentPosition exceeds bufferSize
    if (B->currentPosition >= MAX_BUFFER_SIZE)
    {
        // Switch buffer
        B->currentBuffer = (B->currentBuffer + 1) % 2;
        size_t bytes;
        // Refill buffer from file
        if (!B->dblret)
        {
            bytes = fread(B->currentBuffer == 0 ? B->buffer0 : B->buffer1, sizeof(char), MAX_BUFFER_SIZE, B->fp);
            B->dblret = false;
        }
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
        B->dblret = true; // Fixes Retraction at start of buffer
        // fseek(B->fp, -1 * MAX_BUFFER_SIZE, SEEK_CUR);
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
    B->dblret = false;

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

        // debug
        // printf("Current Char: %c     Lexeme: %s\n", currentChar, token.lexeme);

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
                strcat(token.type, "TK_ERROR");
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
                strcat(token.type, "TK_ERROR");
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
                strcat(token.type, "TK_ERROR");
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
                strcat(token.type, "TK_ERROR");
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
                strcat(token.type, "TK_ERROR");
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
            // &
            if (currentChar == '&')
            {
                // &&
                state = 27;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 27:
            // &&
            if (currentChar == '&')
            {
                // &&&
                strcat(token.type, "TK_AND");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 29:
            // #
            if (isalpha(currentChar) && tolower(currentChar) == currentChar)
            {
                // #[a-z]
                state = 30;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 30:
            // #[a-z]
            if (isalpha(currentChar) && tolower(currentChar) == currentChar)
            {
                // #[a-z]*
                state = 30;
            }
            else
            {
                // (#[a-z]* + otherChar) and retract
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
            // _
            if (isalpha(currentChar))
            {
                // _[a-z|A-Z]
                state = 33;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 33:
            // _[a-z|A-Z]
            if (isalpha(currentChar))
            {
                // _[a-z|A-Z]*
                state = 33;
            }
            else if (isdigit(currentChar))
            {
                // _[a-z|A-Z]*[0-9]
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

        case 34:
            // _[a-z|A-Z]*[0-9]
            if (isdigit(currentChar))
            {
                // _[a-z|A-Z]*[0-9]*
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
            // <
            if (currentChar == '=')
            {
                // <=
                strcat(token.type, "TK_LE");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else if (currentChar == '-')
            {
                // <-
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
                    token.lexeme[strlen(token.lexeme) - 2] = '\0';
                else
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                    token.end = false;
                }

                B->currentPosition -= 2;
                return token;
            }
            break;

        case 40:
            // <--
            if (currentChar == '-')
            {
                strcat(token.type, "TK_ASSIGNOP");
                token.lineNumber = B->lineNumber;
                return token;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
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

                // debug
                // printf("Value: %s\n", value);
                if (value)
                {
                    strcat(token.type, value);
                }
                else
                {
                    strcat(token.type, "TK_FIELDID");
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

                // debug
                // printf("Value: %s\n", value);
                if (value)
                {
                    strcat(token.type, value);
                }
                else
                {
                    strcat(token.type, "TK_FIELDID");
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
                    token.lexeme[strlen(token.lexeme) - 2] = '\0';
                else
                {
                    token.lexeme[strlen(token.lexeme) - 1] = '\0';
                    token.end = false;
                }

                B->currentPosition -= 2;
                // printf("Retracted Twice");
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
                strcat(token.type, "TK_ERROR");
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
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 59:
            if (isdigit(currentChar))
            {
                state = 60;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;

        case 60:
            if (isdigit(currentChar))
            {
                strcat(token.type, "TK_RNUM");
                return token;
            }
            else
            {
                strcat(token.type, "TK_ERROR");
            }
            break;
        }

        if (strlen(token.lexeme) == 1 && !strcmp(token.type, "TK_ERROR"))
        {
            printf("Line No %d : Error: Unknown Symbol <%s>\n", B->lineNumber, token.lexeme);
            return token;
        }
        else if (!strcmp(token.type, "TK_ERROR"))
        {
            token.lineNumber = (token.lexeme[strlen(token.lexeme) - 1] == '\n') ? --B->lineNumber : B->lineNumber;
            if (currentChar != '\0')
            {
                token.lexeme[strlen(token.lexeme) - 1] = '\0';
            }
            // single retraction and return error
            B->currentPosition--;
            printf("Line No. %d : Error: Unknown Pattern <%s>\n", B->lineNumber, token.lexeme);
            return token;
        }
    }
    return token;
}

void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE *inputFile, *outputFile;
    char currentChar, nextChar;

    // Open source code file for reading
    if ((inputFile = fopen(testcaseFile, "r")) == NULL)
    {
        perror("Error opening input file");
        return;
    }

    // Open clean code file for writing
    if ((outputFile = fopen(cleanFile, "w")) == NULL)
    {
        perror("Error opening output file");
        fclose(inputFile);
        return;
    }

    // Process each character in the input file
    while ((currentChar = fgetc(inputFile)) != '\0')
    {
        // Check for start of single-line comment
        if (currentChar == '/' && (nextChar = fgetc(inputFile)) == '/')
        {
            // Skip until end of line
            while ((currentChar = fgetc(inputFile)) != '\n' && currentChar != '\0')
                ;
            // Write newline character to output file
            fputc('\n', outputFile);
        }
        // Write non-comment characters to output file
        else
        {
            fputc(currentChar, outputFile);
        }
    }
}

// Function to write a token to a file
void writeTokenToFile(FILE *file, TokenInfo token)
{
    fprintf(file, "%s %s %d %d\n", token.type, token.lexeme, token.lineNumber, token.end ? 1 : 0);
}

// Function to read a token from a file
TokenInfo readTokenFromFile(FILE *file)
{
    TokenInfo token;
    fscanf(file, "%s %s %d %d", token.type, token.lexeme, &token.lineNumber, &token.end);
    return token;
}

// Main Tokenizer Function
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
        if (!strcmp(token.type, "TK_ERROR"))
            continue;
        else if (!strcmp(token.type, "TK_COMMENT"))
        {
            // create a file to write tokens to
            FILE *tk = fopen("tokens.txt", "w");
            writeTokenToFile(tk, token);
            printf("Line no. %d     Lexeme %%      Token %s\n", token.lineNumber, token.type);
            continue;
        }
        printf("Line no. %d     Lexeme %s      Token %s\n", token.lineNumber, token.lexeme, token.type);
    } while (!token.end);

    fclose(fp);
    free(B);
}
