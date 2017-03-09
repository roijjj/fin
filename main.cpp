#include <vector>
#include <string>
#include <cstdlib>
#include "Game.h"
#include "utils.h"

using namespace std;

bool mainMenu (Game& game);

int main()
{	
	Game game;

	system("title ASCII RPG");

	if (mainMenu(game)) return 0;
	game.mainLoop();

	return 0;
}

bool mainMenu(Game& game)
{
	vector<string> menu;

	menu.push_back("New Game");
	menu.push_back("Load Game");
	menu.push_back("Exit");

	int sel = choiceMenu("MAIN MENU", menu, -1);  //Center menu vertically

	cls();

	switch(sel) //What did the user select from the menu?
	{
	case 0:
		game.start(false); //Start the game!
		break;

	case 1:
		game.start(true); //Load and start the game
		break;

	case 2:
		return true; //Exit
	}

	return false;
}