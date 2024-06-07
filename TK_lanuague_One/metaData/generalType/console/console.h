//
// Created by tim_k on 05/06/2024.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "../../Types.h"

void* stringToType(const TkasmType *type, const char* rawValue, /*out*/bool *isSuccess);
void* readTypeFromConsole(const TkasmType *type, /*out*/bool *isSuccess);
void printTypeToConsole(const TkasmType *type, void* value);

#endif //CONSOLE_H
