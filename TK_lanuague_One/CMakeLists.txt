cmake_minimum_required(VERSION 3.28)
project(TK_lanuague_One C)

set(CMAKE_C_STANDARD 11)

add_executable(TK_lanuague_One main.c
        lib/ArrayList/ArrayList.h
        lib/ArrayList/ArrayList.c
        lib/StringStream/StringStream.h
        lib/StringStream/StringStream.c
        lib/fileReader/fileReader.c
        lib/fileReader/fileReader.h
        tokenizer/mainTokenizer.c
        tokenizer/mainTokenizer.h
        metaData/Types.h
        metaData/TkasmTypes.h
        lib/string/String.c
        lib/string/String.h
        lib/HashMap/map.c
        lib/HashMap/map.h
        lib/Stack/Stack.c
        lib/Stack/Stack.h
        lib/stringTools/stringTools.c
        lib/stringTools/stringTools.h)
