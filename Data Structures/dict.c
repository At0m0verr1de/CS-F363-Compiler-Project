#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dict.h"

int hash(char *key)
{
    int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hashValue += key[i];
    }
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
    int index = hash(key);
    if (dict->table[index] != NULL && strcmp(dict->table[index]->key, key) == 0)
    {
        return dict->table[index]->value;
    }
    return -1; // Key not found
}

void initLookupTable()
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
}
