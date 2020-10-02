// Implements a dictionary's functionality

#define _XOPEN_SOURCE

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 2];
    struct node *next;
}
node;

// Number of buckets in hash table
//const unsigned int N = 1;
const unsigned int N = 1000;

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

    // Convert word to lower case
    char w[LENGTH + 1];
    char *d = w;
    for (const char *s = word; *s; d++, s++)
    {
        *d = tolower(*s);
    }
    *d = '\0';

    // Check if the word is in the corresponding list
    for (node *ptr = table[hash(w)]; ptr; ptr = ptr->next)
    {
        if (strcmp(w, ptr->word) == 0)
        {
            return true;
        }
    }

    return false;

}


uint32_t hash_djb2(const unsigned char *key, size_t n);
uint32_t hash_fnv1a_32(const unsigned char *key, size_t n);
uint16_t hash_pearson_16(const unsigned char *key, size_t n);

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
//    return 0;

    return hash_djb2((const unsigned char *)word, N); // primeiro colocado
//    return hash_fnv1a_32((const unsigned char *)word, N); // segundo colocado
//    return hash_pearson_16((const unsigned char *)word, N); // terceiro colocado

}


// djb2 hash function
uint32_t hash_djb2(const unsigned char *key, size_t n)
{
    // djb2 algorithm by Dan Bernstein
    uint32_t hash = 5381;

    unsigned char c;
    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
//        hash = ((hash << 5) + hash) ^ c; // hash * 33 ^ c
    }

    return hash % n;

}


// Fowler–Noll–Vo is a non-cryptographic hash function created by Glenn Fowler, Landon Curt Noll, and Kiem-Phong Vo.
#define FNV1_PRIME_32 0x01000193 // 16777619
#define FNV1_BASE_32 2166136261U // 0x811c9dc5

// FNV-1a 32 bits hash fuction
uint32_t hash_fnv1a_32(const unsigned char *key, size_t n)
{
    // FNV- 1a 32 bits algorithm
    uint32_t hash = FNV1_BASE_32;

    unsigned char c;
    while ((c = *key++))
    {
        hash = (hash ^ c) * FNV1_PRIME_32;
    }

    return hash % n;
}


// Pearson lookup table
const unsigned int T_SIZE = 256;
unsigned char T[T_SIZE] =
{
    // TODO: Add 0-255 shuffled in any (random) order
};

// Pearson hashing hash function
uint16_t hash_pearson_16(const unsigned char *key, size_t n)
{
    unsigned char hh[2];

    for (unsigned int j = 0; j < 2; ++j)
    {
        unsigned char h = 0;

        // Change the first byte
        const unsigned char *p = key;
        unsigned char c = *p++;
        if (!c)
        {
            return 0;
        }

        h = T[(c + j) % 256];

        // Compute remainig bytes
        while ((c = *p++))
        {
            h = T[h ^ c];
        }

        hh[j] = h;
    }

    return ((hh[0] << 8) | hh[1]) % n;


}


// Shuffle an array
void shuffle(void *base, size_t nmemb, size_t size)
{
    unsigned char tmp[size];
    unsigned char *arr = base;
    size_t stride = size * sizeof(char);

    srand48(time(NULL));

    if (nmemb <= 1)
    {
        return;
    }

    for (size_t i = nmemb - 1; i > 0; i--)
    {
        size_t j = drand48() * (i + 1);

        memcpy(tmp, arr + j * stride, size);
        memcpy(arr + j * stride, arr + i * stride, size);
        memcpy(arr + i * stride, tmp, size);
    }

}


void shuffle(void *base, size_t nmemb, size_t size);

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

//    for (unsigned int i = 0; i < T_SIZE; i++)
//    {
//        T[i] = i;
//    }
//    shuffle(T, T_SIZE, sizeof(unsigned char));

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
//        if (*entry)
//        {
//            collisions++;
//        }
//        else
//        {
//            not_collisions++;
//        }

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

//    printf("collisions: %d   not collisions:%d   total: %d\n", collisions, not_collisions, collisions + not_collisions);

    return true;

}
