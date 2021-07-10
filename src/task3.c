#define _GNU_SOURCE

#include "trie.h"
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 2048

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

        char buffer[BLOCK_SIZE];
        while (fread(buffer, 1, sizeof(buffer), datasetFile) > 0)
        {
            // SRC: https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
            char *line = strtok(buffer, "\n");
            while (line)
            {
                trieInsert(&root, line);
                line = strtok(NULL, "\n");
            }
            memset(buffer, 0, sizeof(buffer));
        }

        fclose(datasetFile);
    }

    FILE *dictionary = fopen("Dictionary.txt", "w");
    printf("Saving dictionary\n");
    triePrint(root, dictionary);

    fclose(dictionary);
}
