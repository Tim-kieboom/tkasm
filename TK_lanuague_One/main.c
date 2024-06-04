#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "lib/fileReader/fileReader.h"
#include "lib/HashMap/map.h"
#include "tokenizer/mainTokenizer.h"

int main(void)
{
    arraylist* file = readFile("C:\\Users\\tim_k\\OneDrive\\Documenten\\GitHub\\hobby\\tkasm\\TK_lanuague_One\\main.tkasm");

    map_int_t *labelTracker = malloc(sizeof(map_int_t));
    map_int_t *lineNumberTracker = malloc(sizeof(map_int_t));
    map_init(labelTracker);
    map_init(lineNumberTracker);

    arraylist* tokenLines = tokenizer(file, labelTracker, lineNumberTracker);

    map_deinit(labelTracker);
    map_deinit(lineNumberTracker);
    free(labelTracker);
    free(lineNumberTracker);
    arraylist_destroy(file);
    return 0;
}
