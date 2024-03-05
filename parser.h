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
#include "parserDef.h"
#include "lexer.h"

// Function declarations

DictionaryParser *createDictionaryParser();
int hashNT(char *str);
void insertParser(DictionaryParser *dict, char *key, char **value, int len);
void initFirst(DictionaryParser *dict);
void initFollow(DictionaryParser *dict);
int searchF(DictionaryParser *dict, char *NT, char *T);
int hashT(char *str);
void initRules(RULES *rules);
void addRule(GRAMMAR grammar, char *nonTerminal, NODE *nodes);
RULES *getRules(GRAMMAR grammar, char *nonTerminal);
void printGrammar(GRAMMAR grammar);
void initGrammer(GRAMMAR grammar);
NODE ***initPredictiveParsingTable();
StackNode *createStackNode(NODE *data);
void initializeStack(Stack *stack);
void push(Stack *stack, NODE *data);
NODE *pop(Stack *stack);
bool isEmpty(Stack *stack);
void display(Stack *stack);
void freeStack(Stack *stack);
TreeNode *createTreeNode(char* token);
void addChild(TreeNode *parent, TreeNode *child);
void printTree(TreeNode *root, int depth);
void freeTree(TreeNode *root);
TreeNode *getNextSibling(TreeNode *node);
void addRuleToStack(Stack *stack, NODE *rule, TreeNode **cur);
void processToken(Stack *stack, NODE ***predictiveParsingTable, TokenInfo Token, TreeNode **current, bool *flag);
parseTree parseInputSourceCode(char *testcaseFile, NODE ***T);
void inorderTraversal(TreeNode *root);
FirstAndFollow ComputeFirstAndFollowSets(GRAMMAR G);
void createParseTable(FirstAndFollow F, Table predictiveParsingTable);
void printParseTree(parseTree root, char *outfile);
void printParseTreeHelper(TreeNode *node, FILE *outfile);