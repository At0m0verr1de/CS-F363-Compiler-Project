

#include "parser.h"

int hashLT(char *str)
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
    return hashValue % TABLE_SIZE;
}

DictionaryLexer *createDictionaryLexer()
{
    DictionaryLexer *dict = (DictionaryLexer *)malloc(sizeof(DictionaryLexer));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        dict->table[i] = NULL;
    }
    return dict;
}

void insertLexer(DictionaryLexer *dict, char *key, char *value)
{
    int index = hashLT(key);
    struct KeyValuePair *newPair = (struct KeyValuePair *)malloc(sizeof(struct KeyValuePair));
    newPair->key = strdup(key); // strdup dynamically allocates memory for the string
    newPair->value = value;
    dict->table[index] = newPair;
}

char *search(DictionaryLexer *dict, char *key)
{
    // if (strlen(key) <=10 && strlen(key) >= 2){
    int index = hashLT(key);
    //} else return NULL:
    if (dict->table[index] != NULL && strcmp(dict->table[index]->key, key) == 0)
    {
        return dict->table[index]->value;
    }
    return NULL; // Key not found
}

DictionaryLexer *initLookupTable()
{
    DictionaryLexer *dict = createDictionaryLexer();

    // Insert default key-value pairs
    insertLexer(dict, "call", "TK_CALL");
    insertLexer(dict, "record", "TK_RECORD");
    insertLexer(dict, "endrecord", "TK_ENDRECORD");
    insertLexer(dict, "else", "TK_ELSE");
    insertLexer(dict, "parameters", "TK_PARAMETERS");
    insertLexer(dict, "with", "TK_WITH");
    insertLexer(dict, "end", "TK_END");
    insertLexer(dict, "while", "TK_WHILE");
    insertLexer(dict, "union", "TK_UNION");
    insertLexer(dict, "endunion", "TK_ENDUNION");
    insertLexer(dict, "as", "TK_AS");
    insertLexer(dict, "type", "TK_TYPE");
    insertLexer(dict, "_main", "TK_MAIN");
    insertLexer(dict, "definetype", "TK_DEFINETYPE");
    insertLexer(dict, "global", "TK_GLOBAL");
    insertLexer(dict, "parameter", "TK_PARAMETER");
    insertLexer(dict, "list", "TK_LIST");
    insertLexer(dict, "input", "TK_INPUT");
    insertLexer(dict, "output", "TK_OUTPUT");
    insertLexer(dict, "int", "TK_INT");
    insertLexer(dict, "real", "TK_REAL");
    insertLexer(dict, "endwhile", "TK_ENDWHILE");
    insertLexer(dict, "if", "TK_IF");
    insertLexer(dict, "then", "TK_THEN");
    insertLexer(dict, "endif", "TK_ENDIF");
    insertLexer(dict, "read", "TK_READ");
    insertLexer(dict, "write", "TK_WRITE");
    insertLexer(dict, "return", "TK_RETURN");

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
TokenInfo getNextToken(twinBuffer *B, FILE *fp, DictionaryLexer *dict)
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
                    printf("Line %d Error: Function Identifier is longer than the prescribed length of 30 characters.\n", B->lineNumber);
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
                    printf("Line %d Error: Function Identifier is longer than the prescribed length of 30 characters.\n", B->lineNumber);
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
                    printf("Line %d Error: Variable Identifier is longer than the prescribed length of 20 characters.\n", B->lineNumber);
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
                    printf("Line %d Error: Variable Identifier is longer than the prescribed length of 20 characters.\n", B->lineNumber);
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
            printf("Line %d Error: Unknown Symbol <%s>\n", B->lineNumber, token.lexeme);
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
            printf("Line %d Error: Unknown Pattern <%s>\n", B->lineNumber, token.lexeme);
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
    while ((currentChar = fgetc(inputFile)) != EOF)
    {
        // Check for start of single-line comment
        if (currentChar == '%')
        {
            // Skip until end of line
            while ((currentChar = fgetc(inputFile)) != '\n' && currentChar != '\0')
                ;
        }
        // Write non-comment characters to output file
        else
        {
            fputc(currentChar, outputFile);
        }
    }
    fclose(inputFile);
    fclose(outputFile);
}

DictionaryParser *createDictionaryParser()
{
    DictionaryParser *dict = (DictionaryParser *)malloc(sizeof(DictionaryParser));
    for (int i = 0; i < FAF_TABLE_SIZE; i++)
    {
        dict->table[i] = NULL;
    }
    return dict;
}

int hashNT(char *str)
{
    char asso_values[] =
        {
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 0, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 0, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 30, 5, 26,
            25, 0, 40, 5, 5, 0, 83, 83, 60, 10,
            55, 0, 0, 83, 5, 35, 20, 0, 5, 83,
            83, 40, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
            83, 83, 83, 83, 83, 83};
    int hval = strlen(str);

    switch (hval)
    {
    default:
        hval += asso_values[(unsigned char)str[1]];
    
    case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }

    if (!strcmp(str, "term\0"))
        hval = 23;
    if (!strcmp(str, "primitiveDatatype\0"))
        hval = 21;
    if (!strcmp(str, "arithmeticExpression\0"))
        hval = 57;
    if (!strcmp(str, "expPrime\0"))
        hval = 81;
    if (!strcmp(str, "ε\0"))
        hval = 83;
    return hval;
}

void insertParser(DictionaryParser *dict, char *key, char **value, int len)
{
    int index = hashNT(key);
    struct KeyValuePairParser *newPair = (struct KeyValuePairParser *)malloc(sizeof(struct KeyValuePairParser));
    newPair->key = strdup(key); // strdup dynamically allocates memory for the string
    newPair->value = value;
    newPair->len = len;
    // printf("%d %s %u\n", bleh++ ,key , index);

    dict->table[index] = newPair;
}

