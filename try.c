#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TABLE_SIZE 1500
#define MAX_RULE_LENGTH 5 // Maximum length of a production rule

// Node structure for a term in a production rule
typedef struct node {
    char name[50];
    bool terminal;
    struct node* next;
} NODE;

// Rules structure containing heads of production rules
typedef struct Rules {
    NODE* heads;
    int length;
} RULES;

// Grammar structure
typedef struct Grammar {
    RULES* rules[TABLE_SIZE];
} GRAMMAR;

// Hash function
int hash(char *key) {
    int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue += (key[i] * i);
    }
    return hashValue % TABLE_SIZE;
}

void initRules(RULES* rules){
    rules->heads = NULL;
    rules->length = 0;
}

// Function to add a rule to the grammar
void addRule(GRAMMAR* grammar, char* nonTerminal, NODE* nodes) {
    int index = hash(nonTerminal);
    if (grammar->rules[index] == NULL) {
        RULES* newRules = (RULES*)malloc(sizeof(RULES));
        initRules(newRules);
        newRules->length++;
        newRules->heads = (NODE*) malloc(5*sizeof(NODE));
        newRules->heads[0] = nodes[0];
        grammar->rules[index] = newRules;
    }
    else {
        grammar->rules[index]->heads[grammar->rules[index]->length]=nodes[0];
        grammar->rules[index]->length++;
        //printf("Non-terminal %s already has a rule defined.\n", nonTerminal);
    }
}

// Function to retrieve rules for a non-terminal
RULES* getRules(GRAMMAR* grammar, char* nonTerminal) {
    int index = hash(nonTerminal);
    return grammar->rules[index];
}

void printGrammar(GRAMMAR* grammar){
    int count=1;
    for(int i=0;i<TABLE_SIZE;i++){
        if (grammar->rules[i] != NULL){
            printf("\n");
            printf("Non terminal number: %d\n", count++);
            RULES* r = grammar->rules[i];
            for (int i = 0; i < r->length; i++) {
            printf("Rule %d:\n", i + 1);
            printf("Name: %s, Terminal: %s\n",r->heads[i].name, r->heads[i].terminal ? "true" : "false");
            NODE* head = r->heads[i].next;
            while (head != NULL) {
                printf("Name: %s, Terminal: %s\n", head->name, head->terminal ? "true" : "false");
                head = head->next;
            }
        }
        }
    }
}

