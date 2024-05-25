#include "stringTools.h"

#include <sstream>

int16_t stringTo_int16(char* string)
{
    int16_t prevNumbers = 0;

    int16_t i = -1;
    while (string[++i] != '\0')
    {
        prevNumbers *= 10;
        int16_t currentNumber = (int16_t)(string[i] - '0');
        prevNumbers += currentNumber;
    }

    return prevNumbers;
}

int16_t stringTo_int16(const char* string)
{
    return stringTo_int16((char*)string);
}

int32_t stringTo_int32(char* str)
{
    int32_t prevNumbers = 0;
    bool isNegative = str[0] == '-';

    uint16_t i = (isNegative) ? 0 : -1;
    while (str[++i] != '\0')
    {
        prevNumbers *= 10;
        int32_t currentNumber = (int32_t)(str[i] - '0');
        prevNumbers += currentNumber;
    }

    if (isNegative)
        prevNumbers *= -1;

    return prevNumbers;
}

int32_t stringTo_int32(const char* string)
{
    return stringTo_int32((char*)string);
}

vector<string> splitString(string& str, char delimiter) 
{
    stringstream ss(str);
    string token;
    vector<std::string> tokens;

    while (getline(ss, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

int32_t sizeofString(char* string)
{
    int32_t i = -1;
    while (string[++i] != '\0');
    return i;
}

int32_t sizeofString(const char* string)
{
    return sizeofString((char*)string);
}

char stringLastChar(const char* string)
{
    return stringLastChar((char*)string);
}

char stringLastChar(char* string)
{
    int32_t lastIndex = sizeofString(string) - 1;

    return string[lastIndex];
}