// game-engine.cpp : Defines the entry point for the application.
//

#include "game-engine.hpp"
#include "Game.hpp"

#undef main

using namespace std;

int main(int argc, char* argv[])
{
	Game game;

	bool success = game.Initialize();

	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}
