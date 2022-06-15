#pragma once

#ifndef GAME_HPP // include guard
#define GAME_HPP

#include <SDL.h>

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	// Initialize the game
	bool Initialize();

	// Run the game loop
	void RunLoop();

	// Cleanup and shut down the game
	void Shutdown();

private:
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	SDL_Window* mWindow;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	bool mIsRunning;

	Vector2 mMovementDir;

	Vector2 mCharacterPos;
};

#endif