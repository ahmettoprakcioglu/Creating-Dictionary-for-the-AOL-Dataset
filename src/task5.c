#define _GNU_SOURCE

#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUMFILES 10

char globalFilepaths[NUMFILES][1024];
trieNode *globalTries[NUMFILES];

void *threadCallback(void *pIdx)
{
    int idx = (*(int *)pIdx);

    FILE *myFile = fopen(globalFilepaths[idx], "r");
    printf("[Thread %d] Reading file '%s'\n", idx, globalFilepaths[idx]);

    char *line = malloc(MAXLINE * sizeof(char));
    int size = sizeof(line);
    int lineLength = 0;
    while ((lineLength = getline(&line, &size, myFile)) >= 0)
    {
        // Remove the '\n' character
        line[lineLength - 1] = '\0';
        trieInsert(&globalTries[idx], line);
    }
    fclose(myFile);

    return NULL;
}

void main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("NO DATASET FOLDER PROVIDED!!!\n");
        return;
    }

    char *folder = argv[1];
    int threadIdx[NUMFILES];
    pthread_t tids[NUMFILES];
    for (int i = 0; i < NUMFILES; i++)
    {
        threadIdx[i] = i;
        globalTries[i] = NULL;
        sprintf(globalFilepaths[i], "%sdata%d.txt", folder, i + 1);

        pthread_create(&tids[i], NULL, threadCallback, &threadIdx[i]);
    }

    for (int i = 0; i < NUMFILES; i++)
        pthread_join(tids[i], NULL);

    trieNode *finalTrie = NULL;
    printf("Merging\n");
    for (int i = 0; i < NUMFILES; i++)
        trieMerge(&finalTrie, &globalTries[i]);

    FILE *dictionary = fopen("Dictionary.txt", "w");
    printf("Saving dictionary\n");
    triePrint(finalTrie, dictionary);
    fclose(dictionary);
}
