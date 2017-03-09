#pragma once

#include <string>
#include <vector>

void cls();
void gotoxy(int x, int y);

int consoleColor(int back, int text);
void setColor(int value);

void center(std::string message, int row);

int choiceMenu(std::string menu, std::vector<std::string>& items, int initialRow);
void drawScreen(std::string name);

void sleep(int msDelay);

int iDistance(int x1, int y1, int x2, int y2);