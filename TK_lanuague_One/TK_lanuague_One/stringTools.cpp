#include "stringTools.h"
#include <sstream>

#define IS_WHITE_SPACE(ch) ((ch == ' ') || (ch == '\t') || (ch == '\n'))

string trimWhiteSpaces(string& str)
{
	vector<char> token = vector<char>();
	bool isInString = false;

	size_t i = -1;
	while (str[++i] != '\0')
	{
		char letter = str[i];

		if (letter == '\"')
			isInString = !isInString;

		if (!IS_WHITE_SPACE(letter) || (isInString == true))
			token.push_back(letter);
	}

	string strToken(token.begin(), token.end());
	return strToken;
}

vector<string> splitWhiteSpaces(string& str)
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

		if (IS_WHITE_SPACE(letter) && (isInString == false))
		{
			if (token.empty())
				continue;

			string strToken(token.begin(), token.end());

			tokens.push_back(strToken);
			token.clear();
		}

		if (!IS_WHITE_SPACE(letter) || (isInString == true))
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