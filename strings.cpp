#include "strings.h"
#include <vector>

using namespace std;

//CHAR
char lowCase(char c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return c;
}

char upCase(char c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return c;
}


//STRINGS
bool contains(const string& str, char c)
{
	for (unsigned int i=0; i<str.size(); i++)
		if (str[i] == c)
			return true;
	return false;
}

bool contains(const string& str, const char* cs)
{
	for (unsigned int i=0; i<str.size(); i++)
		if (contains(string(cs), str[i]))
			return true;
	return false;
}

string split(string& str, char c, int occ)
{
	string result;
	unsigned int i;

	bool isCopying=false;

	for (i=0; i<str.length(); i++)
	{
		if (str[i] == c)
		{
			if (isCopying)
				isCopying = false;
			occ--;
		}

		if (occ==0)
			isCopying = true;

		if (isCopying && str[i] != c)
			result += str[i];
	}
	
	return result;
}

void splitInto(string& str, char c, vector<string>& result)
{
	unsigned int i, j=0, cnt=0;

	for (i=0; i<str.length(); i++)
		if (str[i] == c) cnt++;

	for (i=0; i<=cnt; i++)
		result.push_back(string());
	
	for (i=0; i<str.length(); i++)
	{
		if (str[i] == c)
		{
			j++;
		}
		else
		{
			result[j] += str[i];
		}
	}
}

string lowerCase(string& str)
{
	string result;
	for (unsigned int i=0; i<str.size(); i++)
		result += lowCase(str[i]);
	return result;
}

string upperCase(string& str)
{
	string result;
	for (unsigned int i=0; i<str.size(); i++)
		result += upCase(str[i]);
	return result;
}