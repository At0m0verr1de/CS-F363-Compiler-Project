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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEME_SIZE 1000
#define MAX_TOKEN_SIZE 15
#define FAF_TABLE_SIZE 84
#define MAX_RULE_LENGTH 5    // Maximum length of a production rule
#define MAX_NON_TERMINALS 100 // Maximum number of non-terminals in the grammar
#define MAX_TERMINALS 127 // Maximum number of terminals in the grammar
#define MAX_CHILDREN 15

typedef struct KeyValuePairParser
{
    char *key;
    char **value;
    int len;
} KeyValuePairParser;
typedef struct TreeNode
{
    char *type;
    char *lexeme;
    int line_number;
    //bool is_leaf;
    struct TreeNode *parent;
    struct TreeNode *children[MAX_CHILDREN];
    int num_children;
    int index_in_parent;
    struct TreeNode *sibling;
} TreeNode;

// StackNode structure for a term in a production rule
typedef struct node
{
    char name[50];
    bool terminal;
    struct node *next;
} NODE;

// Rules structure containing heads of production rules
typedef struct Rules
{
    NODE *heads;
    int length;
} RULES;

// Grammar structure
struct Grammar
{
    RULES *rules[FAF_TABLE_SIZE];
};

typedef struct Grammar * GRAMMAR;

// Define the structure for each node in the stack
typedef struct StackNode
{
    NODE *data;
    struct StackNode *next;
} StackNode;

// Define the stack structure
typedef struct
{
    StackNode *top;
} Stack;

typedef struct DictionaryParser
{
    struct KeyValuePairParser *table[FAF_TABLE_SIZE];
} DictionaryParser;

struct FirstFollow
{
    DictionaryParser *first;
    DictionaryParser *follow;
};

typedef struct FirstFollow * FirstAndFollow;
typedef struct node*** Table;
typedef struct TreeNode * parseTree;