// Implements a dictionary's functionality

# define _GNU_SOURCE

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Number of buckets in hash table
//const unsigned int N = 1;
const unsigned int N = 65536 * 3;

// Hash table
char table[N][LENGTH + 1];

// Dictionary size
unsigned int dic_size = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
//    return false;

    // Check if the word is in the corresponding list
    unsigned int entry = hash(word);
    for (unsigned int i = entry; i < N; i++)
    {
        if (!table[i][0])
        {
            return false;
        }
        else if (strcasecmp(word, &table[i][0]) == 0)
        {
            return true;
        }
    }

    for (unsigned int i = 0; i < entry; i++)
    {
        if (!table[i][0])
        {
            return false;
        }
        else if (strcasecmp(word, &table[i][0]) == 0)
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
    for (unsigned int i = 0; i < N; i++)
    {
        table[i][0] = '\0';
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
        word[ncr - 1] = '\0';

        unsigned int entry = hash(word);
        unsigned int i;
        for (i = entry; i < N; i++)
        {
            if (!table[i][0])
            {
                strcpy(&table[i][0], word);
                break;
            }
        }

        if (i >= N)
        {
            for (i = 0; i < entry; i++)
            {
                if (!table[i][0])
                {
                    strcpy(&table[i][0], word);
                    break;
                }
            }
            if (i >= entry)
            {
                free(word);
                fclose(file);
                unload();
                return false;
            }

        }

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

    // Zero dictionary size
    dic_size = 0;

    return true;

}