void initFirst(DictionaryParser *dict)
{

    // Insert key-value pairs
    char **program = (char **)malloc(2 * sizeof(char *));
    program[0] = strdup("TK_MAIN");
    program[1] = strdup("TK_FUNID");
    insertParser(dict, strdup("program"), program, 2);

    // mainFunction
    char **mainFunction = (char **)malloc(1 * sizeof(char *));
    mainFunction[0] = strdup("TK_MAIN");
    insertParser(dict, strdup("mainFunction"), mainFunction, 1);

    // otherFunctions
    char **otherFunctions = (char **)malloc(2 * sizeof(char *));
    otherFunctions[0] = strdup("TK_FUNID");
    otherFunctions[1] = strdup("ε");
    insertParser(dict, strdup("otherFunctions"), otherFunctions, 2);

    // function
    char **function = (char **)malloc(1 * sizeof(char *));
    function[0] = strdup("TK_FUNID");
    insertParser(dict, strdup("function"), function, 1);

    // input_par
    char **input_par = (char **)malloc(1 * sizeof(char *));
    input_par[0] = strdup("TK_INPUT");
    insertParser(dict, strdup("input_par"), input_par, 1);

    // output_par
    char **output_par = (char **)malloc(2 * sizeof(char *));
    output_par[0] = strdup("TK_OUTPUT");
    output_par[1] = strdup("ε");
    insertParser(dict, strdup("output_par"), output_par, 2);

    // parameter_list
    char **parameter_list = (char **)malloc(5 * sizeof(char *));
    parameter_list[0] = strdup("TK_INT");
    parameter_list[1] = strdup("TK_REAL");
    parameter_list[2] = strdup("TK_RECORD");
    parameter_list[3] = strdup("TK_RUID");
    parameter_list[4] = strdup("TK_UNION");
    insertParser(dict, strdup("parameter_list"), parameter_list, 5);

    // dataType
    char **dataType = (char **)malloc(5 * sizeof(char *));
    dataType[0] = strdup("TK_INT");
    dataType[1] = strdup("TK_REAL");
    dataType[2] = strdup("TK_RECORD");
    dataType[3] = strdup("TK_RUID");
    dataType[4] = strdup("TK_UNION");
    insertParser(dict, strdup("dataType"), dataType, 5);

    // primitiveDatatype
    char **primitiveDatatype = (char **)malloc(2 * sizeof(char *));
    primitiveDatatype[0] = strdup("TK_INT");
    primitiveDatatype[1] = strdup("TK_REAL");
    insertParser(dict, strdup("primitiveDatatype"), primitiveDatatype, 2);

    // constructedDatatype
    char **constructedDatatype = (char **)malloc(3 * sizeof(char *));
    constructedDatatype[0] = strdup("TK_RECORD");
    constructedDatatype[1] = strdup("TK_RUID");
    constructedDatatype[2] = strdup("TK_UNION");
    insertParser(dict, strdup("constructedDatatype"), constructedDatatype, 3);

    // remaining_list
    char **remaining_list = (char **)malloc(2 * sizeof(char *));
    remaining_list[0] = strdup("TK_COMMA");
    remaining_list[1] = strdup("ε");
    insertParser(dict, strdup("remaining_list"), remaining_list, 2);

    // stmts
    char **stmts = (char **)malloc(12 * sizeof(char *));
    stmts[0] = strdup("TK_SQL");
    stmts[1] = strdup("TK_ID");
    stmts[2] = strdup("TK_RECORD");
    stmts[3] = strdup("TK_UNION");
    stmts[4] = strdup("TK_TYPE");
    stmts[5] = strdup("TK_CALL");
    stmts[6] = strdup("TK_WHILE");
    stmts[7] = strdup("TK_IF");
    stmts[8] = strdup("TK_READ");
    stmts[9] = strdup("TK_WRITE");
    stmts[10] = strdup("TK_RETURN");
    stmts[11] = strdup("TK_DEFINETYPE");
    insertParser(dict, strdup("stmts"), stmts, 12);

    // typeDefinitions
    char **typeDefinitions = (char **)malloc(4 * sizeof(char *));
    typeDefinitions[0] = strdup("TK_RECORD");
    typeDefinitions[1] = strdup("TK_UNION");
    typeDefinitions[2] = strdup("TK_DEFINETYPE");
    typeDefinitions[3] = strdup("ε");
    insertParser(dict, strdup("typeDefinitions"), typeDefinitions, 4);

    // actualOrRedefined
    char **actualOrRedefined = (char **)malloc(3 * sizeof(char *));
    actualOrRedefined[0] = strdup("TK_RECORD");
    actualOrRedefined[1] = strdup("TK_UNION");
    actualOrRedefined[2] = strdup("TK_DEFINETYPE");
    insertParser(dict, strdup("actualOrRedefined"), actualOrRedefined, 3);

    // typeDefinition
    char **typeDefinition = (char **)malloc(2 * sizeof(char *));
    typeDefinition[0] = strdup("TK_RECORD");
    typeDefinition[1] = strdup("TK_UNION");
    insertParser(dict, strdup("typeDefinition"), typeDefinition, 2);

    // fieldDefinitions
    char **fieldDefinitions = (char **)malloc(1 * sizeof(char *));
    fieldDefinitions[0] = strdup("TK_TYPE");
    insertParser(dict, strdup("fieldDefinitions"), fieldDefinitions, 1);

    // fieldDefinition
    char **fieldDefinition = (char **)malloc(1 * sizeof(char *));
    fieldDefinition[0] = strdup("TK_TYPE");
    insertParser(dict, strdup("fieldDefinition"), fieldDefinition, 1);

    // fieldType
    char **fieldType = (char **)malloc(5 * sizeof(char *));
    fieldType[0] = strdup("TK_INT");
    fieldType[1] = strdup("TK_REAL");
    fieldType[2] = strdup("TK_RUID");
    fieldType[3] = strdup("TK_RECORD");
    fieldType[4] = strdup("TK_UNION");
    insertParser(dict, strdup("fieldType"), fieldType, 5);

    // moreFields
    char **moreFields = (char **)malloc(2 * sizeof(char *));
    moreFields[0] = strdup("TK_TYPE");
    moreFields[1] = strdup("ε");
    insertParser(dict, strdup("moreFields"), moreFields, 2);

    // declarations
    char **declarations = (char **)malloc(2 * sizeof(char *));
    declarations[0] = strdup("TK_TYPE");
    declarations[1] = strdup("ε");
    insertParser(dict, strdup("declarations"), declarations, 2);

    // declaration
    char **declaration = (char **)malloc(1 * sizeof(char *));
    declaration[0] = strdup("TK_TYPE");
    insertParser(dict, strdup("declaration"), declaration, 1);

    // global_or_not
    char **global_or_not = (char **)malloc(2 * sizeof(char *));
    global_or_not[0] = strdup("TK_COLON");
    global_or_not[1] = strdup("ε");
    insertParser(dict, strdup("global_or_not"), global_or_not, 2);

    // otherStmts
    char **otherStmts = (char **)malloc(8 * sizeof(char *));
    otherStmts[0] = strdup("TK_SQL");
    otherStmts[1] = strdup("TK_ID");
    otherStmts[2] = strdup("TK_CALL");
    otherStmts[3] = strdup("TK_WHILE");
    otherStmts[4] = strdup("TK_IF");
    otherStmts[5] = strdup("TK_READ");
    otherStmts[6] = strdup("TK_WRITE");
    otherStmts[7] = strdup("ε");
    insertParser(dict, strdup("otherStmts"), otherStmts, 8);

    // stmt
    char **stmt = (char **)malloc(7 * sizeof(char *));
    stmt[0] = strdup("TK_SQL");
    stmt[1] = strdup("TK_ID");
    stmt[2] = strdup("TK_CALL");
    stmt[3] = strdup("TK_WHILE");
    stmt[4] = strdup("TK_IF");
    stmt[5] = strdup("TK_READ");
    stmt[6] = strdup("TK_WRITE");
    insertParser(dict, strdup("stmt"), stmt, 7);

    // assignmentStmt
    char **assignmentStmt = (char **)malloc(1 * sizeof(char *));
    assignmentStmt[0] = strdup("TK_ID");
    insertParser(dict, strdup("assignmentStmt"), assignmentStmt, 1);

    // SingleOrRecId
    char **SingleOrRecId = (char **)malloc(1 * sizeof(char *));
    SingleOrRecId[0] = strdup("TK_ID");
    insertParser(dict, strdup("SingleOrRecId"), SingleOrRecId, 1);

    // option_single_constructed
    char **option_single_constructed = (char **)malloc(2 * sizeof(char *));
    option_single_constructed[0] = strdup("TK_DOT");
    option_single_constructed[1] = strdup("ε");
    insertParser(dict, strdup("option_single_constructed"), option_single_constructed, 2);

    // oneExpansion
    char **oneExpansion = (char **)malloc(1 * sizeof(char *));
    oneExpansion[0] = strdup("TK_DOT");
    insertParser(dict, strdup("oneExpansion"), oneExpansion, 1);

    // moreExpansions
    char **moreExpansions = (char **)malloc(2 * sizeof(char *));
    moreExpansions[0] = strdup("TK_DOT");
    moreExpansions[1] = strdup("ε");
    insertParser(dict, strdup("moreExpansions"), moreExpansions, 2);

    // funCallStmt
    char **funCallStmt = (char **)malloc(2 * sizeof(char *));
    funCallStmt[0] = strdup("TK_SQL");
    funCallStmt[1] = strdup("TK_CALL");
    insertParser(dict, strdup("funCallStmt"), funCallStmt, 2);

    // outputParameters
    char **outputParameters = (char **)malloc(2 * sizeof(char *));
    outputParameters[0] = strdup("TK_SQL");
    outputParameters[1] = strdup("ε");
    insertParser(dict, strdup("outputParameters"), outputParameters, 2);

    // inputParameters
    char **inputParameters = (char **)malloc(1 * sizeof(char *));
    inputParameters[0] = strdup("TK_SQL");
    insertParser(dict, strdup("inputParameters"), inputParameters, 1);

    // iterativeStmt
    char **iterativeStmt = (char **)malloc(1 * sizeof(char *));
    iterativeStmt[0] = strdup("TK_WHILE");
    insertParser(dict, strdup("iterativeStmt"), iterativeStmt, 1);

    // conditionalStmt
    char **conditionalStmt = (char **)malloc(1 * sizeof(char *));
    conditionalStmt[0] = strdup("TK_IF");
    insertParser(dict, strdup("conditionalStmt"), conditionalStmt, 1);

    // elsePart
    char **elsePart = (char **)malloc(2 * sizeof(char *));
    elsePart[0] = strdup("TK_ELSE");
    elsePart[1] = strdup("TK_ENDIF");
    insertParser(dict, strdup("elsePart"), elsePart, 2);

    // ioStmt
    char **ioStmt = (char **)malloc(2 * sizeof(char *));
    ioStmt[0] = strdup("TK_READ");
    ioStmt[1] = strdup("TK_WRITE");
    insertParser(dict, strdup("ioStmt"), ioStmt, 2);

    // arithmeticExpression
    char **arithmeticExpression = (char **)malloc(4 * sizeof(char *));
    arithmeticExpression[0] = strdup("TK_ID");
    arithmeticExpression[1] = strdup("TK_OP");
    arithmeticExpression[2] = strdup("TK_NUM");
    arithmeticExpression[3] = strdup("TK_RNUM");
    insertParser(dict, strdup("arithmeticExpression"), arithmeticExpression, 4);

    // expPrime
    char **expPrime = (char **)malloc(3 * sizeof(char *));
    expPrime[0] = strdup("TK_PLUS");
    expPrime[1] = strdup("TK_MINUS");
    expPrime[2] = strdup("ε");
    insertParser(dict, strdup("expPrime"), expPrime, 3);

    // term
    char **term = (char **)malloc(4 * sizeof(char *));
    term[0] = strdup("TK_ID");
    term[1] = strdup("TK_OP");
    term[2] = strdup("TK_NUM");
    term[3] = strdup("TK_RNUM");
    insertParser(dict, strdup("term"), term, 4);

    // termPrime
    char **termPrime = (char **)malloc(3 * sizeof(char *));
    termPrime[0] = strdup("TK_MUL");
    termPrime[1] = strdup("TK_DIV");
    termPrime[2] = strdup("ε");
    insertParser(dict, strdup("termPrime"), termPrime, 3);

    // factor
    char **factor = (char **)malloc(4 * sizeof(char *));
    factor[0] = strdup("TK_ID");
    factor[1] = strdup("TK_OP");
    factor[2] = strdup("TK_NUM");
    factor[3] = strdup("TK_RNUM");
    insertParser(dict, strdup("factor"), factor, 4);

    // highPrecedenceOperators
    char **highPrecedenceOperators = (char **)malloc(2 * sizeof(char *));
    highPrecedenceOperators[0] = strdup("TK_MUL");
    highPrecedenceOperators[1] = strdup("TK_DIV");
    insertParser(dict, strdup("highPrecedenceOperators"), highPrecedenceOperators, 2);

    // lowPrecedenceOperators
    char **lowPrecedenceOperators = (char **)malloc(2 * sizeof(char *));
    lowPrecedenceOperators[0] = strdup("TK_PLUS");
    lowPrecedenceOperators[1] = strdup("TK_MINUS");
    insertParser(dict, strdup("lowPrecedenceOperators"), lowPrecedenceOperators, 2);

    // booleanExpression
    char **booleanExpression = (char **)malloc(5 * sizeof(char *));
    booleanExpression[0] = strdup("TK_ID");
    booleanExpression[1] = strdup("TK_OP");
    booleanExpression[2] = strdup("TK_NOT");
    booleanExpression[3] = strdup("TK_NUM");
    booleanExpression[4] = strdup("TK_RNUM");
    insertParser(dict, strdup("booleanExpression"), booleanExpression, 5);

    // var
    char **var = (char **)malloc(3 * sizeof(char *));
    var[0] = strdup("TK_ID");
    var[1] = strdup("TK_NUM");
    var[2] = strdup("TK_RNUM");
    insertParser(dict, strdup("var"), var, 3);

    // logicalOp
    char **logicalOp = (char **)malloc(2 * sizeof(char *));
    logicalOp[0] = strdup("TK_AND");
    logicalOp[1] = strdup("TK_OR");
    insertParser(dict, strdup("logicalOp"), logicalOp, 2);

    // relationalOp
    char **relationalOp = (char **)malloc(6 * sizeof(char *));
    relationalOp[0] = strdup("TK_LT");
    relationalOp[1] = strdup("TK_LE");
    relationalOp[2] = strdup("TK_EQ");
    relationalOp[3] = strdup("TK_GT");
    relationalOp[4] = strdup("TK_GE");
    relationalOp[5] = strdup("TK_NE");
    insertParser(dict, strdup("relationalOp"), relationalOp, 6);

    // returnStmt
    char **returnStmt = (char **)malloc(1 * sizeof(char *));
    returnStmt[0] = strdup("TK_RETURN");
    insertParser(dict, strdup("returnStmt"), returnStmt, 1);

    // optionalReturn
    char **optionalReturn = (char **)malloc(2 * sizeof(char *));
    optionalReturn[0] = strdup("TK_SQL");
    optionalReturn[1] = strdup("ε");
    insertParser(dict, strdup("optionalReturn"), optionalReturn, 2);

    // idList
    char **idList = (char **)malloc(1 * sizeof(char *));
    idList[0] = strdup("TK_ID");
    insertParser(dict, strdup("idList"), idList, 1);

    // more_ids
    char **more_ids = (char **)malloc(2 * sizeof(char *));
    more_ids[0] = strdup("TK_COMMA");
    more_ids[1] = strdup("ε");
    insertParser(dict, strdup("more_ids"), more_ids, 2);

    // definetypestmt
    char **definetypestmt = (char **)malloc(1 * sizeof(char *));
    definetypestmt[0] = strdup("TK_DEFINETYPE");
    insertParser(dict, strdup("definetypestmt"), definetypestmt, 1);

    // A
    char **A = (char **)malloc(2 * sizeof(char *));
    A[0] = strdup("TK_RECORD");
    A[1] = strdup("TK_UNION");
    insertParser(dict, strdup("A"), A, 2);

    // Free dictionary and exit
    // (Remember to free memory allocated for keys and values)
}

