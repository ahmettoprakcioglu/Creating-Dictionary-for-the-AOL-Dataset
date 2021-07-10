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

        // SRC: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
        char *line = malloc(MAXLINE * sizeof(char));
        int size = sizeof(line);
        int lineLength = 0;
        while ((lineLength = getline(&line, &size, datasetFile)) >= 0)
        {
            // Remove the '\n' character
            line[lineLength - 1] = '\0';
            trieInsert(&root, line);
        }

        fclose(datasetFile);
    }

    FILE *dictionary = fopen("Dictionary.txt", "w");
    printf("Saving dictionary\n");
    triePrint(root, dictionary);
    fclose(dictionary);
}
