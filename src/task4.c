#define _GNU_SOURCE

#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

trieNode *globalRoot;
pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

void *threadCallback(void *filepath)
{
    FILE *myFile = fopen(filepath, "r");
    printf("Reading file '%s'\n", filepath);

    char *line = malloc(MAXLINE * sizeof(char));
    int size = sizeof(line);
    int lineLength = 0;
    while ((lineLength = getline(&line, &size, myFile)) >= 0)
    {
        // Remove the '\n' character
        line[lineLength - 1] = '\0';
        pthread_mutex_lock(&globalLock);
        trieInsert(&globalRoot, line);
        pthread_mutex_unlock(&globalLock);
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

    pthread_t tids[10];
    char *files[10];

    for (int tid = 0; tid < 10; tid++)
    {
        files[tid] = malloc(1024 * sizeof(char));
        sprintf(files[tid], "%sdata%d.txt", folder, tid + 1);

        pthread_create(&tids[tid], NULL, threadCallback, files[tid]);
    }

    for (int i = 0; i < 10; i++)
        pthread_join(tids[i], NULL);

    FILE *dictionary = fopen("Dictionary.txt", "w");
    printf("Saving dictionary\n");
    triePrint(globalRoot, dictionary);
    fclose(dictionary);
}
