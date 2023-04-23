#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int count;
};

void insert(struct Trie **ppTrie, char *word)
{
    struct Trie *pTrie = *ppTrie;
    if (!pTrie)
    {
        pTrie = (struct Trie *)calloc(1, sizeof(struct Trie));
        *ppTrie = pTrie;
    }
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (!pTrie->children[index])
        {
            pTrie->children[index] = (struct Trie *)calloc(1, sizeof(struct Trie));
        }
        pTrie = pTrie->children[index];
    }
    pTrie->count++;
}

int numberOfOccurances(struct Trie *pTrie, char *word)
{
    if (!pTrie)
        return 0;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (!pTrie->children[index])
        {
            return 0;
        }
        pTrie = pTrie->children[index];
    }
    return pTrie->count;
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie)
        return NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        pTrie->children[i] = deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
    return NULL;
}

int main(void)
{
    int n = 5; // number of words in the dictionary
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    struct Trie *trie = NULL;
    for (int i = 0; i < n; i++)
    {
        insert(&trie, pWords[i]);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s : %d\n", pWords[i], numberOfOccurances(trie, pWords[i]));
    }
    trie = deallocateTrie(trie);
    if (trie != NULL)
    {
        printf("There is an error in this program\n");
    }
    return 0;
}