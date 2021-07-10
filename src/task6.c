#define _GNU_SOURCE

#include "trie.h"
#include <stdio.h>
#include <string.h>

void main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("NO DATASET FOLDER PROVIDED!!!\n");
        return;
    }

    char *folder = argv[1];
    char dataset[1024];

    trieNode *root = NULL;

    for (int i = 1; i <= 10; i++)
    {
        sprintf(dataset, "%sdata%d.txt", folder, i);
        FILE *datasetFile = fopen(dataset, "r");
        printf("Reading file '%s'\n", dataset);

        char *contents = NULL;
        long contentsLength;

        fseek(datasetFile, 0, SEEK_END);
        contentsLength = ftell(datasetFile);
        contents = malloc(contentsLength);
        fseek(datasetFile, 0, SEEK_SET);
        fread(contents, 1, contentsLength, datasetFile);

        // SRC: https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
        char *line = strtok(contents, "\n");
        while (line)
        {
            trieInsert(&root, line);
            line = strtok(NULL, "\n");
        }

        // NOTE: Remember to free file contents!
        free(contents);
        fclose(datasetFile);
    }

    FILE *dictionary = fopen("Dictionary.txt", "w");
    printf("Saving dictionary\n");
    triePrint(root, dictionary);

    fclose(dictionary);
}
