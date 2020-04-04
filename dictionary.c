// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
#define N 20000

// Hash table
node *table[N];

// Variable to track the number of words in dictionary
int wordCount;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word to obtain hash value
    unsigned int wordHash = hash(word);

    // Access linked list at index returned by hash function
    // Create temp node to be used for traverse
    node *temp = table[wordHash];

    // Traverse linked list searching for the word
    while (temp != NULL)
    {
        if (strcasecmp(table[wordHash]->word, word) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Hashes word to a number
// Djb2 hash function
// Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dictionaryFile = fopen(dictionary, "r");

    if (!dictionaryFile)
    {
        printf("Unable to open dictionary file for read access\n");
        unload();
        return false;
    }

    // Create variable for buffer word
    char word[LENGTH + 1];

    // Read strings from file one at a time until EOF
    while (fscanf(dictionaryFile, "%s", word) != EOF)
    {
        // Hash the word
        unsigned int wordHash = hash(word);

        // Allocate memory and create new node
        node *wordCopy = malloc(sizeof(node));
        if (wordCopy == NULL)
        {
            unload();
            return false;
        }
        
        // Copy word into node
        strcpy(wordCopy->word, word);

        // Insert node into hash table
        wordCopy->next = table[wordHash];
        table[wordHash] = wordCopy;
        
        
    }
    fclose(dictionaryFile);
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    wordCount = 0;
    
    // Loop through each bucket
    for (int i = 0; i < N; i++)
    {
        node *loop = table[i];
        
        // Loop through each word in each bucket
        while (loop != NULL)
        {
            wordCount++;
            loop = loop->next;
        }
    }
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *temp = table[i];
        
        while (table[i] != NULL)
        {
            temp = temp->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return true;
}