void initFollow(DictionaryParser *dict)
{
    // program
    char **program = (char **)malloc(1 * sizeof(char *));
    program[0] = strdup("ε");
    insertParser(dict, strdup("program"), program, 1);

    // mainFunction
    char **mainFunction = (char **)malloc(1 * sizeof(char *));
    mainFunction[0] = strdup("ε");
    insertParser(dict, strdup("mainFunction"), mainFunction, 1);

    // otherFunctions
    char **otherFunctions = (char **)malloc(1 * sizeof(char *));
    otherFunctions[0] = strdup("TK_MAIN");
    insertParser(dict, strdup("otherFunctions"), otherFunctions, 1);

    // function
    char **function = (char **)malloc(2 * sizeof(char *));
    function[0] = strdup("TK_MAIN");
    function[1] = strdup("TK_FUNID");
    insertParser(dict, strdup("function"), function, 2);

    // input_par
    char **input_par = (char **)malloc(2 * sizeof(char *));
    input_par[0] = strdup("TK_SEM");
    input_par[1] = strdup("TK_OUTPUT");
    insertParser(dict, strdup("input_par"), input_par, 2);

    // output_par
    char **output_par = (char **)malloc(1 * sizeof(char *));
    output_par[0] = strdup("TK_SEM");
    insertParser(dict, strdup("output_par"), output_par, 1);

    // parameter_list
    char **parameter_list = (char **)malloc(1 * sizeof(char *));
    parameter_list[0] = strdup("TK_SQR");
    insertParser(dict, strdup("parameter_list"), parameter_list, 1);

    // dataType
    char **dataType = (char **)malloc(2 * sizeof(char *));
    dataType[0] = strdup("TK_ID");
    dataType[1] = strdup("TK_COLON");
    insertParser(dict, strdup("dataType"), dataType, 2);

    // primitiveDatatype
    char **primitiveDatatype = (char **)malloc(2 * sizeof(char *));
    primitiveDatatype[0] = strdup("TK_ID");
    primitiveDatatype[1] = strdup("TK_COLON");
    insertParser(dict, strdup("primitiveDatatype"), primitiveDatatype, 2);

    // constructedDatatype
    char **constructedDatatype = (char **)malloc(2 * sizeof(char *));
    constructedDatatype[0] = strdup("TK_ID");
    constructedDatatype[1] = strdup("TK_COLON");
    insertParser(dict, strdup("constructedDatatype"), constructedDatatype, 2);

    // remaining_list
    char **remaining_list = (char **)malloc(1 * sizeof(char *));
    remaining_list[0] = strdup("TK_SQR");
    insertParser(dict, strdup("remaining_list"), remaining_list, 1);

    // stmts
    char **stmts = (char **)malloc(1 * sizeof(char *));
    stmts[0] = strdup("TK_END");
    insertParser(dict, strdup("stmts"), stmts, 1);

    // typeDefinitions
    char **typeDefinitions = (char **)malloc(12 * sizeof(char *));
    typeDefinitions[0] = strdup("TK_SQL");
    typeDefinitions[1] = strdup("TK_ID");
    typeDefinitions[2] = strdup("TK_TYPE");
    typeDefinitions[3] = strdup("TK_CALL");
    typeDefinitions[4] = strdup("TK_WHILE");
    typeDefinitions[5] = strdup("TK_IF");
    typeDefinitions[6] = strdup("TK_READ");
    typeDefinitions[7] = strdup("TK_WRITE");
    typeDefinitions[8] = strdup("TK_RETURN");
    typeDefinitions[9] = strdup("TK_DEFINETYPE");
    typeDefinitions[10] = strdup("TK_RECORD");
    typeDefinitions[11] = strdup("TK_UNION");
    insertParser(dict, strdup("typeDefinitions"), typeDefinitions, 12);

    // actualOrRedefined
    char **actualOrRedefined = (char **)malloc(13 * sizeof(char *));
    actualOrRedefined[0] = strdup("TK_SQL");
    actualOrRedefined[1] = strdup("TK_ID");
    actualOrRedefined[2] = strdup("TK_RECORD");
    actualOrRedefined[3] = strdup("TK_UNION");
    actualOrRedefined[4] = strdup("TK_TYPE");
    actualOrRedefined[5] = strdup("TK_CALL");
    actualOrRedefined[6] = strdup("TK_WHILE");
    actualOrRedefined[7] = strdup("TK_IF");
    actualOrRedefined[8] = strdup("TK_READ");
    actualOrRedefined[9] = strdup("TK_WRITE");
    actualOrRedefined[10] = strdup("TK_RETURN");
    actualOrRedefined[11] = strdup("TK_DEFINETYPE");
    actualOrRedefined[12] = strdup("TK_SEM");
    insertParser(dict, strdup("actualOrRedefined"), actualOrRedefined, 13);

    // typeDefinition
    char **typeDefinition = (char **)malloc(12 * sizeof(char *));
    typeDefinition[0] = strdup("TK_SQL");
    typeDefinition[1] = strdup("TK_ID");
    typeDefinition[2] = strdup("TK_RECORD");
    typeDefinition[3] = strdup("TK_UNION");
    typeDefinition[4] = strdup("TK_TYPE");
    typeDefinition[5] = strdup("TK_CALL");
    typeDefinition[6] = strdup("TK_WHILE");
    typeDefinition[7] = strdup("TK_IF");
    typeDefinition[8] = strdup("TK_READ");
    typeDefinition[9] = strdup("TK_WRITE");
    typeDefinition[10] = strdup("TK_RETURN");
    typeDefinition[11] = strdup("TK_DEFINETYPE");
    insertParser(dict, strdup("typeDefinition"), typeDefinition, 12);

    // fieldDefinitions
    char **fieldDefinitions = (char **)malloc(3 * sizeof(char *));
    fieldDefinitions[0] = strdup("TK_ENDRECORD");
    fieldDefinitions[1] = strdup("TK_ENDUNION");
    fieldDefinitions[2] = strdup("TK_TYPE");
    insertParser(dict, strdup("fieldDefinitions"), fieldDefinitions, 3);

    // fieldDefinition
    char **fieldDefinition = (char **)malloc(3 * sizeof(char *));
    fieldDefinition[0] = strdup("TK_ENDRECORD");
    fieldDefinition[1] = strdup("TK_ENDUNION");
    fieldDefinition[2] = strdup("TK_TYPE");
    insertParser(dict, strdup("fieldDefinition"), fieldDefinition, 3);

    // fieldType
    char **fieldType = (char **)malloc(1 * sizeof(char *));
    fieldType[0] = strdup("TK_COLON");
    insertParser(dict, strdup("fieldType"), fieldType, 1);

    // moreFields
    char **moreFields = (char **)malloc(2 * sizeof(char *));
    moreFields[0] = strdup("TK_ENDRECORD");
    moreFields[1] = strdup("TK_ENDUNION");
    insertParser(dict, strdup("moreFields"), moreFields, 2);

    // declarations
    char **declarations = (char **)malloc(8 * sizeof(char *));
    declarations[0] = strdup("TK_SQL");
    declarations[1] = strdup("TK_ID");
    declarations[2] = strdup("TK_CALL");
    declarations[3] = strdup("TK_WHILE");
    declarations[4] = strdup("TK_IF");
    declarations[5] = strdup("TK_READ");
    declarations[6] = strdup("TK_WRITE");
    declarations[7] = strdup("TK_RETURN");
    insertParser(dict, strdup("declarations"), declarations, 8);

    // declaration
    char **declaration = (char **)malloc(9 * sizeof(char *));
    declaration[0] = strdup("TK_SQL");
    declaration[1] = strdup("TK_ID");
    declaration[2] = strdup("TK_TYPE");
    declaration[3] = strdup("TK_CALL");
    declaration[4] = strdup("TK_WHILE");
    declaration[5] = strdup("TK_IF");
    declaration[6] = strdup("TK_READ");
    declaration[7] = strdup("TK_WRITE");
    declaration[8] = strdup("TK_RETURN");
    insertParser(dict, strdup("declaration"), declaration, 9);

    // global_or_not
    char **global_or_not = (char **)malloc(1 * sizeof(char *));
    global_or_not[0] = strdup("TK_SEM");
    insertParser(dict, strdup("global_or_not"), global_or_not, 1);

    // otherStmts
    char **otherStmts = (char **)malloc(4 * sizeof(char *));
    otherStmts[0] = strdup("TK_ENDWHILE");
    otherStmts[1] = strdup("TK_ELSE");
    otherStmts[2] = strdup("TK_ENDIF");
    otherStmts[3] = strdup("TK_RETURN");
    insertParser(dict, strdup("otherStmts"), otherStmts, 4);

    // stmt
    char **stmt = (char **)malloc(12 * sizeof(char *));
    stmt[0] = strdup("TK_SQL");
    stmt[1] = strdup("TK_ID");
    stmt[2] = strdup("TK_CALL");
    stmt[3] = strdup("TK_WHILE");
    stmt[4] = strdup("TK_ENDWHILE");
    stmt[5] = strdup("TK_IF");
    stmt[6] = strdup("TK_ELSE");
    stmt[7] = strdup("TK_ENDIF");
    stmt[8] = strdup("TK_READ");
    stmt[9] = strdup("TK_WRITE");
    stmt[10] = strdup("TK_RETURN");
    stmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("stmt"), stmt, 12);

    // assignmentStmt
    char **assignmentStmt = (char **)malloc(12 * sizeof(char *));
    assignmentStmt[0] = strdup("TK_SQL");
    assignmentStmt[1] = strdup("TK_ID");
    assignmentStmt[2] = strdup("TK_CALL");
    assignmentStmt[3] = strdup("TK_WHILE");
    assignmentStmt[4] = strdup("TK_ENDWHILE");
    assignmentStmt[5] = strdup("TK_IF");
    assignmentStmt[6] = strdup("TK_ELSE");
    assignmentStmt[7] = strdup("TK_ENDIF");
    assignmentStmt[8] = strdup("TK_READ");
    assignmentStmt[9] = strdup("TK_WRITE");
    assignmentStmt[10] = strdup("TK_RETURN");
    assignmentStmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("assignmentStmt"), assignmentStmt, 12);

    // SingleOrRecId
    char **SingleOrRecId = (char **)malloc(15 * sizeof(char *));
    SingleOrRecId[0] = strdup("TK_SEM");
    SingleOrRecId[1] = strdup("TK_ASSIGNOP");
    SingleOrRecId[2] = strdup("TK_CL");
    SingleOrRecId[3] = strdup("TK_MUL");
    SingleOrRecId[4] = strdup("TK_DIV");
    SingleOrRecId[5] = strdup("TK_PLUS");
    SingleOrRecId[6] = strdup("TK_MINUS");
    SingleOrRecId[7] = strdup("TK_LT");
    SingleOrRecId[8] = strdup("TK_LE");
    SingleOrRecId[9] = strdup("TK_EQ");
    SingleOrRecId[10] = strdup("TK_GT");
    SingleOrRecId[11] = strdup("TK_GE");
    SingleOrRecId[12] = strdup("TK_NE");
    SingleOrRecId[13] = strdup("TK_ID");
    SingleOrRecId[14] = strdup("TK_RECORD");
    insertParser(dict, strdup("SingleOrRecId"), SingleOrRecId, 15);

    // option_single_constructed
    char **option_single_constructed = (char **)malloc(15 * sizeof(char *));
    option_single_constructed[0] = strdup("TK_SEM");
    option_single_constructed[1] = strdup("TK_ASSIGNOP");
    option_single_constructed[2] = strdup("TK_CL");
    option_single_constructed[3] = strdup("TK_MUL");
    option_single_constructed[4] = strdup("TK_DIV");
    option_single_constructed[5] = strdup("TK_PLUS");
    option_single_constructed[6] = strdup("TK_MINUS");
    option_single_constructed[7] = strdup("TK_LT");
    option_single_constructed[8] = strdup("TK_LE");
    option_single_constructed[9] = strdup("TK_EQ");
    option_single_constructed[10] = strdup("TK_GT");
    option_single_constructed[11] = strdup("TK_GE");
    option_single_constructed[12] = strdup("TK_NE");
    option_single_constructed[13] = strdup("TK_ID");
    option_single_constructed[14] = strdup("TK_RECORD");
    insertParser(dict, strdup("option_single_constructed"), option_single_constructed, 15);

    // oneExpansion
    char **oneExpansion = (char **)malloc(15 * sizeof(char *));
    oneExpansion[0] = strdup("TK_SEM");
    oneExpansion[1] = strdup("TK_ASSIGNOP");
    oneExpansion[2] = strdup("TK_DOT");
    oneExpansion[3] = strdup("TK_CL");
    oneExpansion[4] = strdup("TK_MUL");
    oneExpansion[5] = strdup("TK_DIV");
    oneExpansion[6] = strdup("TK_PLUS");
    oneExpansion[7] = strdup("TK_MINUS");
    oneExpansion[8] = strdup("TK_LT");
    oneExpansion[9] = strdup("TK_LE");
    oneExpansion[10] = strdup("TK_EQ");
    oneExpansion[11] = strdup("TK_GT");
    oneExpansion[12] = strdup("TK_GE");
    oneExpansion[13] = strdup("TK_NE");
    oneExpansion[14] = strdup("TK_ID");
    insertParser(dict, strdup("oneExpansion"), oneExpansion, 15);

    // moreExpansions
    char **moreExpansions = (char **)malloc(15 * sizeof(char *));
    moreExpansions[0] = strdup("TK_SEM");
    moreExpansions[1] = strdup("TK_ASSIGNOP");
    moreExpansions[2] = strdup("TK_CL");
    moreExpansions[3] = strdup("TK_MUL");
    moreExpansions[4] = strdup("TK_DIV");
    moreExpansions[5] = strdup("TK_PLUS");
    moreExpansions[6] = strdup("TK_MINUS");
    moreExpansions[7] = strdup("TK_LT");
    moreExpansions[8] = strdup("TK_LE");
    moreExpansions[9] = strdup("TK_EQ");
    moreExpansions[10] = strdup("TK_GT");
    moreExpansions[11] = strdup("TK_GE");
    moreExpansions[12] = strdup("TK_NE");
    moreExpansions[13] = strdup("TK_ID");
    moreExpansions[14] = strdup("TK_RECORD");
    insertParser(dict, strdup("moreExpansions"), moreExpansions, 15);

    // funCallStmt
    char **funCallStmt = (char **)malloc(12 * sizeof(char *));
    funCallStmt[0] = strdup("TK_SQL");
    funCallStmt[1] = strdup("TK_ID");
    funCallStmt[2] = strdup("TK_CALL");
    funCallStmt[3] = strdup("TK_WHILE");
    funCallStmt[4] = strdup("TK_ENDWHILE");
    funCallStmt[5] = strdup("TK_IF");
    funCallStmt[6] = strdup("TK_ELSE");
    funCallStmt[7] = strdup("TK_ENDIF");
    funCallStmt[8] = strdup("TK_READ");
    funCallStmt[9] = strdup("TK_WRITE");
    funCallStmt[10] = strdup("TK_RETURN");
    funCallStmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("funCallStmt"), funCallStmt, 12);

    // outputParameters
    char **outputParameters = (char **)malloc(1 * sizeof(char *));
    outputParameters[0] = strdup("TK_CALL");
    insertParser(dict, strdup("outputParameters"), outputParameters, 1);

    // inputParameters
    char **inputParameters = (char **)malloc(1 * sizeof(char *));
    inputParameters[0] = strdup("TK_SEM");
    insertParser(dict, strdup("inputParameters"), inputParameters, 1);

    // iterativeStmt
    char **iterativeStmt = (char **)malloc(12 * sizeof(char *));
    iterativeStmt[0] = strdup("TK_SQL");
    iterativeStmt[1] = strdup("TK_ID");
    iterativeStmt[2] = strdup("TK_CALL");
    iterativeStmt[3] = strdup("TK_WHILE");
    iterativeStmt[4] = strdup("TK_ENDWHILE");
    iterativeStmt[5] = strdup("TK_IF");
    iterativeStmt[6] = strdup("TK_ELSE");
    iterativeStmt[7] = strdup("TK_ENDIF");
    iterativeStmt[8] = strdup("TK_READ");
    iterativeStmt[9] = strdup("TK_WRITE");
    iterativeStmt[10] = strdup("TK_RETURN");
    iterativeStmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("iterativeStmt"), iterativeStmt, 12);

    // conditionalStmt
    char **conditionalStmt = (char **)malloc(12 * sizeof(char *));
    conditionalStmt[0] = strdup("TK_SQL");
    conditionalStmt[1] = strdup("TK_ID");
    conditionalStmt[2] = strdup("TK_CALL");
    conditionalStmt[3] = strdup("TK_WHILE");
    conditionalStmt[4] = strdup("TK_ENDWHILE");
    conditionalStmt[5] = strdup("TK_IF");
    conditionalStmt[6] = strdup("TK_ELSE");
    conditionalStmt[7] = strdup("TK_ENDIF");
    conditionalStmt[8] = strdup("TK_READ");
    conditionalStmt[9] = strdup("TK_WRITE");
    conditionalStmt[10] = strdup("TK_RETURN");
    conditionalStmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("conditionalStmt"), conditionalStmt, 12);

    // elsePart
    char **elsePart = (char **)malloc(12 * sizeof(char *));
    elsePart[0] = strdup("TK_SQL");
    elsePart[1] = strdup("TK_ID");
    elsePart[2] = strdup("TK_CALL");
    elsePart[3] = strdup("TK_WHILE");
    elsePart[4] = strdup("TK_ENDWHILE");
    elsePart[5] = strdup("TK_IF");
    elsePart[6] = strdup("TK_ELSE");
    elsePart[7] = strdup("TK_ENDIF");
    elsePart[8] = strdup("TK_READ");
    elsePart[9] = strdup("TK_WRITE");
    elsePart[10] = strdup("TK_RETURN");
    elsePart[11] = strdup("TK_SEM");
    insertParser(dict, strdup("elsePart"), elsePart, 12);

    // ioStmt
    char **ioStmt = (char **)malloc(12 * sizeof(char *));
    ioStmt[0] = strdup("TK_SQL");
    ioStmt[1] = strdup("TK_ID");
    ioStmt[2] = strdup("TK_CALL");
    ioStmt[3] = strdup("TK_WHILE");
    ioStmt[4] = strdup("TK_ENDWHILE");
    ioStmt[5] = strdup("TK_IF");
    ioStmt[6] = strdup("TK_ELSE");
    ioStmt[7] = strdup("TK_ENDIF");
    ioStmt[8] = strdup("TK_READ");
    ioStmt[9] = strdup("TK_WRITE");
    ioStmt[10] = strdup("TK_RETURN");
    ioStmt[11] = strdup("TK_SEM");
    insertParser(dict, strdup("ioStmt"), ioStmt, 12);

    // arithmeticExpression
    char **arithmeticExpression = (char **)malloc(8 * sizeof(char *));
    arithmeticExpression[0] = strdup("TK_SEM");
    arithmeticExpression[1] = strdup("TK_CL");
    arithmeticExpression[2] = strdup("TK_PLUS");
    arithmeticExpression[3] = strdup("TK_MINUS");
    arithmeticExpression[4] = strdup("TK_MUL");
    arithmeticExpression[5] = strdup("TK_DIV");
    arithmeticExpression[6] = strdup("TK_ID");
    arithmeticExpression[7] = strdup("TK_NUM");
    insertParser(dict, strdup("arithmeticExpression"), arithmeticExpression, 8);

    // expPrime
    char **expPrime = (char **)malloc(6 * sizeof(char *));
    expPrime[0] = strdup("TK_SEM");
    expPrime[1] = strdup("TK_CL");
    expPrime[2] = strdup("TK_PLUS");
    expPrime[3] = strdup("TK_MINUS");
    expPrime[4] = strdup("TK_ID");
    expPrime[5] = strdup("TK_NUM");
    insertParser(dict, strdup("expPrime"), expPrime, 6);

    // term
    char **term = (char **)malloc(4 * sizeof(char *));
    term[0] = strdup("TK_SEM");
    term[1] = strdup("TK_CL");
    term[2] = strdup("TK_PLUS");
    term[3] = strdup("TK_MINUS");
    insertParser(dict, strdup("term"), term, 4);

    // termPrime
    char **termPrime = (char **)malloc(4 * sizeof(char *));
    termPrime[0] = strdup("TK_SEM");
    termPrime[1] = strdup("TK_CL");
    termPrime[2] = strdup("TK_PLUS");
    termPrime[3] = strdup("TK_MINUS");
    insertParser(dict, strdup("termPrime"), termPrime, 4);

    // factor
    char **factor = (char **)malloc(6 * sizeof(char *));
    factor[0] = strdup("TK_SEM");
    factor[1] = strdup("TK_CL");
    factor[2] = strdup("TK_MUL");
    factor[3] = strdup("TK_DIV");
    factor[4] = strdup("TK_PLUS");
    factor[5] = strdup("TK_MINUS");
    insertParser(dict, strdup("factor"), factor, 6);

    // highPrecedenceOperators
    char **highPrecedenceOperators = (char **)malloc(3 * sizeof(char *));
    highPrecedenceOperators[0] = strdup("TK_ID");
    highPrecedenceOperators[1] = strdup("TK_OP");
    highPrecedenceOperators[2] = strdup("TK_NUM");
    insertParser(dict, strdup("highPrecedenceOperators"), highPrecedenceOperators, 3);

    // lowPrecedenceOperators
    char **lowPrecedenceOperators = (char **)malloc(3 * sizeof(char *));
    lowPrecedenceOperators[0] = strdup("TK_ID");
    lowPrecedenceOperators[1] = strdup("TK_OP");
    lowPrecedenceOperators[2] = strdup("TK_NUM");
    insertParser(dict, strdup("lowPrecedenceOperators"), lowPrecedenceOperators, 3);

    // booleanExpression
    char **booleanExpression = (char **)malloc(1 * sizeof(char *));
    booleanExpression[0] = strdup("TK_CL");
    insertParser(dict, strdup("booleanExpression"), booleanExpression, 1);

    // var
    char **var = (char **)malloc(12 * sizeof(char *));
    var[0] = strdup("TK_SEM");
    var[1] = strdup("TK_CL");
    var[2] = strdup("TK_MUL");
    var[3] = strdup("TK_DIV");
    var[4] = strdup("TK_PLUS");
    var[5] = strdup("TK_MINUS");
    var[6] = strdup("TK_LT");
    var[7] = strdup("TK_LE");
    var[8] = strdup("TK_EQ");
    var[9] = strdup("TK_GT");
    var[10] = strdup("TK_GE");
    var[11] = strdup("TK_NE");
    insertParser(dict, strdup("var"), var, 12);

    // logicalOp
    char **logicalOp = (char **)malloc(4 * sizeof(char *));
    logicalOp[0] = strdup("TK_AND");
    logicalOp[1] = strdup("TK_OR");
    logicalOp[2] = strdup("TK_NOT");
    logicalOp[3] = strdup("TK_OP");
    insertParser(dict, strdup("logicalOp"), logicalOp, 4);

    // relationalOp
    char **relationalOp = (char **)malloc(4 * sizeof(char *));
    relationalOp[0] = strdup("TK_ID");
    relationalOp[1] = strdup("TK_NUM");
    relationalOp[2] = strdup("TK_RNUM");
    relationalOp[3] = strdup("TK_OP");
    insertParser(dict, strdup("relationalOp"), relationalOp, 4);

    // returnStmt
    char **returnStmt = (char **)malloc(1 * sizeof(char *));
    returnStmt[0] = strdup("TK_END");
    insertParser(dict, strdup("returnStmt"), returnStmt, 1);

    // optionalReturn
    char **optionalReturn = (char **)malloc(1 * sizeof(char *));
    optionalReturn[0] = strdup("TK_SEM");
    insertParser(dict, strdup("optionalReturn"), optionalReturn, 1);

    // idList
    char **idList = (char **)malloc(1 * sizeof(char *));
    idList[0] = strdup("TK_SQR");
    insertParser(dict, strdup("idList"), idList, 1);

    // more_ids
    char **more_ids = (char **)malloc(1 * sizeof(char *));
    more_ids[0] = strdup("TK_SQR");
    insertParser(dict, strdup("more_ids"), more_ids, 1);

    // definetypestmt
    char **definetypestmt = (char **)malloc(12 * sizeof(char *));
    definetypestmt[0] = strdup("TK_SQL");
    definetypestmt[1] = strdup("TK_ID");
    definetypestmt[2] = strdup("TK_RECORD");
    definetypestmt[3] = strdup("TK_UNION");
    definetypestmt[4] = strdup("TK_TYPE");
    definetypestmt[5] = strdup("TK_CALL");
    definetypestmt[6] = strdup("TK_WHILE");
    definetypestmt[7] = strdup("TK_IF");
    definetypestmt[8] = strdup("TK_READ");
    definetypestmt[9] = strdup("TK_WRITE");
    definetypestmt[10] = strdup("TK_RETURN");
    definetypestmt[11] = strdup("TK_DEFINETYPE");
    insertParser(dict, strdup("definetypestmt"), definetypestmt, 12);

    // A
    char **A = (char **)malloc(1 * sizeof(char *));
    A[0] = strdup("TK_RUID");
    insertParser(dict, strdup("A"), A, 1);
}

