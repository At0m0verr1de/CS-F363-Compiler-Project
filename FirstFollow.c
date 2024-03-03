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

// Read the grammar from the file
void readGrammar()
{
    FILE *fp;
    char buffer[MAX_LEN];

    fp = fopen("grammar.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening grammar file.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        strcpy(productions[num_productions++], buffer);
    }

    fclose(fp);
}

// Calculate First sets
void calculateFirst()
{
    int i, j, k, l;
    char *production;
    int changed;

    // Initialize First sets with epsilon
    for (i = 0; i < num_productions; i++)
    {
        if (productions[i][0] != '<')
        {
            first_set[i][0] = productions[i][0];
        }
    }

    // Iterate until there are no changes in First sets
    do
    {
        changed = 0;

        for (i = 0; i < num_productions; i++)
        {
            production = productions[i];
            j = 0;

            while (production[j] != '>' && production[j] != '\0')
            {
                if (!isupper(production[j]))
                {
                    // Terminal symbol
                    if (production[j] != ' ')
                    {
                        if (!strchr(first_set[i], production[j]))
                        {
                            strcat(first_set[i], " ");
                            strncat(first_set[i], &production[j], 1);
                            changed = 1;
                        }
                        break;
                    }
                }
                else
                {
                    // Non-terminal symbol
                    for (k = 0; k < num_productions; k++)
                    {
                        if (productions[k][0] == production[j])
                        {
                            for (l = 0; productions[k][l] != '>' && productions[k][l] != '\0'; l++)
                            {
                                if (productions[k][l] == ' ')
                                {
                                    continue;
                                }
                                if (!strchr(first_set[i], productions[k][l]))
                                {
                                    strcat(first_set[i], " ");
                                    strncat(first_set[i], &productions[k][l], 1);
                                    changed = 1;
                                }
                                if (productions[k][l] != '#')
                                {
                                    break;
                                }
                            }
                        }
                    }
                    if (!strchr(productions[i], '#'))
                    {
                        break;
                    }
                }
                j++;
            }
        }
    } while (changed);
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
