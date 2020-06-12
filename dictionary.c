// Implements a dictionary's functionality

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000000;

// Hash table
node *table[N];

// Words in Hash table
unsigned wordCount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int checkHash = hash(word);
    if (table[checkHash] != NULL)
    {
        node *checkPointer = table[checkHash];
        while (checkPointer != NULL)
        {
            if (strcasecmp(checkPointer->word, word) == 0)
            {
                return true;
            }
            checkPointer = checkPointer->next;
        }
    }
    return false;
}

// Hashes word to a number
// Derived from Robert Sedgewick's string hashing algorithm
// https://stackoverflow.com/questions/6136964/rs-hashing-program/6137031
unsigned int hash(const char *word)
{
    unsigned int len = strlen(word);

    // convert given word to lower case
    char lowerWord[len + 1];
    strcpy(lowerWord, word);

    for (int i = 0; i < len + 1; i++)
    {
        lowerWord[i] = tolower(lowerWord[i]);
    }

    unsigned int result = 0;
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int i    = 0;

    for (i = 0; i < len; word++, i++)
    {
        result = result * a + (*lowerWord);
        a = a * b;
    }
    return result % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initializing all indexes of hashtable to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Read one word at a time from dictionary
    char dictWord[LENGTH + 1];
    while (fscanf(dict, "%s", dictWord) != EOF)
    {
        // Populating node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, dictWord);
        n->next = NULL;

        // Placing node in hash table
        unsigned int tmpHash = hash(dictWord);
        if (table[tmpHash] == NULL)
        {
            table[tmpHash] = n;
            wordCount++;
        }
        else
        {
            node *tmpPointer = table[tmpHash];
            if (tmpPointer == NULL)
            {
                return false;
            }
            while (tmpPointer->next != NULL)
            {
                tmpPointer = tmpPointer->next;
            }
            tmpPointer->next = n;
            wordCount++;
        }
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *unloadCurrent = table[i];
        {
            while (unloadCurrent != NULL)
            {
                node *unloadNext = unloadCurrent->next;
                free(unloadCurrent);
                unloadCurrent = unloadNext;
            }
        }
    }
    return true;
}