int searchF(DictionaryParser *dict, char *NT, char *T)
{
    int index = hashNT(NT);
    int len = dict->table[index]->len;
    char **list = dict->table[index]->value;
    int result = -1;
    for (int i = 0; i < len; i++)
    {
        if (!strcmp(list[i], T))
        {
            result = 1;
            break;
        }
        else if (!strcmp(list[i], "ε\0"))
        {
            result = 0;
        }
    }
    return result;
}

int hashT(char *str)
{
    static unsigned char asso_values[] =
        {
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 30, 126, 30, 0, 10,
            45, 45, 50, 0, 126, 126, 15, 35, 5, 20,
            30, 55, 0, 0, 0, 0, 0, 10, 126, 20,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
            126, 126, 126, 126, 126, 126};
    return strlen(str) + asso_values[(unsigned char)str[4]] + asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[strlen(str) - 1]];
}

void initRules(RULES *rules)
{
    rules->heads = NULL;
    rules->length = 0;
}

// Function to add a rule to the grammar
void addRule(GRAMMAR *grammar, char *nonTerminal, NODE *nodes)
{
    int index = hashNT(nonTerminal);
    if (grammar->rules[index] == NULL)
    {
        RULES *newRules = (RULES *)malloc(sizeof(RULES));
        initRules(newRules);
        newRules->length++;
        newRules->heads = (NODE *)malloc(10 * sizeof(NODE));
        newRules->heads[0] = nodes[0];
        grammar->rules[index] = newRules;
    }
    else
    {
        grammar->rules[index]->heads[grammar->rules[index]->length] = nodes[0];
        grammar->rules[index]->length++;
        // printf("Non-terminal %s already has a rule defined.\n", nonTerminal);
    }
}

// Function to retrieve rules for a non-terminal
RULES *getRules(GRAMMAR *grammar, char *nonTerminal)
{
    int index = hashNT(nonTerminal);
    return grammar->rules[index];
}

void printGrammar(GRAMMAR *grammar)
{
    int count = 1;
    for (int i = 0; i < FAF_TABLE_SIZE; i++)
    {
        if (grammar->rules[i] != NULL)
        {
            printf("\n");
            printf("Non terminal number: %d\n", count++);
            RULES *r = grammar->rules[i];
            for (int i = 0; i < r->length; i++)
            {
                printf("Rule %d:\n", i + 1);
                printf("Name: %s, Terminal: %s\n", r->heads[i].name, r->heads[i].terminal ? "true" : "false");
                NODE *head = r->heads[i].next;
                while (head != NULL)
                {
                    printf("Name: %s, Terminal: %s\n", head->name, head->terminal ? "true" : "false");
                    head = head->next;
                }
            }
        }
    }
}

