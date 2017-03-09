#include "utils.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <conio.h>
#include "strings.h"

#ifdef WIN32
	#include <Windows.h>
#endif

using namespace std;

void cls()
{
#ifdef WIN32
	system("cls");
#else //Probably on UNIX systems it would be system("clear") but I'm not sure
	cout << string(100, '\n');
#endif
}

void gotoxy(int x, int y)
{
#ifdef WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD p1;

	p1.X = x;
	p1.Y = y; 

	SetConsoleCursorPosition(console, p1);
#endif
}

int consoleColor(int back, int text)
{
	return (back << 4) | text;
}

void setColor(int value)
{
#ifdef WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (WORD) value);
#endif
}

void center(string message, int row)
{
	gotoxy(40 - message.size()/2, row);
	cout << message;
}

int choiceMenu(string menu, vector<string>& items, int initialRow=-1)
{
	if (initialRow == -1) //CENTER VERTICALLY
		initialRow = 12-items.size()/2;

	int selection = 0;

	while (true)
	{
		cls();

		center(menu, 0);

		for (unsigned int i=0; i<items.size(); i++)
		{
			if (selection == i)
				center("> "+items[i]+" <", i + initialRow);
			else
				center(items[i], i + initialRow);
		}

		char input = lowCase(_getch());

		switch(input)
		{
		case 'w':
			selection--;
			if (selection < 0)
				selection = items.size()-1;

			break;

		case 's':
			selection++;
			if (selection >= static_cast<int>(items.size()))
				selection = 0;
			break;

		case K_ESC:
			return -1;

		case K_ENTER:
			return selection;

		default:
			break;
		}
	}
}

void drawScreen(std::string name)
{
	string path = "Data\\screens\\"+name+".txt";

	ifstream in;
	in.open(path);

	if (in.fail())
	{
		cout << "Could not open file '"<< path << "'.\n";
		system("pause");
		return;
	}

	vector<string> buffer;

	while (!in.eof())
	{
		string line;
		getline(in, line);
		buffer.push_back(line);
	}

	for (unsigned int i=0; i<buffer.size() && i < 24; i++)
	{
		printf("%.79s\n", buffer[i].c_str());
	}


	in.close();
}

void sleep(int msDelay)
{
	if (msDelay > 9)
		Sleep(msDelay);
}

int iDistance(int x1, int y1, int x2, int y2)
{
	return (int)(sqrt((float)(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}