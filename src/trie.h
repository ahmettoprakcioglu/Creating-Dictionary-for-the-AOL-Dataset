#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 500

typedef struct trieNode
{
    char c;
    // Most trie implementations don't keep track of frequencies, but store a boolean values
    // on nodes that are end of words
    // I replaced this boolean with an int, where
    // - 0 means that is part of a word
    // - 1+ is end of a word, and is repeated multiple times
    int freq;

    // For memory efficency we use a linked list instead of a fixed size array of pointers
    // Storing multiple childs involves a single child, and that child has siblings
    struct trieNode *child;
    struct trieNode *sibling;
} trieNode;

trieNode *createNode(char c)
{
    trieNode *node = malloc(sizeof(trieNode));
    (*node).c = c;
    (*node).freq = 0;

    (*node).child = NULL;
    (*node).sibling = NULL;

    return node;
}

void trieInsert(trieNode **root, char *text)
{
    if (*root == NULL)
        *root = createNode('\0');

    trieNode **tmp = root;
    for (int i = 0; i < strlen(text); i++)
    {
        tmp = &(*(*tmp)).child;
        while (*tmp)
        {
            if ((*(*tmp)).c == text[i])
                break;

            tmp = &(*(*tmp)).sibling;
        }
        if (*tmp == NULL)
            *tmp = createNode(text[i]);
    }
    (*(*tmp)).freq++;
}

void trieMerge(trieNode **to, trieNode **from)
{
    if ((*from) == NULL)
        return;

    char buff[MAXLINE];
    trieMerge_aux(to, (*(*from)).child, buff, 0);
}

void triePrint(trieNode *root, FILE *fp)
{
    if (root == NULL)
    {
        printf("Trie: empty\n");
        return;
    }
    char buff[MAXLINE];
    triePrint_aux((*root).child, fp, buff, 0);
}

// ********************
// AUXILIARY FUNCTIONS
// ********************

void trieMerge_aux(trieNode **to, trieNode *node, char *buff, int charPos)
{
    if (node == NULL)
        return;

    trieNode *tmp = node;
    while (tmp)
    {
        buff[charPos] = (*tmp).c;
        if ((*tmp).freq > 0)
        {
            buff[charPos + 1] = '\0';
            trieInsert(to, buff);
        }
        trieMerge_aux(to, (*tmp).child, buff, charPos + 1);

        tmp = (*tmp).sibling;
    }
}

void triePrint_aux(trieNode *node, FILE *file, char *buff, int charPos)
{
    if (node == NULL)
        return;

    trieNode *tmp = node;
    while (tmp)
    {
        buff[charPos] = (*tmp).c;
        int freq = (*tmp).freq;
        if (freq)
        {
            buff[charPos + 1] = '\0';
            fprintf(file, "%s\t%d\n", buff, freq);
        }
        triePrint_aux((*tmp).child, file, buff, charPos + 1);

        tmp = (*tmp).sibling;
    }
}
