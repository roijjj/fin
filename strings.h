#pragma once
#include <string>
#include <vector>

enum Keys
{
	K_ESC = 27,
	K_ENTER = 13,
};

char lowCase(char c);
char upCase(char c);

bool contains(const std::string& str, char c);
bool contains(const std::string& str, const char* cs);

std::string split(std::string& str, char c, int occ);
void splitInto(std::string& str, char c, std::vector<std::string>& result);

std::string lowerCase(std::string& str);
std::string upperCase(std::string& str);