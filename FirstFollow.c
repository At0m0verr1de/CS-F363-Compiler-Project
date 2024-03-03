#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROD 100
#define MAX_LEN 100

// Function prototypes
void calculateFirst();
void calculateFollow();
void readGrammar();
void writeFirstAndFollow();

// Global variables
char productions[MAX_PROD][MAX_LEN];
char first_set[MAX_LEN][MAX_LEN];
char follow_set[MAX_LEN][MAX_LEN];
int num_productions = 0;

int main()
{
    readGrammar();
    calculateFirst();
    calculateFollow();
    writeFirstAndFollow();
    return 0;
}

void readGrammar()
{
    FILE *fp;
    char buffer[MAX_LEN];

    // Open the grammar file for reading
    fp = fopen("grammar.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening grammar file.\n");
        exit(1);
    }

    // Read the grammar productions
    while (fgets(buffer, MAX_LEN, fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        strcpy(productions[num_productions++], buffer);
    }

    // Close the file
    fclose(fp);
}

void calculateFirst()
{
    // Implement first set calculation
    // Your code here
}

void calculateFollow()
{
    // Implement follow set calculation
    // Your code here
}

void writeFirstAndFollow()
{
    FILE *fp;
    int i, j;

    // Open the output file for writing
    fp = fopen("FirstAndFollow.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening output file.\n");
        exit(1);
    }

    // Write the First sets to the file
    fprintf(fp, "First\n");
    for (i = 0; i < num_productions; i++)
    {
        fprintf(fp, "%-30s", productions[i]);
        for (j = 0; j < MAX_LEN; j++)
        {
            if (first_set[i][j] != '\0')
            {
                fprintf(fp, "%c ", first_set[i][j]);
            }
        }
        fprintf(fp, "\n");
    }

    // Write a separator between First and Follow sets
    fprintf(fp, "\nFollow\n");

    // Write the Follow sets to the file
    for (i = 0; i < num_productions; i++)
    {
        fprintf(fp, "%-30s", productions[i]);
        for (j = 0; j < MAX_LEN; j++)
        {
            if (follow_set[i][j] != '\0')
            {
                fprintf(fp, "%c ", follow_set[i][j]);
            }
        }
        fprintf(fp, "\n");
    }

    // Close the file
    fclose(fp);
}
