// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of letters plus apostrophe
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root = NULL;

// Prototypes of help functions
node *create_node();
unsigned int size_node(node *pnode);
bool unload_node(node *pnode);

// Creates node
node *create_node()
{
    node *pnode;
    if ((pnode = malloc(sizeof(node))))
    {
        pnode->is_word = false;
        for (int i = 0; i < N; i++)
        {
            pnode->children[i] = NULL;
        }
    }

    return pnode;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    if (!(root = create_node()))
    {
        return false;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie:
    // For every dictionary word
    while (fscanf(file, "%s", word) != EOF)
    {
        // Iterate through the trie starting from root
        node *pnode = root;

        // For every letter in word
        for (char *pc = word; *pc != '\0'; pc++)
        {
            int i = 26;
            if (*pc != '\'')
            {
                i = *pc - 'a';
            }

            // If letter is not in trie, create node for letter
            node **pchildren = pnode->children + i;
            if (! *pchildren)
            {
                if (!(*pchildren = create_node()))
                {
                    unload();
                    return false;
                }
            }

            // Go to child node corresponding to this letter
            pnode = *pchildren;
        }

        // Word inserted
        pnode->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words below a node in dictionary
unsigned int size_node(node *pnode)
{
    // For every not empty child, add number of words below it
    unsigned int nwords = 0;
    for (int i = 0; i < N; i++)
    {
        if (pnode->children[i])
        {
            nwords += size_node(pnode->children[i]);
        }
    }

    // If node is a word, add 1
    if (pnode->is_word)
    {
        nwords++;
    }

    return nwords;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_node(root);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Iterate through the trie starting from root
    node *pnode = root;

    // For every letter in word
    for (int i = 0; i < strlen(word); i++)
    {
        char c = word[i];
        int iletter = 26;
        if (c != '\'')
        {
            iletter = tolower(c) - 'a';
        }

        // If letter is not in trie, word is not in dictionary, return false
        if (! pnode->children[iletter])
        {
            return false;
        }

        // Go to child node corresponding to this letter
        pnode = pnode->children[iletter];
    }

    // End of word; if it is in dictionary
    return pnode->is_word;
}

// Unload node
bool unload_node(node *pnode)
{
    // For every not empty child, unload it
    bool fail = false;
    for (int i = 0; i < N; i++)
    {
        if (pnode->children[i])
        {
            fail += !unload_node(pnode->children[i]);
        }
    }

    // If all children successfully unloded, free node
    if (!fail)
    {
        free(pnode);
    }

    return !fail;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Unload root node
    if (root == NULL)
    {
        return true;
    }
    else
    {
        return unload_node(root);
    }

}
