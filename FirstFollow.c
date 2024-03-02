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
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 83

struct KeyValuePair
{
    char *key;
    char **value;
    int len;
};

struct Dictionary
{
    struct KeyValuePair *table[TABLE_SIZE];
};

/*
int hash(char *key)
{
    int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hashValue += (key[i]*i);
    }
    return hashValue % TABLE_SIZE;
}
*/

// Hash function
static unsigned int hash(register const char *str)
{
    static unsigned char asso_values[] =
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
    register unsigned int hval = strlen(str);

    switch (hval)
    {
    default:
        hval += asso_values[(unsigned char)str[1]];
    /*FALLTHROUGH*/
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
    // printf("%s %u\n", str , hval);
    return hval;
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

void insert(struct Dictionary *dict, char *key, char **value, int len)
{
    int index = hash(key);
    struct KeyValuePair *newPair = (struct KeyValuePair *)malloc(sizeof(struct KeyValuePair));
    newPair->key = strdup(key); // strdup dynamically allocates memory for the string
    newPair->value = value;
    newPair->len = len;
    dict->table[index] = newPair;
}

char **search(struct Dictionary *dict, char *key, int *len)
{
    int index = hash(key);
    if (dict->table[index] != NULL && strcmp(dict->table[index]->key, key) == 0)
    {
        *len = dict->table[index]->len;
        return dict->table[index]->value;
    }
    return NULL; // Key not found
}

void initFirst(struct Dictionary *dict)
{

    // Insert key-value pairs
    char **program = (char **)malloc(2 * sizeof(char *));
    program[0] = strdup("TK_MAIN");
    program[1] = strdup("TK_FUNID");
    insert(dict, strdup("program"), program, 2);

    // mainFunction
    char **mainFunction = (char **)malloc(1 * sizeof(char *));
    mainFunction[0] = strdup("TK_MAIN");
    insert(dict, strdup("mainFunction"), mainFunction, 1);

    // otherFunctions
    char **otherFunctions = (char **)malloc(2 * sizeof(char *));
    otherFunctions[0] = strdup("TK_FUNID");
    otherFunctions[1] = strdup("ε");
    insert(dict, strdup("otherFunctions"), otherFunctions, 2);

    // function
    char **function = (char **)malloc(1 * sizeof(char *));
    function[0] = strdup("TK_FUNID");
    insert(dict, strdup("function"), function, 1);

    // input_par
    char **input_par = (char **)malloc(1 * sizeof(char *));
    input_par[0] = strdup("TK_INPUT");
    insert(dict, strdup("input_par"), input_par, 1);

    // output_par
    char **output_par = (char **)malloc(2 * sizeof(char *));
    output_par[0] = strdup("TK_OUTPUT");
    output_par[1] = strdup("ε");
    insert(dict, strdup("output_par"), output_par, 2);

    // parameter_list
    char **parameter_list = (char **)malloc(5 * sizeof(char *));
    parameter_list[0] = strdup("TK_INT");
    parameter_list[1] = strdup("TK_REAL");
    parameter_list[2] = strdup("TK_RECORD");
    parameter_list[3] = strdup("TK_RUID");
    parameter_list[4] = strdup("TK_UNION");
    insert(dict, strdup("parameter_list"), parameter_list, 5);

    // dataType
    char **dataType = (char **)malloc(5 * sizeof(char *));
    dataType[0] = strdup("TK_INT");
    dataType[1] = strdup("TK_REAL");
    dataType[2] = strdup("TK_RECORD");
    dataType[3] = strdup("TK_RUID");
    dataType[4] = strdup("TK_UNION");
    insert(dict, strdup("dataType"), dataType, 5);

    // primitiveDatatype
    char **primitiveDatatype = (char **)malloc(2 * sizeof(char *));
    primitiveDatatype[0] = strdup("TK_INT");
    primitiveDatatype[1] = strdup("TK_REAL");
    insert(dict, strdup("primitiveDatatype"), primitiveDatatype, 2);

    // constructedDatatype
    char **constructedDatatype = (char **)malloc(3 * sizeof(char *));
    constructedDatatype[0] = strdup("TK_RECORD");
    constructedDatatype[1] = strdup("TK_RUID");
    constructedDatatype[2] = strdup("TK_UNION");
    insert(dict, strdup("constructedDatatype"), constructedDatatype, 3);

    // remaining_list
    char **remaining_list = (char **)malloc(2 * sizeof(char *));
    remaining_list[0] = strdup("TK_COMMA");
    remaining_list[1] = strdup("ε");
    insert(dict, strdup("remaining_list"), remaining_list, 2);

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
    insert(dict, strdup("stmts"), stmts, 12);

    // typeDefinitions
    char **typeDefinitions = (char **)malloc(4 * sizeof(char *));
    typeDefinitions[0] = strdup("TK_RECORD");
    typeDefinitions[1] = strdup("TK_UNION");
    typeDefinitions[2] = strdup("TK_DEFINETYPE");
    typeDefinitions[3] = strdup("ε");
    insert(dict, strdup("typeDefinitions"), typeDefinitions, 4);

    // actualOrRedefined
    char **actualOrRedefined = (char **)malloc(3 * sizeof(char *));
    actualOrRedefined[0] = strdup("TK_RECORD");
    actualOrRedefined[1] = strdup("TK_UNION");
    actualOrRedefined[2] = strdup("TK_DEFINETYPE");
    insert(dict, strdup("actualOrRedefined"), actualOrRedefined, 3);

    // typeDefinition
    char **typeDefinition = (char **)malloc(2 * sizeof(char *));
    typeDefinition[0] = strdup("TK_RECORD");
    typeDefinition[1] = strdup("TK_UNION");
    insert(dict, strdup("typeDefinition"), typeDefinition, 2);

    // fieldDefinitions
    char **fieldDefinitions = (char **)malloc(1 * sizeof(char *));
    fieldDefinitions[0] = strdup("TK_TYPE");
    insert(dict, strdup("fieldDefinitions"), fieldDefinitions, 1);

    // fieldDefinition
    char **fieldDefinition = (char **)malloc(1 * sizeof(char *));
    fieldDefinition[0] = strdup("TK_TYPE");
    insert(dict, strdup("fieldDefinition"), fieldDefinition, 1);

    // fieldType
    char **fieldType = (char **)malloc(5 * sizeof(char *));
    fieldType[0] = strdup("TK_INT");
    fieldType[1] = strdup("TK_REAL");
    fieldType[2] = strdup("TK_RUID");
    fieldType[3] = strdup("TK_RECORD");
    fieldType[4] = strdup("TK_UNION");
    insert(dict, strdup("fieldType"), fieldType, 5);

    // moreFields
    char **moreFields = (char **)malloc(2 * sizeof(char *));
    moreFields[0] = strdup("TK_TYPE");
    moreFields[1] = strdup("ε");
    insert(dict, strdup("moreFields"), moreFields, 2);

    // declarations
    char **declarations = (char **)malloc(2 * sizeof(char *));
    declarations[0] = strdup("TK_TYPE");
    declarations[1] = strdup("ε");
    insert(dict, strdup("declarations"), declarations, 2);

    // declaration
    char **declaration = (char **)malloc(1 * sizeof(char *));
    declaration[0] = strdup("TK_TYPE");
    insert(dict, strdup("declaration"), declaration, 2);

    // global_or_not
    char **global_or_not = (char **)malloc(2 * sizeof(char *));
    global_or_not[0] = strdup("TK_COLON");
    global_or_not[1] = strdup("ε");
    insert(dict, strdup("global_or_not"), global_or_not, 2);

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
    insert(dict, strdup("otherStmts"), otherStmts, 8);

    // stmt
    char **stmt = (char **)malloc(7 * sizeof(char *));
    stmt[0] = strdup("TK_SQL");
    stmt[1] = strdup("TK_ID");
    stmt[2] = strdup("TK_CALL");
    stmt[3] = strdup("TK_WHILE");
    stmt[4] = strdup("TK_IF");
    stmt[5] = strdup("TK_READ");
    stmt[6] = strdup("TK_WRITE");
    insert(dict, strdup("stmt"), stmt, 7);

    // assignmentStmt
    char **assignmentStmt = (char **)malloc(1 * sizeof(char *));
    assignmentStmt[0] = strdup("TK_ID");
    insert(dict, strdup("assignmentStmt"), assignmentStmt, 1);

    // SingleOrRecId
    char **SingleOrRecId = (char **)malloc(1 * sizeof(char *));
    SingleOrRecId[0] = strdup("TK_ID");
    insert(dict, strdup("SingleOrRecId"), SingleOrRecId, 1);

    // option_single_constructed
    char **option_single_constructed = (char **)malloc(2 * sizeof(char *));
    option_single_constructed[0] = strdup("TK_DOT");
    option_single_constructed[1] = strdup("ε");
    insert(dict, strdup("option_single_constructed"), option_single_constructed, 2);

    // oneExpansion
    char **oneExpansion = (char **)malloc(1 * sizeof(char *));
    oneExpansion[0] = strdup("TK_DOT");
    insert(dict, strdup("oneExpansion"), oneExpansion, 1);

    // moreExpansions
    char **moreExpansions = (char **)malloc(2 * sizeof(char *));
    moreExpansions[0] = strdup("TK_DOT");
    moreExpansions[1] = strdup("ε");
    insert(dict, strdup("moreExpansions"), moreExpansions, 2);

    // funCallStmt
    char **funCallStmt = (char **)malloc(2 * sizeof(char *));
    funCallStmt[0] = strdup("TK_SQL");
    funCallStmt[1] = strdup("TK_CALL");
    insert(dict, strdup("funCallStmt"), funCallStmt, 2);

    // outputParameters
    char **outputParameters = (char **)malloc(2 * sizeof(char *));
    outputParameters[0] = strdup("TK_SQL");
    outputParameters[1] = strdup("ε");
    insert(dict, strdup("outputParameters"), outputParameters, 2);

    // inputParameters
    char **inputParameters = (char **)malloc(1 * sizeof(char *));
    inputParameters[0] = strdup("TK_SQL");
    insert(dict, strdup("inputParameters"), inputParameters, 1);

    // iterativeStmt
    char **iterativeStmt = (char **)malloc(1 * sizeof(char *));
    iterativeStmt[0] = strdup("TK_WHILE");
    insert(dict, strdup("iterativeStmt"), iterativeStmt, 1);

    // conditionalStmt
    char **conditionalStmt = (char **)malloc(1 * sizeof(char *));
    conditionalStmt[0] = strdup("TK_IF");
    insert(dict, strdup("conditionalStmt"), conditionalStmt, 1);

    // elsePart
    char **elsePart = (char **)malloc(2 * sizeof(char *));
    elsePart[0] = strdup("TK_ELSE");
    elsePart[1] = strdup("TK_ENDIF");
    insert(dict, strdup("elsePart"), elsePart, 2);

    // ioStmt
    char **ioStmt = (char **)malloc(2 * sizeof(char *));
    ioStmt[0] = strdup("TK_READ");
    ioStmt[1] = strdup("TK_WRITE");
    insert(dict, strdup("ioStmt"), ioStmt, 2);

    // arithmeticExpression
    char **arithmeticExpression = (char **)malloc(4 * sizeof(char *));
    arithmeticExpression[0] = strdup("TK_ID");
    arithmeticExpression[1] = strdup("TK_OP");
    arithmeticExpression[2] = strdup("TK_NUM");
    arithmeticExpression[3] = strdup("TK_RNUM");
    insert(dict, strdup("arithmeticExpression"), arithmeticExpression, 4);

    // epPrime
    char **epPrime = (char **)malloc(3 * sizeof(char *));
    epPrime[0] = strdup("TK_PLUS");
    epPrime[1] = strdup("TK_MINUS");
    epPrime[2] = strdup("ε");
    insert(dict, strdup("epPrime"), epPrime, 3);

    // term
    char **term = (char **)malloc(4 * sizeof(char *));
    term[0] = strdup("TK_ID");
    term[1] = strdup("TK_OP");
    term[2] = strdup("TK_NUM");
    term[3] = strdup("TK_RNUM");
    insert(dict, strdup("term"), term, 4);

    // termPrime
    char **termPrime = (char **)malloc(3 * sizeof(char *));
    termPrime[0] = strdup("TK_MUL");
    termPrime[1] = strdup("TK_DIV");
    termPrime[2] = strdup("ε");
    insert(dict, strdup("termPrime"), termPrime, 3);

    // factor
    char **factor = (char **)malloc(4 * sizeof(char *));
    factor[0] = strdup("TK_ID");
    factor[1] = strdup("TK_OP");
    factor[2] = strdup("TK_NUM");
    factor[3] = strdup("TK_RNUM");
    insert(dict, strdup("factor"), factor, 4);

    // highPrecedenceOperators
    char **highPrecedenceOperators = (char **)malloc(2 * sizeof(char *));
    highPrecedenceOperators[0] = strdup("TK_MUL");
    highPrecedenceOperators[1] = strdup("TK_DIV");
    insert(dict, strdup("highPrecedenceOperators"), highPrecedenceOperators, 2);

    // lowPrecedenceOperators
    char **lowPrecedenceOperators = (char **)malloc(2 * sizeof(char *));
    lowPrecedenceOperators[0] = strdup("TK_PLUS");
    lowPrecedenceOperators[1] = strdup("TK_MINUS");
    insert(dict, strdup("lowPrecedenceOperators"), lowPrecedenceOperators, 2);

    // booleanExpression
    char **booleanExpression = (char **)malloc(5 * sizeof(char *));
    booleanExpression[0] = strdup("TK_ID");
    booleanExpression[1] = strdup("TK_OP");
    booleanExpression[2] = strdup("TK_NOT");
    booleanExpression[3] = strdup("TK_NUM");
    booleanExpression[4] = strdup("TK_RNUM");
    insert(dict, strdup("booleanExpression"), booleanExpression, 5);

    // var
    char **var = (char **)malloc(3 * sizeof(char *));
    var[0] = strdup("TK_ID");
    var[1] = strdup("TK_NUM");
    var[2] = strdup("TK_RNUM");
    insert(dict, strdup("var"), var, 3);

    // logicalOp
    char **logicalOp = (char **)malloc(2 * sizeof(char *));
    logicalOp[0] = strdup("TK_AND");
    logicalOp[1] = strdup("TK_OR");
    insert(dict, strdup("logicalOp"), logicalOp, 2);

    // relationalOp
    char **relationalOp = (char **)malloc(6 * sizeof(char *));
    relationalOp[0] = strdup("TK_LT");
    relationalOp[1] = strdup("TK_LE");
    relationalOp[2] = strdup("TK_EQ");
    relationalOp[3] = strdup("TK_GT");
    relationalOp[4] = strdup("TK_GE");
    relationalOp[5] = strdup("TK_NE");
    insert(dict, strdup("relationalOp"), relationalOp, 6);

    // returnStmt
    char **returnStmt = (char **)malloc(1 * sizeof(char *));
    returnStmt[0] = strdup("TK_RETURN");
    insert(dict, strdup("returnStmt"), returnStmt, 1);

    // optionalReturn
    char **optionalReturn = (char **)malloc(2 * sizeof(char *));
    optionalReturn[0] = strdup("TK_SQL");
    optionalReturn[1] = strdup("ε");
    insert(dict, strdup("optionalReturn"), optionalReturn, 2);

    // idList
    char **idList = (char **)malloc(1 * sizeof(char *));
    idList[0] = strdup("TK_ID");
    insert(dict, strdup("idList"), idList, 1);

    // more_ids
    char **more_ids = (char **)malloc(2 * sizeof(char *));
    more_ids[0] = strdup("TK_COMMA");
    more_ids[1] = strdup("ε");
    insert(dict, strdup("more_ids"), more_ids, 2);

    // definetypestmt
    char **definetypestmt = (char **)malloc(1 * sizeof(char *));
    definetypestmt[0] = strdup("TK_DEFINETYPE");
    insert(dict, strdup("definetypestmt"), definetypestmt, 1);

    // A
    char **A = (char **)malloc(2 * sizeof(char *));
    A[0] = strdup("TK_RECORD");
    A[1] = strdup("TK_UNION");
    insert(dict, strdup("A"), A, 2);

    // Free dictionary and exit
    // (Remember to free memory allocated for keys and values)
}

void initFollow(struct Dictionary *dict)
{
    // program
    char **program = (char **)malloc(1 * sizeof(char *));
    program[0] = strdup("ε");
    insert(dict, strdup("program"), program, 1);

    // mainFunction
    char **mainFunction = (char **)malloc(1 * sizeof(char *));
    mainFunction[0] = strdup("ε");
    insert(dict, strdup("mainFunction"), mainFunction, 1);

    // otherFunctions
    char **otherFunctions = (char **)malloc(1 * sizeof(char *));
    otherFunctions[0] = strdup("TK_MAIN");
    insert(dict, strdup("otherFunctions"), otherFunctions, 1);

    // function
    char **function = (char **)malloc(2 * sizeof(char *));
    function[0] = strdup("TK_MAIN");
    function[1] = strdup("TK_FUNID");
    insert(dict, strdup("function"), function, 2);

    // input_par
    char **input_par = (char **)malloc(2 * sizeof(char *));
    input_par[0] = strdup("TK_SEM");
    input_par[1] = strdup("TK_OUTPUT");
    insert(dict, strdup("input_par"), input_par, 2);

    // output_par
    char **output_par = (char **)malloc(1 * sizeof(char *));
    output_par[0] = strdup("TK_SEM");
    insert(dict, strdup("output_par"), output_par, 1);

    // parameter_list
    char **parameter_list = (char **)malloc(1 * sizeof(char *));
    parameter_list[0] = strdup("TK_SQR");
    insert(dict, strdup("parameter_list"), parameter_list, 1);

    // dataType
    char **dataType = (char **)malloc(2 * sizeof(char *));
    dataType[0] = strdup("TK_ID");
    dataType[1] = strdup("TK_COLON");
    insert(dict, strdup("dataType"), dataType, 2);

    // primitiveDatatype
    char **primitiveDatatype = (char **)malloc(2 * sizeof(char *));
    primitiveDatatype[0] = strdup("TK_ID");
    primitiveDatatype[1] = strdup("TK_COLON");
    insert(dict, strdup("primitiveDatatype"), primitiveDatatype, 2);

    // constructedDatatype
    char **constructedDatatype = (char **)malloc(2 * sizeof(char *));
    constructedDatatype[0] = strdup("TK_ID");
    constructedDatatype[1] = strdup("TK_COLON");
    insert(dict, strdup("constructedDatatype"), constructedDatatype, 2);

    // remaining_list
    char **remaining_list = (char **)malloc(1 * sizeof(char *));
    remaining_list[0] = strdup("TK_SQR");
    insert(dict, strdup("remaining_list"), remaining_list, 1);

    // stmts
    char **stmts = (char **)malloc(1 * sizeof(char *));
    stmts[0] = strdup("TK_END");
    insert(dict, strdup("stmts"), stmts, 1);

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
    insert(dict, strdup("typeDefinitions"), typeDefinitions, 12);

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
    insert(dict, strdup("actualOrRedefined"), actualOrRedefined, 13);

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
    insert(dict, strdup("typeDefinition"), typeDefinition, 12);

    // fieldDefinitions
    char **fieldDefinitions = (char **)malloc(3 * sizeof(char *));
    fieldDefinitions[0] = strdup("TK_ENDRECORD");
    fieldDefinitions[1] = strdup("TK_ENDUNION");
    fieldDefinitions[2] = strdup("TK_TYPE");
    insert(dict, strdup("fieldDefinitions"), fieldDefinitions, 3);

    // fieldDefinition
    char **fieldDefinition = (char **)malloc(3 * sizeof(char *));
    fieldDefinition[0] = strdup("TK_ENDRECORD");
    fieldDefinition[1] = strdup("TK_ENDUNION");
    fieldDefinition[2] = strdup("TK_TYPE");
    insert(dict, strdup("fieldDefinition"), fieldDefinition, 3);

    // fieldType
    char **fieldType = (char **)malloc(1 * sizeof(char *));
    fieldType[0] = strdup("TK_COLON");
    insert(dict, strdup("fieldType"), fieldType, 1);

    // moreFields
    char **moreFields = (char **)malloc(2 * sizeof(char *));
    moreFields[0] = strdup("TK_ENDRECORD");
    moreFields[1] = strdup("TK_ENDUNION");
    insert(dict, strdup("moreFields"), moreFields, 2);

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
    insert(dict, strdup("declarations"), declarations, 8);

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
    insert(dict, strdup("declaration"), declaration, 9);

    // global_or_not
    char **global_or_not = (char **)malloc(1 * sizeof(char *));
    global_or_not[0] = strdup("TK_SEM");
    insert(dict, strdup("global_or_not"), global_or_not, 1);

    // otherStmts
    char **otherStmts = (char **)malloc(4 * sizeof(char *));
    otherStmts[0] = strdup("TK_ENDWHILE");
    otherStmts[1] = strdup("TK_ELSE");
    otherStmts[2] = strdup("TK_ENDIF");
    otherStmts[3] = strdup("TK_RETURN");
    insert(dict, strdup("otherStmts"), otherStmts, 4);

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
    insert(dict, strdup("stmt"), stmt, 12);

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
    insert(dict, strdup("assignmentStmt"), assignmentStmt, 12);

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
    insert(dict, strdup("SingleOrRecId"), SingleOrRecId, 15);

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
    insert(dict, strdup("option_single_constructed"), option_single_constructed, 15);

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
    insert(dict, strdup("oneExpansion"), oneExpansion, 15);

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
    insert(dict, strdup("moreExpansions"), moreExpansions, 15);

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
    insert(dict, strdup("funCallStmt"), funCallStmt, 12);

    // outputParameters
    char **outputParameters = (char **)malloc(1 * sizeof(char *));
    outputParameters[0] = strdup("TK_CALL");
    insert(dict, strdup("outputParameters"), outputParameters, 1);

    // inputParameters
    char **inputParameters = (char **)malloc(1 * sizeof(char *));
    inputParameters[0] = strdup("TK_SEM");
    insert(dict, strdup("inputParameters"), inputParameters, 1);

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
    insert(dict, strdup("iterativeStmt"), iterativeStmt, 12);

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
    insert(dict, strdup("conditionalStmt"), conditionalStmt, 12);

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
    insert(dict, strdup("elsePart"), elsePart, 12);

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
    insert(dict, strdup("ioStmt"), ioStmt, 12);

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
    insert(dict, strdup("arithmeticExpression"), arithmeticExpression, 8);

    // expPrime
    char **expPrime = (char **)malloc(6 * sizeof(char *));
    expPrime[0] = strdup("TK_SEM");
    expPrime[1] = strdup("TK_CL");
    expPrime[2] = strdup("TK_PLUS");
    expPrime[3] = strdup("TK_MINUS");
    expPrime[4] = strdup("TK_ID");
    expPrime[5] = strdup("TK_NUM");
    insert(dict, strdup("expPrime"), expPrime, 6);

    // term
    char **term = (char **)malloc(4 * sizeof(char *));
    term[0] = strdup("TK_SEM");
    term[1] = strdup("TK_CL");
    term[2] = strdup("TK_PLUS");
    term[3] = strdup("TK_MINUS");
    insert(dict, strdup("term"), term, 4);

    // termPrime
    char **termPrime = (char **)malloc(4 * sizeof(char *));
    termPrime[0] = strdup("TK_SEM");
    termPrime[1] = strdup("TK_CL");
    termPrime[2] = strdup("TK_PLUS");
    termPrime[3] = strdup("TK_MINUS");
    insert(dict, strdup("termPrime"), termPrime, 4);

    // factor
    char **factor = (char **)malloc(6 * sizeof(char *));
    factor[0] = strdup("TK_SEM");
    factor[1] = strdup("TK_CL");
    factor[2] = strdup("TK_MUL");
    factor[3] = strdup("TK_DIV");
    factor[4] = strdup("TK_PLUS");
    factor[5] = strdup("TK_MINUS");
    insert(dict, strdup("factor"), factor, 6);

    // highPrecedenceOperators
    char **highPrecedenceOperators = (char **)malloc(3 * sizeof(char *));
    highPrecedenceOperators[0] = strdup("TK_ID");
    highPrecedenceOperators[1] = strdup("TK_OP");
    highPrecedenceOperators[2] = strdup("TK_NUM");
    insert(dict, strdup("highPrecedenceOperators"), highPrecedenceOperators, 3);

    // lowPrecedenceOperators
    char **lowPrecedenceOperators = (char **)malloc(3 * sizeof(char *));
    lowPrecedenceOperators[0] = strdup("TK_ID");
    lowPrecedenceOperators[1] = strdup("TK_OP");
    lowPrecedenceOperators[2] = strdup("TK_NUM");
    insert(dict, strdup("lowPrecedenceOperators"), lowPrecedenceOperators, 3);

    // booleanExpression
    char **booleanExpression = (char **)malloc(1 * sizeof(char *));
    booleanExpression[0] = strdup("TK_CL");
    insert(dict, strdup("booleanExpression"), booleanExpression, 1);

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
    insert(dict, strdup("var"), var, 12);

    // logicalOp
    char **logicalOp = (char **)malloc(4 * sizeof(char *));
    logicalOp[0] = strdup("TK_AND");
    logicalOp[1] = strdup("TK_OR");
    logicalOp[2] = strdup("TK_NOT");
    logicalOp[3] = strdup("TK_OP");
    insert(dict, strdup("logicalOp"), logicalOp, 4);

    // relationalOp
    char **relationalOp = (char **)malloc(4 * sizeof(char *));
    relationalOp[0] = strdup("TK_ID");
    relationalOp[1] = strdup("TK_NUM");
    relationalOp[2] = strdup("TK_RNUM");
    relationalOp[3] = strdup("TK_OP");
    insert(dict, strdup("relationalOp"), relationalOp, 4);

    // returnStmt
    char **returnStmt = (char **)malloc(1 * sizeof(char *));
    returnStmt[0] = strdup("TK_END");
    insert(dict, strdup("returnStmt"), returnStmt, 1);

    // optionalReturn
    char **optionalReturn = (char **)malloc(1 * sizeof(char *));
    optionalReturn[0] = strdup("TK_SEM");
    insert(dict, strdup("optionalReturn"), optionalReturn, 1);

    // idList
    char **idList = (char **)malloc(1 * sizeof(char *));
    idList[0] = strdup("TK_SQR");
    insert(dict, strdup("idList"), idList, 1);

    // more_ids
    char **more_ids = (char **)malloc(1 * sizeof(char *));
    more_ids[0] = strdup("TK_SQR");
    insert(dict, strdup("more_ids"), more_ids, 1);

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
    insert(dict, strdup("definetypestmt"), definetypestmt, 12);

    // A
    char **A = (char **)malloc(1 * sizeof(char *));
    A[0] = strdup("TK_RUID");
    insert(dict, strdup("A"), A, 1);
}

int searchF(struct Dictionary *dict, char *NT, char *T)
{
    int index = hash(NT);
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

int main()
{
    // Example usage
    struct Dictionary *dict = createDictionary();
    initFirst(dict);
    // int j = searchF(dict,"more_ids","TK_MAIN");
    // printf("%d\n",j);
    int j = 1;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (dict->table[i] != NULL)
        {
            printf("%d %d\n", j++, i);
        }
    }

    /*
    int len;
    char **result = search(dict, "relationalOp",&len);
    if (result != NULL) {
        printf("Number of elements in 'result': %d\n", len);
        printf("Value found for : ");
        for (int i = 0; i < len; ++i) {
            printf("%s ", result[i]);
        }
        printf("\n");
    } else {
        printf("Key not found\n");
    }
    // Free dynamically allocated memory for keys and values

    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (dict->table[i] != NULL) {
            free(dict->table[i]->key);
            for (int j = 0; dict->table[i]->value[j] != NULL; ++j) {
                free(dict->table[i]->value[j]);
            }
            free(dict->table[i]->value);
            free(dict->table[i]);
        }
    }
    free(dict);
*/
    return 0;
}

/*



struct node {
    char name[20]; // $ for epsilon
    bool terminal;
    struct node* next;
};

struct node** list[375];

int main(){
    int li[][3] = {{1,2,3,4}};
    printf("%d",li[0][0]);
    return 0;
}

*/
