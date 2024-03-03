/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/

#define FAF_TABLE_SIZE 100
#define MAX_RULE_LENGTH 5    // Maximum length of a production rule
#define MAX_NON_TERMINALS 83 // Maximum number of non-terminals in the grammar
#define MAX_TERMINALS 125    // Maximum number of terminals in the grammar
#define MAX_CHILDREN 15

typedef struct TreeNode
{
    char *name;
    struct TreeNode *parent;
    struct TreeNode *children[MAX_CHILDREN];
    int num_children;
    int index_in_parent;
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
typedef struct Grammar
{
    RULES *rules[FAF_TABLE_SIZE];
} GRAMMAR;

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