void initGrammer(GRAMMAR *grammar)
{

    for (int i = 0; i < FAF_TABLE_SIZE; i++)
    {
        grammar->rules[i] = NULL;
    }

    // <program> ===> <otherFunctions> <mainFunction>
    NODE *nodes_program = malloc(2 * sizeof(NODE));

    nodes_program[0] = (NODE){"otherFunctions", false, &nodes_program[1]};
    nodes_program[1] = (NODE){"mainFunction", false, NULL};

    addRule(grammar, "program", nodes_program);

    // <mainFunction> ===> TK_MAIN <stmts> TK_END
    NODE *nodes_mainFunction = malloc(3 * sizeof(NODE));

    nodes_mainFunction[0] = (NODE){"TK_MAIN", true, &nodes_mainFunction[1]};
    nodes_mainFunction[1] = (NODE){"stmts", false, &nodes_mainFunction[2]};
    nodes_mainFunction[2] = (NODE){"TK_END", true, NULL};

    addRule(grammar, "mainFunction", nodes_mainFunction);

    // <otherFunctions> ===> <function><otherFunctions> | ε
    NODE *nodes_otherFunctions_1 = (NODE *)malloc(2 * sizeof(NODE));
    NODE *nodes_otherFunctions_2 = malloc(1 * sizeof(NODE));

    nodes_otherFunctions_1[0] = (NODE){"function", false, &nodes_otherFunctions_1[1]};
    nodes_otherFunctions_1[1] = (NODE){"otherFunctions", false, NULL};

    nodes_otherFunctions_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "otherFunctions", nodes_otherFunctions_1);
    addRule(grammar, "otherFunctions", nodes_otherFunctions_2);

    // <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    NODE *nodes_function = malloc(6 * sizeof(NODE));

    nodes_function[0] = (NODE){"TK_FUNID", true, &nodes_function[1]};
    nodes_function[1] = (NODE){"input_par", false, &nodes_function[2]};
    nodes_function[2] = (NODE){"output_par", false, &nodes_function[3]};
    nodes_function[3] = (NODE){"TK_SEM", true, &nodes_function[4]};
    nodes_function[4] = (NODE){"stmts", false, &nodes_function[5]};
    nodes_function[5] = (NODE){"TK_END", true, NULL};

    addRule(grammar, "function", nodes_function);

    // <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    NODE *nodes_input_par = malloc(6 * sizeof(NODE));

    nodes_input_par[0] = (NODE){"TK_INPUT", true, &nodes_input_par[1]};
    nodes_input_par[1] = (NODE){"TK_PARAMETER", true, &nodes_input_par[2]};
    nodes_input_par[2] = (NODE){"TK_LIST", true, &nodes_input_par[3]};
    nodes_input_par[3] = (NODE){"TK_SQL", true, &nodes_input_par[4]};
    nodes_input_par[4] = (NODE){"parameter_list", false, &nodes_input_par[5]};
    nodes_input_par[5] = (NODE){"TK_SQR", true, NULL};

    addRule(grammar, "input_par", nodes_input_par);

    // <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | ε
    NODE *nodes_output_par_1 = malloc(6 * sizeof(NODE));
    NODE *nodes_output_par_2 = malloc(1 * sizeof(NODE));

    nodes_output_par_1[0] = (NODE){"TK_OUTPUT", true, &nodes_output_par_1[1]};
    nodes_output_par_1[1] = (NODE){"TK_PARAMETER", true, &nodes_output_par_1[2]};
    nodes_output_par_1[2] = (NODE){"TK_LIST", true, &nodes_output_par_1[3]};
    nodes_output_par_1[3] = (NODE){"TK_SQL", true, &nodes_output_par_1[4]};
    nodes_output_par_1[4] = (NODE){"parameter_list", false, &nodes_output_par_1[5]};
    nodes_output_par_1[5] = (NODE){"TK_SQR", true, NULL};

    nodes_output_par_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "output_par", nodes_output_par_1);
    addRule(grammar, "output_par", nodes_output_par_2);

    // <parameter_list>===><dataType> TK_ID <remaining_list>
    NODE *nodes_parameter_list = malloc(3 * sizeof(NODE));

    nodes_parameter_list[0] = (NODE){"dataType", false, &nodes_parameter_list[1]};
    nodes_parameter_list[1] = (NODE){"TK_ID", true, &nodes_parameter_list[2]};
    nodes_parameter_list[2] = (NODE){"remaining_list", false, NULL};

    addRule(grammar, "parameter_list", nodes_parameter_list);

    // <dataType>===> <primitiveDatatype> |<constructedDatatype>
    NODE *nodes_dataType_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_dataType_2 = malloc(1 * sizeof(NODE));

    nodes_dataType_1[0] = (NODE){"primitiveDatatype", false, NULL};
    nodes_dataType_2[0] = (NODE){"constructedDatatype", false, NULL};

    addRule(grammar, "dataType", nodes_dataType_1);
    addRule(grammar, "dataType", nodes_dataType_2);

    // <primitiveDatatype>===> TK_INT | TK_REAL
    NODE *nodes_primitiveDatatype_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_primitiveDatatype_2 = malloc(1 * sizeof(NODE));

    nodes_primitiveDatatype_1[0] = (NODE){"TK_INT", true, NULL};
    nodes_primitiveDatatype_2[0] = (NODE){"TK_REAL", true, NULL};

    addRule(grammar, "primitiveDatatype", nodes_primitiveDatatype_1);
    addRule(grammar, "primitiveDatatype", nodes_primitiveDatatype_2);

    // <constructedDatatype>===> TK_RECORD TK_RUID | TK_UNION TK_RUID | TK_RUID
    NODE *nodes_constructedDatatype_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_constructedDatatype_2 = malloc(2 * sizeof(NODE));
    NODE *nodes_constructedDatatype_3 = malloc(1 * sizeof(NODE));

    nodes_constructedDatatype_1[0] = (NODE){"TK_RECORD", true, &nodes_constructedDatatype_1[1]};
    nodes_constructedDatatype_1[1] = (NODE){"TK_RUID", true, NULL};

    nodes_constructedDatatype_2[0] = (NODE){"TK_UNION", true, &nodes_constructedDatatype_2[1]};
    nodes_constructedDatatype_2[1] = (NODE){"TK_RUID", true, NULL};

    nodes_constructedDatatype_3[0] = (NODE){"TK_RUID", true, NULL};

    addRule(grammar, "constructedDatatype", nodes_constructedDatatype_1);
    addRule(grammar, "constructedDatatype", nodes_constructedDatatype_2);
    addRule(grammar, "constructedDatatype", nodes_constructedDatatype_3);

    // <remaining_list>===>TK_COMMA <parameter_list> | ε
    NODE *nodes_remaining_list_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_remaining_list_2 = malloc(1 * sizeof(NODE));

    nodes_remaining_list_1[0] = (NODE){"TK_COMMA", true, &nodes_remaining_list_1[1]};
    nodes_remaining_list_1[1] = (NODE){"parameter_list", false, NULL};

    nodes_remaining_list_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "remaining_list", nodes_remaining_list_1);
    addRule(grammar, "remaining_list", nodes_remaining_list_2);

    // <stmts>===><typeDefinitions> <declarations> <otherStmts><returnStmt>
    NODE *nodes_stmts = malloc(4 * sizeof(NODE));

    nodes_stmts[0] = (NODE){"typeDefinitions", false, &nodes_stmts[1]};
    nodes_stmts[1] = (NODE){"declarations", false, &nodes_stmts[2]};
    nodes_stmts[2] = (NODE){"otherStmts", false, &nodes_stmts[3]};
    nodes_stmts[3] = (NODE){"returnStmt", false, NULL};

    addRule(grammar, "stmts", nodes_stmts);

    // <typeDefinitions>===> <actualOrRedefined> <typeDefinitions> | ε
    NODE *nodes_typeDefinitions_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_typeDefinitions_2 = malloc(1 * sizeof(NODE));

    nodes_typeDefinitions_1[0] = (NODE){"actualOrRedefined", false, &nodes_typeDefinitions_1[1]};
    nodes_typeDefinitions_1[1] = (NODE){"typeDefinitions", false, NULL};

    nodes_typeDefinitions_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "typeDefinitions", nodes_typeDefinitions_1);
    addRule(grammar, "typeDefinitions", nodes_typeDefinitions_2);

    // <actualOrRedefined> ===> <typeDefinition> | <definetypestmt>
    NODE *nodes_actualOrRedefined_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_actualOrRedefined_2 = malloc(1 * sizeof(NODE));

    nodes_actualOrRedefined_1[0] = (NODE){"typeDefinition", false, NULL};
    nodes_actualOrRedefined_2[0] = (NODE){"definetypestmt", false, NULL};

    addRule(grammar, "actualOrRedefined", nodes_actualOrRedefined_1);
    addRule(grammar, "actualOrRedefined", nodes_actualOrRedefined_2);

    // <typeDefinition>===>TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    NODE *nodes_typeDefinition_1 = malloc(4 * sizeof(NODE));
    NODE *nodes_typeDefinition_2 = malloc(4 * sizeof(NODE));

    nodes_typeDefinition_1[0] = (NODE){"TK_RECORD", true, &nodes_typeDefinition_1[1]};
    nodes_typeDefinition_1[1] = (NODE){"TK_RUID", true, &nodes_typeDefinition_1[2]};
    nodes_typeDefinition_1[2] = (NODE){"fieldDefinitions", false, &nodes_typeDefinition_1[3]};
    nodes_typeDefinition_1[3] = (NODE){"TK_ENDRECORD", true, NULL};

    nodes_typeDefinition_2[0] = (NODE){"TK_UNION", true, &nodes_typeDefinition_2[1]};
    nodes_typeDefinition_2[1] = (NODE){"TK_RUID", true, &nodes_typeDefinition_2[2]};
    nodes_typeDefinition_2[2] = (NODE){"fieldDefinitions", false, &nodes_typeDefinition_2[3]};
    nodes_typeDefinition_2[3] = (NODE){"TK_ENDUNION", true, NULL};

    addRule(grammar, "typeDefinition", nodes_typeDefinition_1);
    addRule(grammar, "typeDefinition", nodes_typeDefinition_2);

    // <fieldDefinitions>===> <fieldDefinition><fieldDefinition><moreFields>
    NODE *nodes_fieldDefinitions = malloc(3 * sizeof(NODE));

    nodes_fieldDefinitions[0] = (NODE){"fieldDefinition", false, &nodes_fieldDefinitions[1]};
    nodes_fieldDefinitions[1] = (NODE){"fieldDefinition", false, &nodes_fieldDefinitions[2]};
    nodes_fieldDefinitions[2] = (NODE){"moreFields", false, NULL};

    addRule(grammar, "fieldDefinitions", nodes_fieldDefinitions);

    // <fieldDefinition>===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    NODE *nodes_fieldDefinition = malloc(5 * sizeof(NODE));

    nodes_fieldDefinition[0] = (NODE){"TK_TYPE", true, &nodes_fieldDefinition[1]};
    nodes_fieldDefinition[1] = (NODE){"fieldType", false, &nodes_fieldDefinition[2]};
    nodes_fieldDefinition[2] = (NODE){"TK_COLON", true, &nodes_fieldDefinition[3]};
    nodes_fieldDefinition[3] = (NODE){"TK_FIELDID", true, &nodes_fieldDefinition[4]};
    nodes_fieldDefinition[4] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "fieldDefinition", nodes_fieldDefinition);

    // <fieldType>===> <primitiveDatatype> | <constructedDatatype>
    NODE *nodes_fieldType_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_fieldType_2 = malloc(1 * sizeof(NODE));

    nodes_fieldType_1[0] = (NODE){"primitiveDatatype", false, NULL};
    nodes_fieldType_2[0] = (NODE){"constructedDatatype", false, NULL};

    addRule(grammar, "fieldType", nodes_fieldType_1);
    addRule(grammar, "fieldType", nodes_fieldType_2);

    // <moreFields>===><fieldDefinition><moreFields> | ε
    NODE *nodes_moreFields_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_moreFields_2 = malloc(1 * sizeof(NODE));

    nodes_moreFields_1[0] = (NODE){"fieldDefinition", false, &nodes_moreFields_1[1]};
    nodes_moreFields_1[1] = (NODE){"moreFields", false, NULL};

    nodes_moreFields_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "moreFields", nodes_moreFields_1);
    addRule(grammar, "moreFields", nodes_moreFields_2);

    // <declarations> ===> <declaration><declarations>| ε
    NODE *nodes_declarations_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_declarations_2 = malloc(1 * sizeof(NODE));

    nodes_declarations_1[0] = (NODE){"declaration", false, &nodes_declarations_1[1]};
    nodes_declarations_1[1] = (NODE){"declarations", false, NULL};

    nodes_declarations_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "declarations", nodes_declarations_1);
    addRule(grammar, "declarations", nodes_declarations_2);

    // <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    NODE *nodes_declaration = malloc(6 * sizeof(NODE));

    nodes_declaration[0] = (NODE){"TK_TYPE", true, &nodes_declaration[1]};
    nodes_declaration[1] = (NODE){"dataType", false, &nodes_declaration[2]};
    nodes_declaration[2] = (NODE){"TK_COLON", true, &nodes_declaration[3]};
    nodes_declaration[3] = (NODE){"TK_ID", true, &nodes_declaration[4]};
    nodes_declaration[4] = (NODE){"global_or_not", false, &nodes_declaration[5]};
    nodes_declaration[5] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "declaration", nodes_declaration);

    // <global_or_not>===>TK_COLON TK_GLOBAL| ε
    NODE *nodes_global_or_not_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_global_or_not_2 = malloc(1 * sizeof(NODE));

    nodes_global_or_not_1[0] = (NODE){"TK_COLON", true, &nodes_global_or_not_1[1]};
    nodes_global_or_not_1[1] = (NODE){"TK_GLOBAL", true, NULL};

    nodes_global_or_not_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "global_or_not", nodes_global_or_not_1);
    addRule(grammar, "global_or_not", nodes_global_or_not_2);

    // <otherStmts>===> <stmt><otherStmts> | ε
    NODE *nodes_otherStmts_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_otherStmts_2 = malloc(1 * sizeof(NODE));

    nodes_otherStmts_1[0] = (NODE){"stmt", false, &nodes_otherStmts_1[1]};
    nodes_otherStmts_1[1] = (NODE){"otherStmts", false, NULL};

    nodes_otherStmts_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "otherStmts", nodes_otherStmts_1);
    addRule(grammar, "otherStmts", nodes_otherStmts_2);

    // <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
    NODE *nodes_stmt_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_stmt_2 = malloc(1 * sizeof(NODE));
    NODE *nodes_stmt_3 = malloc(1 * sizeof(NODE));
    NODE *nodes_stmt_4 = malloc(1 * sizeof(NODE));
    NODE *nodes_stmt_5 = malloc(1 * sizeof(NODE));

    nodes_stmt_1[0] = (NODE){"assignmentStmt", false, NULL};
    nodes_stmt_2[0] = (NODE){"iterativeStmt", false, NULL};
    nodes_stmt_3[0] = (NODE){"conditionalStmt", false, NULL};
    nodes_stmt_4[0] = (NODE){"ioStmt", false, NULL};
    nodes_stmt_5[0] = (NODE){"funCallStmt", false, NULL};

    addRule(grammar, "stmt", nodes_stmt_1);
    addRule(grammar, "stmt", nodes_stmt_2);
    addRule(grammar, "stmt", nodes_stmt_3);
    addRule(grammar, "stmt", nodes_stmt_4);
    addRule(grammar, "stmt", nodes_stmt_5);

    // <assignmentStmt>===><SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    NODE *nodes_assignmentStmt = malloc(4 * sizeof(NODE));

    nodes_assignmentStmt[0] = (NODE){"SingleOrRecId", false, &nodes_assignmentStmt[1]};
    nodes_assignmentStmt[1] = (NODE){"TK_ASSIGNOP", true, &nodes_assignmentStmt[2]};
    nodes_assignmentStmt[2] = (NODE){"arithmeticExpression", false, &nodes_assignmentStmt[3]};
    nodes_assignmentStmt[3] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "assignmentStmt", nodes_assignmentStmt);

    // <SingleOrRecId>===>TK_ID <option_single_constructed>
    NODE *nodes_singleOrRecId = malloc(2 * sizeof(NODE));

    nodes_singleOrRecId[0] = (NODE){"TK_ID", true, &nodes_singleOrRecId[1]};
    nodes_singleOrRecId[1] = (NODE){"option_single_constructed", false, NULL};

    addRule(grammar, "SingleOrRecId", nodes_singleOrRecId);

    // <option_single_constructed>===> ε | <oneExpansion><moreExpansions>
    NODE *nodes_optionSingleConstructed_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_optionSingleConstructed_2 = malloc(2 * sizeof(NODE));

    nodes_optionSingleConstructed_1[0] = (NODE){"ε", true, NULL};

    nodes_optionSingleConstructed_2[0] = (NODE){"oneExpansion", false, &nodes_optionSingleConstructed_2[1]};
    nodes_optionSingleConstructed_2[1] = (NODE){"moreExpansions", false, NULL};

    addRule(grammar, "option_single_constructed", nodes_optionSingleConstructed_1);
    addRule(grammar, "option_single_constructed", nodes_optionSingleConstructed_2);

    // <oneExpansion>===> TK_DOT TK_FIELDID
    NODE *nodes_oneExpansion = malloc(2 * sizeof(NODE));

    nodes_oneExpansion[0] = (NODE){"TK_DOT", true, &nodes_oneExpansion[1]};
    nodes_oneExpansion[1] = (NODE){"TK_FIELDID", true, NULL};

    addRule(grammar, "oneExpansion", nodes_oneExpansion);

    // <moreExpansions>===> <oneExpansion> <moreExpansions> | ε
    NODE *nodes_moreExpansions_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_moreExpansions_2 = malloc(1 * sizeof(NODE));

    nodes_moreExpansions_1[0] = (NODE){"oneExpansion", false, &nodes_moreExpansions_1[1]};
    nodes_moreExpansions_1[1] = (NODE){"moreExpansions", false, NULL};

    nodes_moreExpansions_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "moreExpansions", nodes_moreExpansions_1);
    addRule(grammar, "moreExpansions", nodes_moreExpansions_2);

    // <funCallStmt>===><outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    NODE *nodes_funCallStmt = malloc(7 * sizeof(NODE));

    nodes_funCallStmt[0] = (NODE){"outputParameters", false, &nodes_funCallStmt[1]};
    nodes_funCallStmt[1] = (NODE){"TK_CALL", true, &nodes_funCallStmt[2]};
    nodes_funCallStmt[2] = (NODE){"TK_FUNID", true, &nodes_funCallStmt[3]};
    nodes_funCallStmt[3] = (NODE){"TK_WITH", true, &nodes_funCallStmt[4]};
    nodes_funCallStmt[4] = (NODE){"TK_PARAMETERS", true, &nodes_funCallStmt[5]};
    nodes_funCallStmt[5] = (NODE){"inputParameters", false, &nodes_funCallStmt[6]};
    nodes_funCallStmt[6] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "funCallStmt", nodes_funCallStmt);

    // <outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP | ε
    NODE *nodes_outputParameters_1 = malloc(4 * sizeof(NODE));
    NODE *nodes_outputParameters_2 = malloc(1 * sizeof(NODE));

    nodes_outputParameters_1[0] = (NODE){"TK_SQL", true, &nodes_outputParameters_1[1]};
    nodes_outputParameters_1[1] = (NODE){"idList", false, &nodes_outputParameters_1[2]};
    nodes_outputParameters_1[2] = (NODE){"TK_SQR", true, &nodes_outputParameters_1[3]};
    nodes_outputParameters_1[3] = (NODE){"TK_ASSIGNOP", true, NULL};

    nodes_outputParameters_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "outputParameters", nodes_outputParameters_1);
    addRule(grammar, "outputParameters", nodes_outputParameters_2);

    // <inputParameters>===> TK_SQL <idList> TK_SQR
    NODE *nodes_inputParameters = malloc(3 * sizeof(NODE));

    nodes_inputParameters[0] = (NODE){"TK_SQL", true, &nodes_inputParameters[1]};
    nodes_inputParameters[1] = (NODE){"idList", false, &nodes_inputParameters[2]};
    nodes_inputParameters[2] = (NODE){"TK_SQR", true, NULL};

    addRule(grammar, "inputParameters", nodes_inputParameters);

    // <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt><otherStmts> TK_ENDWHILE
    NODE *nodes_iterativeStmt = malloc(7 * sizeof(NODE));

    nodes_iterativeStmt[0] = (NODE){"TK_WHILE", true, &nodes_iterativeStmt[1]};
    nodes_iterativeStmt[1] = (NODE){"TK_OP", true, &nodes_iterativeStmt[2]};
    nodes_iterativeStmt[2] = (NODE){"booleanExpression", false, &nodes_iterativeStmt[3]};
    nodes_iterativeStmt[3] = (NODE){"TK_CL", true, &nodes_iterativeStmt[4]};
    nodes_iterativeStmt[4] = (NODE){"stmt", false, &nodes_iterativeStmt[5]};
    nodes_iterativeStmt[5] = (NODE){"otherStmts", false, &nodes_iterativeStmt[6]};
    nodes_iterativeStmt[6] = (NODE){"TK_ENDWHILE", true, NULL};

    addRule(grammar, "iterativeStmt", nodes_iterativeStmt);

    // <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt><otherStmts> <elsePart>
    NODE *nodes_conditionalStmt = malloc(8 * sizeof(NODE));

    nodes_conditionalStmt[0] = (NODE){"TK_IF", true, &nodes_conditionalStmt[1]};
    nodes_conditionalStmt[1] = (NODE){"TK_OP", true, &nodes_conditionalStmt[2]};
    nodes_conditionalStmt[2] = (NODE){"booleanExpression", false, &nodes_conditionalStmt[3]};
    nodes_conditionalStmt[3] = (NODE){"TK_CL", true, &nodes_conditionalStmt[4]};
    nodes_conditionalStmt[4] = (NODE){"TK_THEN", true, &nodes_conditionalStmt[5]};
    nodes_conditionalStmt[5] = (NODE){"stmt", false, &nodes_conditionalStmt[6]};
    nodes_conditionalStmt[6] = (NODE){"otherStmts", false, &nodes_conditionalStmt[7]};
    nodes_conditionalStmt[7] = (NODE){"elsePart", false, NULL};

    addRule(grammar, "conditionalStmt", nodes_conditionalStmt);

    // <elsePart>===>TK_ELSE <stmt><otherStmts> TK_ENDIF | TK_ENDIF
    NODE *nodes_elsePart_1 = malloc(4 * sizeof(NODE));
    NODE *nodes_elsePart_2 = malloc(1 * sizeof(NODE));

    nodes_elsePart_1[0] = (NODE){"TK_ELSE", true, &nodes_elsePart_1[1]};
    nodes_elsePart_1[1] = (NODE){"stmt", false, &nodes_elsePart_1[2]};
    nodes_elsePart_1[2] = (NODE){"otherStmts", false, &nodes_elsePart_1[3]};
    nodes_elsePart_1[3] = (NODE){"TK_ENDIF", true, NULL};

    nodes_elsePart_2[0] = (NODE){"TK_ENDIF", true, NULL};

    addRule(grammar, "elsePart", nodes_elsePart_1);
    addRule(grammar, "elsePart", nodes_elsePart_2);

    // <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
    NODE *nodes_ioStmt_1 = malloc(5 * sizeof(NODE));
    NODE *nodes_ioStmt_2 = malloc(5 * sizeof(NODE));

    nodes_ioStmt_1[0] = (NODE){"TK_READ", true, &nodes_ioStmt_1[1]};
    nodes_ioStmt_1[1] = (NODE){"TK_OP", true, &nodes_ioStmt_1[2]};
    nodes_ioStmt_1[2] = (NODE){"var", false, &nodes_ioStmt_1[3]};
    nodes_ioStmt_1[3] = (NODE){"TK_CL", true, &nodes_ioStmt_1[4]};
    nodes_ioStmt_1[4] = (NODE){"TK_SEM", true, NULL};

    nodes_ioStmt_2[0] = (NODE){"TK_WRITE", true, &nodes_ioStmt_2[1]};
    nodes_ioStmt_2[1] = (NODE){"TK_OP", true, &nodes_ioStmt_2[2]};
    nodes_ioStmt_2[2] = (NODE){"var", false, &nodes_ioStmt_2[3]};
    nodes_ioStmt_2[3] = (NODE){"TK_CL", true, &nodes_ioStmt_2[4]};
    nodes_ioStmt_2[4] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "ioStmt", nodes_ioStmt_1);
    addRule(grammar, "ioStmt", nodes_ioStmt_2);

    // <arithmeticExpression> ===> <term> <expPrime>
    NODE *nodes_arithmeticExpression = malloc(2 * sizeof(NODE));

    nodes_arithmeticExpression[0] = (NODE){"term", false, &nodes_arithmeticExpression[1]};
    nodes_arithmeticExpression[1] = (NODE){"expPrime", false, NULL};

    addRule(grammar, "arithmeticExpression", nodes_arithmeticExpression);

    // <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime> | ε
    NODE *nodes_expPrime_1 = malloc(3 * sizeof(NODE));
    NODE *nodes_expPrime_2 = malloc(1 * sizeof(NODE));

    nodes_expPrime_1[0] = (NODE){"lowPrecedenceOperators", false, &nodes_expPrime_1[1]};
    nodes_expPrime_1[1] = (NODE){"term", false, &nodes_expPrime_1[2]};
    nodes_expPrime_1[2] = (NODE){"expPrime", false, NULL};

    nodes_expPrime_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "expPrime", nodes_expPrime_1);
    addRule(grammar, "expPrime", nodes_expPrime_2);

    // <term>===> <factor> <termPrime>
    NODE *nodes_term = malloc(2 * sizeof(NODE));

    nodes_term[0] = (NODE){"factor", false, &nodes_term[1]};
    nodes_term[1] = (NODE){"termPrime", false, NULL};

    addRule(grammar, "term", nodes_term);

    // <termPrime> ===> <highPrecedenceOperators><factor> <termPrime> | ε
    NODE *nodes_termPrime_1 = malloc(3 * sizeof(NODE));
    NODE *nodes_termPrime_2 = malloc(1 * sizeof(NODE));

    nodes_termPrime_1[0] = (NODE){"highPrecedenceOperators", false, &nodes_termPrime_1[1]};
    nodes_termPrime_1[1] = (NODE){"factor", false, &nodes_termPrime_1[2]};
    nodes_termPrime_1[2] = (NODE){"termPrime", false, NULL};

    nodes_termPrime_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "termPrime", nodes_termPrime_1);
    addRule(grammar, "termPrime", nodes_termPrime_2);

    // <factor> ===> TK_OP <arithmeticExpression> TK_CL | <var>
    NODE *nodes_factor_1 = malloc(3 * sizeof(NODE));
    NODE *nodes_factor_2 = malloc(1 * sizeof(NODE));

    nodes_factor_1[0] = (NODE){"TK_OP", true, &nodes_factor_1[1]};
    nodes_factor_1[1] = (NODE){"arithmeticExpression", false, &nodes_factor_1[2]};
    nodes_factor_1[2] = (NODE){"TK_CL", true, NULL};

    nodes_factor_2[0] = (NODE){"var", false, NULL};

    addRule(grammar, "factor", nodes_factor_1);
    addRule(grammar, "factor", nodes_factor_2);

    // <highPrecedenceOperators>===> TK_MUL | TK_DIV
    NODE *nodes_highPrecedenceOperators_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_highPrecedenceOperators_2 = malloc(1 * sizeof(NODE));

    nodes_highPrecedenceOperators_1[0] = (NODE){"TK_MUL", true, NULL};

    nodes_highPrecedenceOperators_2[0] = (NODE){"TK_DIV", true, NULL};

    addRule(grammar, "highPrecedenceOperators", nodes_highPrecedenceOperators_1);
    addRule(grammar, "highPrecedenceOperators", nodes_highPrecedenceOperators_2);

    // <lowPrecedenceOperators> ===> TK_PLUS | TK_MINUS
    NODE *nodes_lowPrecedenceOperators_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_lowPrecedenceOperators_2 = malloc(1 * sizeof(NODE));

    nodes_lowPrecedenceOperators_1[0] = (NODE){"TK_PLUS", true, NULL};

    nodes_lowPrecedenceOperators_2[0] = (NODE){"TK_MINUS", true, NULL};

    addRule(grammar, "lowPrecedenceOperators", nodes_lowPrecedenceOperators_1);
    addRule(grammar, "lowPrecedenceOperators", nodes_lowPrecedenceOperators_2);

    // <booleanExpression>===>TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL
    NODE *nodes_booleanExpression_1 = malloc(7 * sizeof(NODE));
    NODE *nodes_booleanExpression_2 = malloc(3 * sizeof(NODE));
    NODE *nodes_booleanExpression_3 = malloc(4 * sizeof(NODE));

    nodes_booleanExpression_1[0] = (NODE){"TK_OP", true, &nodes_booleanExpression_1[1]};
    nodes_booleanExpression_1[1] = (NODE){"booleanExpression", false, &nodes_booleanExpression_1[2]};
    nodes_booleanExpression_1[2] = (NODE){"TK_CL", true, &nodes_booleanExpression_1[3]};
    nodes_booleanExpression_1[3] = (NODE){"logicalOp", false, &nodes_booleanExpression_1[4]};
    nodes_booleanExpression_1[4] = (NODE){"TK_OP", true, &nodes_booleanExpression_1[5]};
    nodes_booleanExpression_1[5] = (NODE){"booleanExpression", false, &nodes_booleanExpression_1[6]};
    nodes_booleanExpression_1[6] = (NODE){"TK_CL", true, NULL};

    nodes_booleanExpression_2[0] = (NODE){"var", false, &nodes_booleanExpression_2[1]};
    nodes_booleanExpression_2[1] = (NODE){"relationalOp", false, &nodes_booleanExpression_2[2]};
    nodes_booleanExpression_2[2] = (NODE){"var", false, NULL};

    nodes_booleanExpression_3[0] = (NODE){"TK_NOT", true, &nodes_booleanExpression_3[1]};
    nodes_booleanExpression_3[1] = (NODE){"TK_OP", true, &nodes_booleanExpression_3[2]};
    nodes_booleanExpression_3[2] = (NODE){"booleanExpression", false, &nodes_booleanExpression_3[3]};
    nodes_booleanExpression_3[3] = (NODE){"TK_CL", true, NULL};

    addRule(grammar, "booleanExpression", nodes_booleanExpression_1);
    addRule(grammar, "booleanExpression", nodes_booleanExpression_2);
    addRule(grammar, "booleanExpression", nodes_booleanExpression_3);

    // <var>===> <SingleOrRecId> | TK_NUM | TK_RNUM
    NODE *nodes_var_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_var_2 = malloc(1 * sizeof(NODE));
    NODE *nodes_var_3 = malloc(1 * sizeof(NODE));

    nodes_var_1[0] = (NODE){"SingleOrRecId", false, NULL};
    nodes_var_2[0] = (NODE){"TK_NUM", true, NULL};
    nodes_var_3[0] = (NODE){"TK_RNUM", true, NULL};

    addRule(grammar, "var", nodes_var_1);
    addRule(grammar, "var", nodes_var_2);
    addRule(grammar, "var", nodes_var_3);

    // <logicalOp>===>TK_AND | TK_OR
    NODE *nodes_logicalOp_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_logicalOp_2 = malloc(1 * sizeof(NODE));

    nodes_logicalOp_1[0] = (NODE){"TK_AND", true, NULL};
    nodes_logicalOp_2[0] = (NODE){"TK_OR", true, NULL};

    addRule(grammar, "logicalOp", nodes_logicalOp_1);
    addRule(grammar, "logicalOp", nodes_logicalOp_2);

    // <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
    NODE *nodes_relationalOp_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_relationalOp_2 = malloc(1 * sizeof(NODE));
    NODE *nodes_relationalOp_3 = malloc(1 * sizeof(NODE));
    NODE *nodes_relationalOp_4 = malloc(1 * sizeof(NODE));
    NODE *nodes_relationalOp_5 = malloc(1 * sizeof(NODE));
    NODE *nodes_relationalOp_6 = malloc(1 * sizeof(NODE));

    nodes_relationalOp_1[0] = (NODE){"TK_LT", true, NULL};
    nodes_relationalOp_2[0] = (NODE){"TK_LE", true, NULL};
    nodes_relationalOp_3[0] = (NODE){"TK_EQ", true, NULL};
    nodes_relationalOp_4[0] = (NODE){"TK_GT", true, NULL};
    nodes_relationalOp_5[0] = (NODE){"TK_GE", true, NULL};
    nodes_relationalOp_6[0] = (NODE){"TK_NE", true, NULL};

    addRule(grammar, "relationalOp", nodes_relationalOp_1);
    addRule(grammar, "relationalOp", nodes_relationalOp_2);
    addRule(grammar, "relationalOp", nodes_relationalOp_3);
    addRule(grammar, "relationalOp", nodes_relationalOp_4);
    addRule(grammar, "relationalOp", nodes_relationalOp_5);
    addRule(grammar, "relationalOp", nodes_relationalOp_6);

    // <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
    NODE *nodes_returnStmt = malloc(3 * sizeof(NODE));

    nodes_returnStmt[0] = (NODE){"TK_RETURN", true, &nodes_returnStmt[1]};
    nodes_returnStmt[1] = (NODE){"optionalReturn", false, &nodes_returnStmt[2]};
    nodes_returnStmt[2] = (NODE){"TK_SEM", true, NULL};

    addRule(grammar, "returnStmt", nodes_returnStmt);

    // <optionalReturn>===>TK_SQL <idList> TK_SQR | ε
    NODE *nodes_optionalReturn_1 = malloc(3 * sizeof(NODE));
    NODE *nodes_optionalReturn_2 = malloc(1 * sizeof(NODE));

    nodes_optionalReturn_1[0] = (NODE){"TK_SQL", true, &nodes_optionalReturn_1[1]};
    nodes_optionalReturn_1[1] = (NODE){"idList", false, &nodes_optionalReturn_1[2]};
    nodes_optionalReturn_1[2] = (NODE){"TK_SQR", true, NULL};

    nodes_optionalReturn_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "optionalReturn", nodes_optionalReturn_1);
    addRule(grammar, "optionalReturn", nodes_optionalReturn_2);

    // <idList>===> TK_ID <more_ids>
    NODE *nodes_idList = malloc(2 * sizeof(NODE));

    nodes_idList[0] = (NODE){"TK_ID", true, &nodes_idList[1]};
    nodes_idList[1] = (NODE){"more_ids", false, NULL};

    addRule(grammar, "idList", nodes_idList);

    // <more_ids>===> TK_COMMA <idList> | ε
    NODE *nodes_moreIds_1 = malloc(2 * sizeof(NODE));
    NODE *nodes_moreIds_2 = malloc(1 * sizeof(NODE));

    nodes_moreIds_1[0] = (NODE){"TK_COMMA", true, &nodes_moreIds_1[1]};
    nodes_moreIds_1[1] = (NODE){"idList", false, NULL};

    nodes_moreIds_2[0] = (NODE){"ε", true, NULL};

    addRule(grammar, "more_ids", nodes_moreIds_1);
    addRule(grammar, "more_ids", nodes_moreIds_2);

    // <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    NODE *nodes_definetypestmt = malloc(5 * sizeof(NODE));

    nodes_definetypestmt[0] = (NODE){"TK_DEFINETYPE", true, &nodes_definetypestmt[1]};
    nodes_definetypestmt[1] = (NODE){"A", false, &nodes_definetypestmt[2]};
    nodes_definetypestmt[2] = (NODE){"TK_RUID", true, &nodes_definetypestmt[3]};
    nodes_definetypestmt[3] = (NODE){"TK_AS", true, &nodes_definetypestmt[4]};
    nodes_definetypestmt[4] = (NODE){"TK_RUID", true, NULL};

    addRule(grammar, "definetypestmt", nodes_definetypestmt);

    // <A>==>TK_RECORD | TK_UNION
    NODE *nodes_A_1 = malloc(1 * sizeof(NODE));
    NODE *nodes_A_2 = malloc(1 * sizeof(NODE));

    nodes_A_1[0] = (NODE){"TK_RECORD", true, NULL};
    nodes_A_2[0] = (NODE){"TK_UNION", true, NULL};

    addRule(grammar, "A", nodes_A_1);
    addRule(grammar, "A", nodes_A_2);
}

