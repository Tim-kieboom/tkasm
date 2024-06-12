#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "lib/fileReader/fileReader.h"
#include "Interpreter/Interpreter.h"

int main(void)
{
    arraylist* file = readFile("C:\\Users\\tim_k\\OneDrive\\Documenten\\GitHub\\hobby\\tkasm\\TK_lanuague_One\\main.tkasm");

    interpretCode(file);

    arraylist_destroy(file);
    return 0;
}
