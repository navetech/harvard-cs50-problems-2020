// Implements a dictionary's functionality

# define _GNU_SOURCE

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <limits.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
//const unsigned int N = 1;
//const unsigned int N = 49152;
const unsigned int N = 65536;


// Hash table
node *table[N];

// Nodes table
const unsigned int N_WORDS = 1000000;
node nodes[N_WORDS];

// Dictionary size
unsigned int dic_size = 0;

unsigned int collisions = 0;
unsigned int not_collisions = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
//    return false;

    // Check if the word is in the corresponding list
    for (node *ptr = table[hash(word)]; ptr; ptr = ptr->next)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }
    }

    return false;

}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
//    return 0;

    // djb2 algorithm by Dan Bernstein
    unsigned int hash = 5381;

    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c); // hash * 33 + c
//        hash = ((hash << 5) + hash) ^ tolower(c); // hash * 33 ^ c
    }

    return hash % N;



/*
//  FNV-1a hash
    unsigned h = 0x811c9dc5;
    int c;

    while ((c = *word++))
    {
      h = ( h ^ tolower(c) ) * 0x01000193;
    }

    return h % N;
*/

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
//    return false;

    // Initialize hash table
    node **p = table;
    node **end = table + N;
    while (p < end)
    {
        *p++ = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    // Get and insert words into hash table
    bool eof = false;
    unsigned int i;
    for (i = 0; i < N_WORDS; i++)
    {
        node *ptr = &nodes[i];
        char *pc;
        pc = fgets(ptr->word, LENGTH + 2, file);
        if (!pc)
        {
            if (feof(file))
            {
                break;
            }
            else
            {
                fclose(file);
                unload();
                return false;
            }
        }
        ptr->word[strlen(ptr->word) - 1] = '\0';

        // Insert the node in the corresponding list
        node **entry = table + hash(ptr->word);
/*        if (*entry)
        {
            collisions++;
        }
        else
        {
            not_collisions++;
        }
*/
        ptr->next = *entry;
        *entry = ptr;

    }

    dic_size = i;

    // Close dictionary
    fclose(file);

    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
//    return 0;

    // Return dictionary size
    return dic_size;

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
//    return false;

    // Zero dictionary size
    dic_size = 0;

//    printf("collisions: %d   not collisions:%d   total: %d\n", collisions, not_collisions, collisions + not_collisions);

    return true;

}