NODE ***initPredictiveParsingTable()
{

    char *Terminals[] = {
        "TK_MAIN", "TK_END", "TK_FUNID", "TK_SEM", "TK_INPUT",
        "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_OUTPUT",
        "TK_COMMA", "TK_ID", "TK_INT", "TK_REAL", "TK_RECORD",
        "TK_RUID", "TK_UNION", "TK_TYPE", "TK_COLON", "TK_FIELDID",
        "TK_ENDRECORD", "TK_ENDUNION", "TK_GLOBAL", "TK_ASSIGNOP",
        "TK_DOT", "TK_CALL", "TK_WITH", "TK_PARAMETERS", "TK_OP",
        "TK_CL", "TK_WHILE", "TK_ENDWHILE", "TK_IF", "TK_THEN",
        "TK_ELSE", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_NUM",
        "TK_RNUM", "TK_MUL", "TK_DIV", "TK_PLUS", "TK_MINUS",
        "TK_AND", "TK_OR", "TK_LT", "TK_LE", "TK_EQ", "TK_GT",
        "TK_GE", "TK_NE", "TK_RETURN", "TK_DEFINETYPE", "TK_AS","TK_NOT",
        NULL // NULL terminator to indicate end of array
    };

    char *NonTerminals[] = {
        "program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list",
        "dataType", "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions",
        "actualOrRedefined", "typeDefinition", "fieldDefinitions", "fieldDefinition", "fieldType", "moreFields",
        "declarations", "declaration", "global_or_not", "otherStmts", "stmt", "assignmentStmt", "SingleOrRecId",
        "option_single_constructed", "oneExpansion", "moreExpansions", "funCallStmt", "outputParameters",
        "inputParameters", "iterativeStmt", "conditionalStmt", "elsePart", "ioStmt", "arithmeticExpression",
        "expPrime", "term", "termPrime", "factor", "highPrecedenceOperators", "lowPrecedenceOperators",
        "booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", "idList",
        "more_ids", "definetypestmt", "A", NULL};

    GRAMMAR *grammar = (GRAMMAR *)malloc(sizeof(GRAMMAR));
    initGrammer(grammar);

    NODE *nodes_error = malloc(1 * sizeof(NODE));
    nodes_error[0] = (NODE){"error", false, NULL};

    NODE *nodes_syn = malloc(1 * sizeof(NODE));
    nodes_syn[0] = (NODE){"syn", false, NULL};

    NODE *nodes_e = malloc(1 * sizeof(NODE));
    nodes_e[0] = (NODE){"ε", true, NULL};

    // Initializing first set
    DictionaryParser *firstSet = createDictionaryParser();
    initFirst(firstSet);

    // Initializing follow set
    DictionaryParser *followSet = createDictionaryParser();
    initFollow(followSet);

    NODE ***predictiveParsingTable = (NODE ***)malloc(MAX_NON_TERMINALS * sizeof(NODE **));

    // Initializing predictive parsing table
    for (int i = 0; i < MAX_NON_TERMINALS; i++)
    {
        predictiveParsingTable[i] = (NODE **)malloc(MAX_TERMINALS * sizeof(NODE *));
        for (int j = 0; j < MAX_TERMINALS; j++)
        {
            predictiveParsingTable[i][j] = NULL;
        }
    }

    for (int i = 0; i < 53; i++)
    {
        for (int j = 0; j < 56; j++)
        {
            char *nonTerminal = NonTerminals[i];
            char *terminal = Terminals[j];
            if (searchF(firstSet, nonTerminal, terminal) == -1)
            {
                if (searchF(followSet, nonTerminal, terminal) == 1)
                {
                    predictiveParsingTable[hashNT(nonTerminal)][hashT(terminal)] = nodes_syn;
                }
                else
                {
                    predictiveParsingTable[hashNT(nonTerminal)][hashT(terminal)] = nodes_error;
                }
            }
            else if (searchF(firstSet, nonTerminal, terminal) == 0)
            {
                NODE *rulesList = grammar->rules[hashNT(nonTerminal)]->heads;
                int rulesListLength = grammar->rules[hashNT(nonTerminal)]->length; 
                int flag=0;
                for(int k=0;k<rulesListLength;k++){
                    if((strcmp(rulesList[k].name,"ε")==0)){
                        flag=1;
                        predictiveParsingTable[hashNT(nonTerminal)][hashT(terminal)] = nodes_e;
                        break;
                    }
                }
                if(flag==0){
                   for(int k=0;k<rulesListLength;k++){
                        if(searchF(firstSet, rulesList[k].name, "ε") == 1 && searchF(followSet, rulesList[k].name, "ε") == 1){
                            predictiveParsingTable[hashNT(nonTerminal)][hashT(terminal)] = &rulesList[k];
                            break;
                        }
                    }
                }
                
            }
            else
            {
                NODE *rulesList = grammar->rules[hashNT(nonTerminal)]->heads;
                int rulesListLength = grammar->rules[hashNT(nonTerminal)]->length;
                for (int k = 0; k < rulesListLength; k++)
                {
                    NODE rule = rulesList[k];
                    if ((!rule.terminal && searchF(firstSet, rule.name, terminal) == 1) || (rule.terminal && strcmp(rule.name, terminal) == 0) || (!rule.terminal && searchF(firstSet, rule.name, terminal) == 0 && searchF(followSet, rule.name, terminal) == 1))
                    {
                        predictiveParsingTable[hashNT(nonTerminal)][hashT(terminal)] = &rulesList[k];
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 53; i++)
    {
        char *nonTerminal = NonTerminals[i];
        if (searchF(firstSet, nonTerminal, "ε") == 1)
        {
            predictiveParsingTable[hashNT(nonTerminal)][hashT("ε")] = nodes_e;
        }
        else
        {
            predictiveParsingTable[hashNT(nonTerminal)][hashT("ε")] = nodes_error;
        }
    }

    return predictiveParsingTable;
}

// Function to create a new node with given NODE data
StackNode *createStackNode(NODE *data)
{
    StackNode *newStackNode = (StackNode *)malloc(sizeof(StackNode));
    if (newStackNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newStackNode->data = data;
    newStackNode->next = NULL;
    return newStackNode;
}

// Function to initialize the stack
void initializeStack(Stack *stack)
{
    stack->top = NULL;
    NODE *nodes_program = malloc(1 * sizeof(NODE));
    nodes_program[0] = (NODE){"program", false, NULL};
    push(stack, nodes_program);
}

// Function to push a NODE pointer onto the stack
void push(Stack *stack, NODE *data)
{
    StackNode *newStackNode = createStackNode(data);
    newStackNode->next = stack->top;
    stack->top = newStackNode;
}

// Function to pop a NODE pointer from the stack
NODE *pop(Stack *stack)
{
    if (stack->top == NULL)
    {
        printf("Stack underflow\n");
        exit(1);
    }
    StackNode *temp = stack->top;
    NODE *data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

// Function to check if the stack is empty
bool isEmpty(Stack *stack)
{
    return stack->top == NULL;
}

// Function to display the elements of the stack (for debugging)
void display(Stack *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack is empty\n");
        return;
    }
    StackNode *current = stack->top;
    printf("Stack elements:\n");
    while (current != NULL)
    {
        printf("Name: %s, Terminal: %s\n",
               current->data->name, current->data->terminal ? "true" : "false");
        current = current->next;
    }
    printf("\n");
}

// Function to free memory allocated for the stack nodes
void freeStack(Stack *stack)
{
    while (!isEmpty(stack))
    {
        StackNode *temp = stack->top;
        stack->top = temp->next;
        free(temp); // Free the StackNode itself
    }
}

// Function to create a new tree node
TreeNode *createTreeNode(char *name)
{
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->name = strdup(name); // Allocate memory for name and copy the string
    if (newNode->name == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->parent = NULL;
    newNode->sibling = NULL;
    newNode->num_children = 0;
    newNode->index_in_parent = -1;
    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to add a child to a parent node
void addChild(TreeNode *parent, TreeNode *child)
{
    if (parent->num_children >= MAX_CHILDREN)
    {
        fprintf(stderr, "Maximum number of children reached\n");
        exit(EXIT_FAILURE);
    }
    child->parent = parent;
    child->index_in_parent = parent->num_children;
    if (parent->num_children > 0)
    {
        child->sibling = parent->children[parent->num_children - 1]->sibling;
        parent->children[parent->num_children - 1]->sibling = child;
    } else {
        child->sibling = parent->sibling;
        parent->sibling = child;
    }
    parent->children[parent->num_children++] = child;
}

// Function to print the tree recursively
void printTree(TreeNode *root, int depth)
{
    if (root == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", root->name);
    for (int i = 0; i < root->num_children; i++)
    {
        printTree(root->children[i], depth + 1);
    }
}

//Inorder Traversal
void inorderTraversal(TreeNode *root)
{
    if (root == NULL)
        return;
    if(root->num_children==0)
    {
        printf("%s", root->name);
        return;
    } 
    inorderTraversal(root->children[0]);
    printf(" --> ");
    printf("%s", root->name);
    for (int i = 1; i < root->num_children; i++)
    {
        printf(" --> ");
        inorderTraversal(root->children[i]);
    }

    return;
}

// Function to free the memory allocated for the tree nodes
void freeTree(TreeNode *root)
{
    if (root == NULL)
        return;
    for (int i = 0; i < root->num_children; i++)
    {
        freeTree(root->children[i]);
    }
    free(root->name);
    free(root);
}

TreeNode *getNextSibling(TreeNode *node)
{
    if (node == NULL)
        return NULL; // No parent or invalid index_in_parent means no siblings

    return node->sibling; // Recursively search for next sibling of parent
}

void addRuleToStack(Stack *stack, NODE *rule, TreeNode **cur)
{
    Stack s;
    s.top = NULL;
    NODE *current = rule;
    while (current != NULL)
    {
        push(&s, current);
        TreeNode *temp = createTreeNode(current->name);
        addChild(*cur, temp);
        current = current->next;
    }
    while (isEmpty(&s) != true)
    {
        push(stack, pop(&s));
    }
    freeStack(&s);
}

void processToken(Stack *stack, NODE ***predictiveParsingTable, TokenInfo Token, TreeNode **current)
{
    while (true)
    {
        if (isEmpty(stack))
        {
            printf("Line %d Error: Invalid token %s encountered with value %s stack top NULL\n",Token.lineNumber,Token.type,Token.lexeme); 
            return;
        }
        else if (stack->top->data->terminal)
        {
            if (strcmp(stack->top->data->name, Token.type) == 0)
            {
                pop(stack);
                *current = getNextSibling(*current);
                return;
            }
            else
            {
                printf("Line %d Error: The token %s for lexeme %s  does not match with the expected token %s\n",Token.lineNumber,Token.type,Token.lexeme,stack->top->data->name); // give error
                pop(stack);
                *current = getNextSibling(*current);
            }
        }
        else
        {
            NODE *rule = predictiveParsingTable[hashNT(stack->top->data->name)][hashT(Token.type)];
            if (rule == NULL)
            {
                printf("Error: No rule found\n"); 
                return;
            }
            else if (!strcmp(rule->name, "error"))
            {
                printf("Line %d Error : Invalid token %s encountered with value %s stack top %s\n",Token.lineNumber,Token.type,Token.lexeme,stack->top->data->name);
                return;
            }
            else if (!strcmp(rule->name, "syn"))
            {
                printf("Line %d Error : Invalid token %s encountered with value %s stack top %s\n",Token.lineNumber,Token.type,Token.lexeme,stack->top->data->name);
                pop(stack);
                *current = getNextSibling(*current);
            }
            else if (!strcmp(rule->name, "ε"))
            {
                pop(stack);
                TreeNode *temp = createTreeNode("ε");
                addChild(*current, temp);
                *current = getNextSibling(*current);
                *current = getNextSibling(*current);
            }
            else
            {
                pop(stack);
                addRuleToStack(stack, rule, current);
                *current = getNextSibling(*current);
            }
        }
    }
}


/*
// void parseInputSourceCode(char *testcaseFile, NODE ***T)
// {
//     Stack stack;
//     initializeStack(&stack);
//     TreeNode *root = createTreeNode("Root");

//     FILE *fp = fopen(testcaseFile, "r");
//     if (fp == NULL)
//     {
//         perror("Error opening file");
//     }

//     DictionaryLexer *dict = initLookupTable();

//     twinBuffer *B = (twinBuffer *)malloc(sizeof(twinBuffer));
//     initTwinBuffer(B, fp);

//     TokenInfo token;
//     do
//     {
//         token = getNextToken(B, fp, dict);
//         if (token.lexeme[0] == '\0')
//             break;
//         if (!strcmp(token.type, "TK_ERROR") || !strcmp(token.type, "TK_COMMENT"))
//             continue;

//         NODE *rule = T[hashNT(token.lexeme)][hashT(token.type)];
//         printf("%s\n", rule->name);

//         TreeNode **current = (TreeNode **)malloc(sizeof(TreeNode *));
//         *current = root;

//         addRuleToStack(&stack, rule, current);
//     } while (!token.end);

//     // display(&stack);
//     // printTree(root, 0);

//     fclose(fp);
//     free(B);
//     freeStack(&stack);
//     freeTree(root);
// }

// void parser(char *sourceFile, char *treeFile)
// {
//     NODE ***predictiveParsingTable = initPredictiveParsingTable();
//     parseInputSourceCode(sourceFile, predictiveParsingTable);
// }

*/

int main()
{
    NODE ***predictiveParsingTable = initPredictiveParsingTable();


    Stack stack;
    initializeStack(&stack);

    TreeNode *root = createTreeNode("program");
    TreeNode **current = (TreeNode **)malloc(sizeof(TreeNode *));
    *current = root;

    FILE *fp = fopen("t.txt", "r");

    if (fp == NULL)
        {
            perror("Error opening file");
        }

        DictionaryLexer *dict = initLookupTable();

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
                continue;
            }
            processToken(&stack, predictiveParsingTable, token, current);
        } while (!token.end);

        fclose(fp);
        free(B);
        freeStack(&stack);
        printTree(root, 0);
        printf("\n\n\n");
        inorderTraversal(root);
        freeTree(root);

    return 0;
}


