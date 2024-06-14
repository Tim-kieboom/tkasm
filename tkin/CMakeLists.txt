cmake_minimum_required(VERSION 3.28)
project(tkin C)

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
        lib/Stream/Stream.c
        lib/Stream/Stream.h
        lib/stringTools/stringTools.c
        lib/stringTools/stringTools.h
        metaData/generalType/generalType.c
        metaData/generalType/generalType.h
        metaData/Commands/commands.c
        metaData/Commands/commands.h
        metaData/debug/debug.c
        metaData/debug/debug.h
        metaData/generalType/console/console.c
        metaData/generalType/console/console.h
        metaData/generalType/operators/operators.c
        metaData/generalType/operators/operators.h
        metaData/memory/stack/stackMemory.c
        metaData/memory/stack/stackMemory.h
        lib/Stack/Stack.c
        lib/Stack/Stack.h
        Interpreter/Interpreter.c
        Interpreter/Interpreter.h)
