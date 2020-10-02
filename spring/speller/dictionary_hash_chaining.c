// Implements a dictionary's functionality

# define _GNU_SOURCE

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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
//const unsigned int N = 32771;
const unsigned int N = 65536 * 2;

// Hash table
node *table[N];

// Dictionary size
unsigned int dic_size = 0;


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
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }

    return hash % N;

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
    dic_size = 0;
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    // Allocate buffer for a word
    char *word;
    size_t bs = LENGTH + 2;
    if (!(word = malloc(bs)))
    {
        fclose(file);
        unload();
        return false;
    }

    // Get and insert words into hash table
    ssize_t ncr;
    while ((ncr = getline(&word, &bs, file)) > 0)
    {
        char *lc = word + ncr - 1;
        if (*lc == '\n')
        {
            *lc = '\0';
        }

        // Create a node with the word
        node *ptr;
        if (!(ptr = malloc(sizeof(node))))
        {
            free(word);
            fclose(file);
            unload();
            return false;
        }
        strcpy(ptr->word, word);

        // Insert the node in the corresponding list
        node **entry = table + hash(word);
        ptr->next = *entry;
        *entry = ptr;

        // Increment dictionary size
        dic_size++;
    }

    // Free word
    free(word);

    // Close dictionary
    fclose(file);

    // Indicate success
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

    // Iterate over hashtable
    node **entry = table;
    node **end = table + N;
    while (entry < end)
    {
        // Free nodes in each corresponding list
        node *ptr = *entry++;
        while (ptr)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }

    // Zero dictionary size
    dic_size = 0;

    return true;

}