int main() {
    // Initialize grammar
    GRAMMAR grammar;
    for (int i = 0; i < TABLE_SIZE; i++) {
        grammar.rules[i] = NULL;
    }

    // // Example: Adding rules to the grammar
    // NODE nodes1[] = {
    //     {"B1", false, &nodes1[1]},
    //     {"+", true, &nodes1[2]},
    //     {"C1", false, NULL}
    // };
    // NODE nodes2[] = {
    //     {"X1", false, &nodes2[1]},
    //     {"+", true, &nodes2[2]},
    //     {"Y1", false, NULL}
    // };
    // addRule(&grammar, "A1", nodes1);
    // addRule(&grammar, "A1", nodes2);

    // <program> ===> <otherFunctions> <mainFunction>
    NODE nodes_program[] = {
        {"otherFunctions", false, &nodes_program[1]},
        {"mainFunction", false, NULL}
    };
    addRule(&grammar, "program", nodes_program);

    // <mainFunction> ===> TK_MAIN <stmts> TK_END
    NODE nodes_mainFunction[] = {
        {"TK_MAIN", true, &nodes_mainFunction[1]},
        {"stmts", false, &nodes_mainFunction[2]},
        {"TK_END", true, NULL}
    };
    addRule(&grammar, "mainFunction", nodes_mainFunction);

    // <otherFunctions> ===> <function><otherFunctions> | ε
    NODE nodes_otherFunctions_1[] = {
        {"function", false, &nodes_otherFunctions_1[1]},
        {"otherFunctions", false, NULL}
    };
    NODE nodes_otherFunctions_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "otherFunctions", nodes_otherFunctions_1);
    addRule(&grammar, "otherFunctions", nodes_otherFunctions_2);

    // <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    NODE nodes_function[] = {
        {"TK_FUNID", true, &nodes_function[1]},
        {"input_par", false, &nodes_function[2]},
        {"output_par", false, &nodes_function[3]},
        {"TK_SEM", true, &nodes_function[4]},
        {"stmts", false, &nodes_function[5]},
        {"TK_END", true, NULL}
    };
    addRule(&grammar, "function", nodes_function);

    // <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    NODE nodes_input_par[] = {
        {"TK_INPUT", true, &nodes_input_par[1]},
        {"TK_PARAMETER", true, &nodes_input_par[2]},
        {"TK_LIST", true, &nodes_input_par[3]},
        {"TK_SQL", true, &nodes_input_par[4]},
        {"parameter_list", false, &nodes_input_par[5]},
        {"TK_SQR", true, NULL}
    };
    addRule(&grammar, "input_par", nodes_input_par);

    // <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | ε
    NODE nodes_output_par_1[] = {
        {"TK_OUTPUT", true, &nodes_output_par_1[1]},
        {"TK_PARAMETER", true, &nodes_output_par_1[2]},
        {"TK_LIST", true, &nodes_output_par_1[3]},
        {"TK_SQL", true, &nodes_output_par_1[4]},
        {"parameter_list", false, &nodes_output_par_1[5]},
        {"TK_SQR", true, NULL}
    };
    NODE nodes_output_par_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "output_par", nodes_output_par_1);
    addRule(&grammar, "output_par", nodes_output_par_2);

    // <parameter_list>===><dataType> TK_ID <remaining_list>
    NODE nodes_parameter_list[] = {
        {"dataType", false, &nodes_parameter_list[1]},
        {"TK_ID", true, &nodes_parameter_list[2]},
        {"remaining_list", false, NULL}
    };
    addRule(&grammar, "parameter_list", nodes_parameter_list);

    // <dataType>===> <primitiveDatatype> |<constructedDatatype>
    NODE nodes_dataType_1[] = {
        {"primitiveDatatype", false, NULL}
    };
    NODE nodes_dataType_2[] = {
        {"constructedDatatype", false, NULL}
    };
    addRule(&grammar, "dataType", nodes_dataType_1);
    addRule(&grammar, "dataType", nodes_dataType_2);

    // <primitiveDatatype>===> TK_INT | TK_REAL
    NODE nodes_primitiveDatatype_1[] = {
        {"TK_INT", true, NULL}
    };
    NODE nodes_primitiveDatatype_2[] = {
        {"TK_REAL", true, NULL}
    };
    addRule(&grammar, "primitiveDatatype", nodes_primitiveDatatype_1);
    addRule(&grammar, "primitiveDatatype", nodes_primitiveDatatype_2);

    // <constructedDatatype>===> TK_RECORD TK_RUID | TK_UNION TK_RUID | TK_RUID
    NODE nodes_constructedDatatype_1[] = {
        {"TK_RECORD", true, &nodes_constructedDatatype_1[1]},
        {"TK_RUID", true, NULL}
    };
    NODE nodes_constructedDatatype_2[] = {
        {"TK_UNION", true, &nodes_constructedDatatype_2[1]},
        {"TK_RUID", true, NULL}
    };
    NODE nodes_constructedDatatype_3[] = {
        {"TK_RUID", true, NULL}
    };
    addRule(&grammar, "constructedDatatype", nodes_constructedDatatype_1);
    addRule(&grammar, "constructedDatatype", nodes_constructedDatatype_2);
    addRule(&grammar, "constructedDatatype", nodes_constructedDatatype_3);

    // <remaining_list>===>TK_COMMA <parameter_list> | ε
    NODE nodes_remaining_list_1[] = {
        {"TK_COMMA", true, &nodes_remaining_list_1[1]},
        {"parameter_list", false, NULL}
    };
    NODE nodes_remaining_list_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "remaining_list", nodes_remaining_list_1);
    addRule(&grammar, "remaining_list", nodes_remaining_list_2);

    // <stmts>===><typeDefinitions> <declarations> <otherStmts><returnStmt>
    NODE nodes_stmts[] = {
        {"typeDefinitions", false, &nodes_stmts[1]},
        {"declarations", false, &nodes_stmts[2]},
        {"otherStmts", false, &nodes_stmts[3]},
        {"returnStmt", false, NULL}
    };
    addRule(&grammar, "stmts", nodes_stmts);

    // <typeDefinitions>===> <actualOrRedefined> <typeDefinitions> | ε
    NODE nodes_typeDefinitions_1[] = {
        {"actualOrRedefined", false, &nodes_typeDefinitions_1[1]},
        {"typeDefinitions", false, NULL}
    };
    NODE nodes_typeDefinitions_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "typeDefinitions", nodes_typeDefinitions_1);
    addRule(&grammar, "typeDefinitions", nodes_typeDefinitions_2);

    // <actualOrRedefined> ===> <typeDefinition> | <definetypestmt>
    NODE nodes_actualOrRedefined_1[] = {
        {"typeDefinition", false, NULL}
    };
    NODE nodes_actualOrRedefined_2[] = {
        {"definetypestmt", false, NULL}
    };
    addRule(&grammar, "actualOrRedefined", nodes_actualOrRedefined_1);
    addRule(&grammar, "actualOrRedefined", nodes_actualOrRedefined_2);

    // <typeDefinition>===>TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    NODE nodes_typeDefinition_1[] = {
        {"TK_RECORD", true, &nodes_typeDefinition_1[1]},
        {"TK_RUID", true, &nodes_typeDefinition_1[2]},
        {"fieldDefinitions", false, &nodes_typeDefinition_1[3]},
        {"TK_ENDRECORD", true, NULL}
    };
    NODE nodes_typeDefinition_2[] = {
        {"TK_UNION", true, &nodes_typeDefinition_2[1]},
        {"TK_RUID", true, &nodes_typeDefinition_2[2]},
        {"fieldDefinitions", false, &nodes_typeDefinition_2[3]},
        {"TK_ENDUNION", true, NULL}
    };
    addRule(&grammar, "typeDefinition", nodes_typeDefinition_1);
    addRule(&grammar, "typeDefinition", nodes_typeDefinition_2);

    // <fieldDefinitions>===> <fieldDefinition><fieldDefinition><moreFields>
    NODE nodes_fieldDefinitions[] = {
        {"fieldDefinition", false, &nodes_fieldDefinitions[1]},
        {"fieldDefinition", false, &nodes_fieldDefinitions[2]},
        {"moreFields", false, NULL}
    };
    addRule(&grammar, "fieldDefinitions", nodes_fieldDefinitions);

    // <fieldDefinition>===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    NODE nodes_fieldDefinition[] = {
        {"TK_TYPE", true, &nodes_fieldDefinition[1]},
        {"fieldType", false, &nodes_fieldDefinition[2]},
        {"TK_COLON", true, &nodes_fieldDefinition[3]},
        {"TK_FIELDID", true, &nodes_fieldDefinition[4]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "fieldDefinition", nodes_fieldDefinition);

    // <fieldType>===> <primitiveDatatype> | <constructedDatatype>
    NODE nodes_fieldType_1[] = {
        {"primitiveDatatype", false, NULL}
    };
    NODE nodes_fieldType_2[] = {
        {"constructedDatatype", false, NULL}
    };
    addRule(&grammar, "fieldType", nodes_fieldType_1);
    addRule(&grammar, "fieldType", nodes_fieldType_2);

    // <moreFields>===><fieldDefinition><moreFields> | ε
    NODE nodes_moreFields_1[] = {
        {"fieldDefinition", false, &nodes_moreFields_1[1]},
        {"moreFields", false, NULL}
    };
    NODE nodes_moreFields_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "moreFields", nodes_moreFields_1);
    addRule(&grammar, "moreFields", nodes_moreFields_2);

    // <declarations> ===> <declaration><declarations>| ε
    NODE nodes_declarations_1[] = {
        {"declaration", false, &nodes_declarations_1[1]},
        {"declarations", false, NULL}
    };
    NODE nodes_declarations_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "declarations", nodes_declarations_1);
    addRule(&grammar, "declarations", nodes_declarations_2);

    // <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    NODE nodes_declaration[] = {
        {"TK_TYPE", true, &nodes_declaration[1]},
        {"dataType", false, &nodes_declaration[2]},
        {"TK_COLON", true, &nodes_declaration[3]},
        {"TK_ID", true, &nodes_declaration[4]},
        {"global_or_not", false, &nodes_declaration[5]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "declaration", nodes_declaration);

    // <global_or_not>===>TK_COLON TK_GLOBAL| ε
    NODE nodes_global_or_not_1[] = {
        {"TK_COLON", true, &nodes_global_or_not_1[1]},
        {"TK_GLOBAL", true, NULL}
    };
    NODE nodes_global_or_not_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "global_or_not", nodes_global_or_not_1);
    addRule(&grammar, "global_or_not", nodes_global_or_not_2);

    // <otherStmts>===> <stmt><otherStmts> | ε
    NODE nodes_otherStmts_1[] = {
        {"stmt", false, &nodes_otherStmts_1[1]},
        {"otherStmts", false, NULL}
    };
    NODE nodes_otherStmts_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "otherStmts", nodes_otherStmts_1);
    addRule(&grammar, "otherStmts", nodes_otherStmts_2);

    // <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
    NODE nodes_stmt_1[] = {
        {"assignmentStmt", false, NULL}
    };
    NODE nodes_stmt_2[] = {
        {"iterativeStmt", false, NULL}
    };
    NODE nodes_stmt_3[] = {
        {"conditionalStmt", false, NULL}
    };
    NODE nodes_stmt_4[] = {
        {"ioStmt", false, NULL}
    };
    NODE nodes_stmt_5[] = {
        {"funCallStmt", false, NULL}
    };
    addRule(&grammar, "stmt", nodes_stmt_1);
    addRule(&grammar, "stmt", nodes_stmt_2);
    addRule(&grammar, "stmt", nodes_stmt_3);
    addRule(&grammar, "stmt", nodes_stmt_4);
    addRule(&grammar, "stmt", nodes_stmt_5);

     // <assignmentStmt>===><SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    NODE nodes_assignmentStmt[] = {
        {"SingleOrRecId", false, &nodes_assignmentStmt[1]},
        {"TK_ASSIGNOP", true, &nodes_assignmentStmt[2]},
        {"arithmeticExpression", false, &nodes_assignmentStmt[3]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "assignmentStmt", nodes_assignmentStmt);

    // <SingleOrRecId>===>TK_ID <option_single_constructed>
    NODE nodes_singleOrRecId[] = {
        {"TK_ID", true, &nodes_singleOrRecId[1]},
        {"option_single_constructed", false, NULL}
    };
    addRule(&grammar, "SingleOrRecId", nodes_singleOrRecId);

    // <option_single_constructed>===> ε | <oneExpansion><moreExpansions>
    NODE nodes_optionSingleConstructed_1[] = {
        {"ε", false, NULL}
    };
    NODE nodes_optionSingleConstructed_2[] = {
        {"oneExpansion", false, &nodes_optionSingleConstructed_2[1]},
        {"moreExpansions", false, NULL}
    };
    addRule(&grammar, "option_single_constructed", nodes_optionSingleConstructed_1);
    addRule(&grammar, "option_single_constructed", nodes_optionSingleConstructed_2);

    // <oneExpansion>===> TK_DOT TK_FIELDID 
    NODE nodes_oneExpansion[] = {
        {"TK_DOT", true, &nodes_oneExpansion[1]},
        {"TK_FIELDID", true, NULL}
    };
    addRule(&grammar, "oneExpansion", nodes_oneExpansion);

    // <moreExpansions>===> <oneExpansion> <moreExpansions> | ε
    NODE nodes_moreExpansions_1[] = {
        {"oneExpansion", false, &nodes_moreExpansions_1[1]},
        {"moreExpansions", false, NULL}
    };
    NODE nodes_moreExpansions_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "moreExpansions", nodes_moreExpansions_1);
    addRule(&grammar, "moreExpansions", nodes_moreExpansions_2);

    // <funCallStmt>===><outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    NODE nodes_funCallStmt[] = {
        {"outputParameters", false, &nodes_funCallStmt[1]},
        {"TK_CALL", true, &nodes_funCallStmt[2]},
        {"TK_FUNID", true, &nodes_funCallStmt[3]},
        {"TK_WITH", true, &nodes_funCallStmt[4]},
        {"TK_PARAMETERS", true, &nodes_funCallStmt[5]},
        {"inputParameters", false, &nodes_funCallStmt[6]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "funCallStmt", nodes_funCallStmt);

    // <outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP | ε
    NODE nodes_outputParameters_1[] = {
        {"TK_SQL", true, &nodes_outputParameters_1[1]},
        {"idList", false, &nodes_outputParameters_1[2]},
        {"TK_SQR", true, &nodes_outputParameters_1[3]},
        {"TK_ASSIGNOP", true, NULL}
    };
    NODE nodes_outputParameters_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "outputParameters", nodes_outputParameters_1);
    addRule(&grammar, "outputParameters", nodes_outputParameters_2);

    // <inputParameters>===> TK_SQL <idList> TK_SQR
    NODE nodes_inputParameters[] = {
        {"TK_SQL", true, &nodes_inputParameters[1]},
        {"idList", false, &nodes_inputParameters[2]},
        {"TK_SQR", true, NULL}
    };
    addRule(&grammar, "inputParameters", nodes_inputParameters);

    // <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt><otherStmts> TK_ENDWHILE
    NODE nodes_iterativeStmt[] = {
        {"TK_WHILE", true, &nodes_iterativeStmt[1]},
        {"TK_OP", true, &nodes_iterativeStmt[2]},
        {"booleanExpression", false, &nodes_iterativeStmt[3]},
        {"TK_CL", true, &nodes_iterativeStmt[4]},
        {"stmt", false, &nodes_iterativeStmt[5]},
        {"otherStmts", false, &nodes_iterativeStmt[6]},
        {"TK_ENDWHILE", true, NULL}
    };
    addRule(&grammar, "iterativeStmt", nodes_iterativeStmt);

    // <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt><otherStmts> <elsePart>
    NODE nodes_conditionalStmt[] = {
        {"TK_IF", true, &nodes_conditionalStmt[1]},
        {"TK_OP", true, &nodes_conditionalStmt[2]},
        {"booleanExpression", false, &nodes_conditionalStmt[3]},
        {"TK_CL", true, &nodes_conditionalStmt[4]},
        {"TK_THEN", true, &nodes_conditionalStmt[5]},
        {"stmt", false, &nodes_conditionalStmt[6]},
        {"otherStmts", false, &nodes_conditionalStmt[7]},
        {"elsePart", false, NULL}
    };
    addRule(&grammar, "conditionalStmt", nodes_conditionalStmt);

    // <elsePart>===>TK_ELSE <stmt><otherStmts> TK_ENDIF | TK_ENDIF
    NODE nodes_elsePart_1[] = {
        {"TK_ELSE", true, &nodes_elsePart_1[1]},
        {"stmt", false, &nodes_elsePart_1[2]},
        {"otherStmts", false, NULL},
        {"TK_ENDIF", true, NULL}
    };
    NODE nodes_elsePart_2[] = {
        {"TK_ENDIF", true, NULL}
    };
    addRule(&grammar, "elsePart", nodes_elsePart_1);
    addRule(&grammar, "elsePart", nodes_elsePart_2);

    // <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
    NODE nodes_ioStmt_1[] = {
        {"TK_READ", true, &nodes_ioStmt_1[1]},
        {"TK_OP", true, &nodes_ioStmt_1[2]},
        {"var", false, &nodes_ioStmt_1[3]},
        {"TK_CL", true, &nodes_ioStmt_1[4]},
        {"TK_SEM", true, NULL}
    };
    NODE nodes_ioStmt_2[] = {
        {"TK_WRITE", true, &nodes_ioStmt_2[1]},
        {"TK_OP", true, &nodes_ioStmt_2[2]},
        {"var", false, &nodes_ioStmt_2[3]},
        {"TK_CL", true, &nodes_ioStmt_2[4]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "ioStmt", nodes_ioStmt_1);
    addRule(&grammar, "ioStmt", nodes_ioStmt_2);

    // <arithmeticExpression> ===> <term> <expPrime>
    NODE nodes_arithmeticExpression[] = {
        {"term", false, &nodes_arithmeticExpression[1]},
        {"expPrime", false, NULL}
    };
    addRule(&grammar, "arithmeticExpression", nodes_arithmeticExpression);

    // <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime> | ε
    NODE nodes_expPrime_1[] = {
        {"lowPrecedenceOperators", false, &nodes_expPrime_1[1]},
        {"term", false, &nodes_expPrime_1[2]},
        {"expPrime", false, NULL}
    };
    NODE nodes_expPrime_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "expPrime", nodes_expPrime_1);
    addRule(&grammar, "expPrime", nodes_expPrime_2);

    // <term>===> <factor> <termPrime>
    NODE nodes_term[] = {
        {"factor", false, &nodes_term[1]},
        {"termPrime", false, NULL}
    };
    addRule(&grammar, "term", nodes_term);

    // <termPrime> ===> <highPrecedenceOperators><factor> <termPrime> | ε
    NODE nodes_termPrime_1[] = {
        {"highPrecedenceOperators", false, &nodes_termPrime_1[1]},
        {"factor", false, &nodes_termPrime_1[2]},
        {"termPrime", false, NULL}
    };
    NODE nodes_termPrime_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "termPrime", nodes_termPrime_1);
    addRule(&grammar, "termPrime", nodes_termPrime_2);

    // <factor> ===> TK_OP <arithmeticExpression> TK_CL | <var>
    NODE nodes_factor_1[] = {
        {"TK_OP", true, &nodes_factor_1[1]},
        {"arithmeticExpression", false, &nodes_factor_1[2]},
        {"TK_CL", true, NULL}
    };
    NODE nodes_factor_2[] = {
        {"var", false, NULL}
    };
    addRule(&grammar, "factor", nodes_factor_1);
    addRule(&grammar, "factor", nodes_factor_2);

    // <highPrecedenceOperators>===> TK_MUL | TK_DIV
    NODE nodes_highPrecedenceOperators_1[] = {
        {"TK_MUL", true, NULL}
    };
    NODE nodes_highPrecedenceOperators_2[] = {
        {"TK_DIV", true, NULL}
    };
    addRule(&grammar, "highPrecedenceOperators", nodes_highPrecedenceOperators_1);
    addRule(&grammar, "highPrecedenceOperators", nodes_highPrecedenceOperators_2);

    // <lowPrecedenceOperators> ===> TK_PLUS | TK_MINUS
    NODE nodes_lowPrecedenceOperators_1[] = {
        {"TK_PLUS", true, NULL}
    };
    NODE nodes_lowPrecedenceOperators_2[] = {
        {"TK_MINUS", true, NULL}
    };
    addRule(&grammar, "lowPrecedenceOperators", nodes_lowPrecedenceOperators_1);
    addRule(&grammar, "lowPrecedenceOperators", nodes_lowPrecedenceOperators_2);

    // <booleanExpression>===>TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL
    NODE nodes_booleanExpression_1[] = {
        {"TK_OP", true, &nodes_booleanExpression_1[1]},
        {"booleanExpression", false, &nodes_booleanExpression_1[2]},
        {"TK_CL", true, &nodes_booleanExpression_1[3]},
        {"logicalOp", false, &nodes_booleanExpression_1[4]},
        {"TK_OP", true, &nodes_booleanExpression_1[5]},
        {"booleanExpression", false, &nodes_booleanExpression_1[6]},
        {"TK_CL", true, NULL}
    };
    NODE nodes_booleanExpression_2[] = {
        {"var", false, &nodes_booleanExpression_2[1]},
        {"relationalOp", false, &nodes_booleanExpression_2[2]},
        {"var", false, NULL}
    };
    NODE nodes_booleanExpression_3[] = {
        {"TK_NOT", true, &nodes_booleanExpression_3[1]},
        {"TK_OP", true, &nodes_booleanExpression_3[2]},
        {"booleanExpression", false, &nodes_booleanExpression_3[3]},
        {"TK_CL", true, NULL}
    };
    addRule(&grammar, "booleanExpression", nodes_booleanExpression_1);
    addRule(&grammar, "booleanExpression", nodes_booleanExpression_2);
    addRule(&grammar, "booleanExpression", nodes_booleanExpression_3);

    // <var>===> <SingleOrRecId> | TK_NUM | TK_RNUM
    NODE nodes_var_1[] = {
        {"SingleOrRecId", false, NULL}
    };
    NODE nodes_var_2[] = {
        {"TK_NUM", true, NULL}
    };
    NODE nodes_var_3[] = {
        {"TK_RNUM", true, NULL}
    };
    addRule(&grammar, "var", nodes_var_1);
    addRule(&grammar, "var", nodes_var_2);
    addRule(&grammar, "var", nodes_var_3);

    // <logicalOp>===>TK_AND | TK_OR
    NODE nodes_logicalOp_1[] = {
        {"TK_AND", true, NULL}
    };
    NODE nodes_logicalOp_2[] = {
        {"TK_OR", true, NULL}
    };
    addRule(&grammar, "logicalOp", nodes_logicalOp_1);
    addRule(&grammar, "logicalOp", nodes_logicalOp_2);

    // <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
    NODE nodes_relationalOp_1[] = {
        {"TK_LT", true, NULL}
    };
    NODE nodes_relationalOp_2[] = {
        {"TK_LE", true, NULL}
    };
    NODE nodes_relationalOp_3[] = {
        {"TK_EQ", true, NULL}
    };
    NODE nodes_relationalOp_4[] = {
        {"TK_GT", true, NULL}
    };
    NODE nodes_relationalOp_5[] = {
        {"TK_GE", true, NULL}
    };
    NODE nodes_relationalOp_6[] = {
        {"TK_NE", true, NULL}
    };
    addRule(&grammar, "relationalOp", nodes_relationalOp_1);
    addRule(&grammar, "relationalOp", nodes_relationalOp_2);
    addRule(&grammar, "relationalOp", nodes_relationalOp_3);
    addRule(&grammar, "relationalOp", nodes_relationalOp_4);
    addRule(&grammar, "relationalOp", nodes_relationalOp_5);
    addRule(&grammar, "relationalOp", nodes_relationalOp_6);

    // <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
    NODE nodes_returnStmt[] = {
        {"TK_RETURN", true, &nodes_returnStmt[1]},
        {"optionalReturn", false, &nodes_returnStmt[2]},
        {"TK_SEM", true, NULL}
    };
    addRule(&grammar, "returnStmt", nodes_returnStmt);

    // <optionalReturn>===>TK_SQL <idList> TK_SQR | ε
    NODE nodes_optionalReturn_1[] = {
        {"TK_SQL", true, &nodes_optionalReturn_1[1]},
        {"idList", false, &nodes_optionalReturn_1[2]},
        {"TK_SQR", true, NULL}
    };
    NODE nodes_optionalReturn_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "optionalReturn", nodes_optionalReturn_1);
    addRule(&grammar, "optionalReturn", nodes_optionalReturn_2);

    // <idList>===> TK_ID <more_ids>
    NODE nodes_idList[] = {
        {"TK_ID", true, &nodes_idList[1]},
        {"more_ids", false, NULL}
    };
    addRule(&grammar, "idList", nodes_idList);

    // <more_ids>===> TK_COMMA <idList> | ε
    NODE nodes_moreIds_1[] = {
        {"TK_COMMA", true, &nodes_moreIds_1[1]},
        {"idList", false, NULL}
    };
    NODE nodes_moreIds_2[] = {
        {"ε", false, NULL}
    };
    addRule(&grammar, "more_ids", nodes_moreIds_1);
    addRule(&grammar, "more_ids", nodes_moreIds_2);

    // <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    NODE nodes_definetypestmt[] = {
        {"TK_DEFINETYPE", true, &nodes_definetypestmt[1]},
        {"A", false, &nodes_definetypestmt[2]},
        {"TK_RUID", true, &nodes_definetypestmt[3]},
        {"TK_AS", true, &nodes_definetypestmt[4]},
        {"TK_RUID", true, NULL}
    };
    addRule(&grammar, "definetypestmt", nodes_definetypestmt);

    // <A>==>TK_RECORD | TK_UNION
    NODE nodes_A_1[] = {
        {"TK_RECORD", true, NULL}
    };
    NODE nodes_A_2[] = {
        {"TK_UNION", true, NULL}
    };
    addRule(&grammar, "A", nodes_A_1);
    addRule(&grammar, "A", nodes_A_2);

    printGrammar(&grammar);
    // Example: Retrieving rules for a non-terminal
    // RULES* rulesA = getRules(&grammar, "otherFunctions");
    // if (rulesA != NULL) {
    //     for (int i = 0; i < rulesA->length; i++) {
    //         printf("Rule %d:\n", i + 1);
    //         printf("%s\n",rulesA->heads[i].name);
    //         NODE* head = rulesA->heads[i].next;
    //         while (head != NULL) {
    //             printf("Name: %s, Terminal: %s\n", head->name, head->terminal ? "true" : "false");
    //             head = head->next;
    //         }
    //     }
    // }
    // else {
    //     printf("No rules found for non-terminal A1.\n");
    // }

    return 0;
}
