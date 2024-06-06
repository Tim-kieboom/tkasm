#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "lib/fileReader/fileReader.h"
#include "lib/HashMap/map.h"
#include "tokenizer/mainTokenizer.h"
#include "lib/Stack/Stack.h"

int main(void)
{
    arraylist* file = readFile("C:\\Users\\tim_k\\OneDrive\\Documenten\\GitHub\\hobby\\tkasm\\TK_lanuague_One\\main.tkasm");

        printf("file: %d lines\n", file->size);
        printf("\n============================== FILE ==============================\n\n");
        const char* token;
        int i;
        arraylist_iterate(file, i, token)
        {
            printf("%s", token);
        }
        printf("\n");

    map_int_t *labelTracker = malloc(sizeof(map_int_t));
    map_int_t *lineNumberTracker = malloc(sizeof(map_int_t));
    map_init(labelTracker);
    map_init(lineNumberTracker);

    arraylist* tokenLines = tokenizer(file, labelTracker, lineNumberTracker);

        printf("\n============================== TOKENIZER ==============================\n\n");
        arraylist_iterate(tokenLines, i, token)
        {
            printf("%s\n", token);
        }

    map_deinit(labelTracker);
    map_deinit(lineNumberTracker);
    free(labelTracker);
    free(lineNumberTracker);
    arraylist_destroy(file);
    return 0;
}
