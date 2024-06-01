#include "stringTools.h"
#include <sstream>

#define IS_WHITE_SPACE(ch) ((ch == ' ') || (ch == '\t') || (ch == '\n'))

template<typename T>
T stringTo_uint(char* string)
{
    T prevNumbers = 0;

    int16_t i = -1;
    while (string[++i] != '\0')
    {
        prevNumbers *= 10;
        T currentNumber = (T)(string[i] - '0');
        prevNumbers += currentNumber;
    }

    return prevNumbers;
}

template<typename T>
T stringTo_uint(const char* string)
{
    return stringTo_uint<T>((char*)string);
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

vector<string> splitWhiteSpaces(string &str)
{
    vector<char> token = vector<char>();
    vector<string> tokens;

    bool isInString = false;

    size_t i = -1;
    while (str[++i] != '\0')
    {
        char letter = str[i];

        if (letter == '\"')
            isInString = !isInString;

        if(IS_WHITE_SPACE(letter) && (isInString == false) )
        {
            if(token.empty())
                continue;

            string strToken(token.begin(), token.end());

            tokens.push_back(strToken);
            token.clear();
        }

        if(!IS_WHITE_SPACE(letter) || (isInString == true) )
            token.push_back(letter);
    }

    if (!token.empty())
    {
        string strToken(token.begin(), token.end());
        tokens.push_back(strToken);
    }

    return tokens;
}

vector<string> splitString(string& str, char delimiter) 
{
    stringstream ss(str);
    string token;
    vector<string> tokens;

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