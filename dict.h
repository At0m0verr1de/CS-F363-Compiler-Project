/*
    Group No. - 3
    Aryan Bakshi        2021A7PS0532P
    Aadeesh Garg        2021A7PS0446P
    Vedant Tuli         2021A7PS0435P
    Amandeep Singh      2021A7PS0575P
    Rijul Bassamboo     2021A7PS0009P
    Mihika Deshpande    2021A7PS2435P
*/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 500

struct KeyValuePair
{
    char *key;
    char *value;
};

struct Dictionary
{
    struct KeyValuePair *table[TABLE_SIZE];
};

int hash(char *key);

// Initialize dictionary
struct Dictionary *createDictionary();

// Insert key-value pair into dictionary
void insert(struct Dictionary *dict, char *key, char *value);

// Search for a key in the dictionary and return its value
char *search(struct Dictionary *dict, char *key);

// Function to initialize and populate the lookup table
struct Dictionary *initLookupTable();

#endif /* DICTIONARY_H */
