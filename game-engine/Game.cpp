#include "Game.hpp"

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mMovementDir() {}

bool Game::Initialize()
{
	// Initialize the library
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Failed to initialize the library");

		return false;
	}

	// Create the window
	mWindow = SDL_CreateWindow("Game Engine", 100, 100, 1024, 768, 0);

	if (!mWindow)
	{
		SDL_Log("Failed to create the window");

		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mCharacterPos.x = (1024.0f - 100.0f) / 2.0f;
	mCharacterPos.y = (768.0f - 100.0f) / 2.0f;

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	// Calling this function removes an event from the queue and stores it in the pointer
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Event received when close button or keyboard shortcut is pressed
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// Get the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Check if escape was pressed
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mMovementDir.x = 0;
	mMovementDir.y = 0;
	if (state[SDL_SCANCODE_W])
	{
		mMovementDir.y -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mMovementDir.y += 1;
	}
	if (state[SDL_SCANCODE_A])
	{
		mMovementDir.x -= 1;
	}
	if (state[SDL_SCANCODE_D])
	{
		mMovementDir.x += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mCharacterPos.y += mMovementDir.y * 300.0f * deltaTime;
	mCharacterPos.x += mMovementDir.x * 300.0f * deltaTime;

	if (mCharacterPos.y < (100.0f / 2.0f))
	{
		mCharacterPos.y = 100.0f / 2.0f;
	}
	else if (mCharacterPos.y > (768.0f - 100.0f / 2.0f))
	{
		mCharacterPos.y = 768.0f - 100.0f / 2.0f;
	}
	if (mCharacterPos.x < (100.0f / 2.0f))
	{
		mCharacterPos.x = 100.0f / 2.0f;
	}
	else if (mCharacterPos.x > (1024.0f - 100.0f / 2.0f))
	{
		mCharacterPos.x = 1024.0f - 100.0f / 2.0f;
	}
}

void Game::GenerateOutput()
{
	// Set color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	// Clear the back buffer
	SDL_RenderClear(mRenderer);

	// Set the character's color
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw the character
	SDL_Rect character{
		static_cast<int>(mCharacterPos.x - 50),
		static_cast<int>(mCharacterPos.y - 50),
		100,
		100,
	};
	SDL_RenderFillRect(mRenderer, &character);

	// Swap the front and back buffers
	SDL_RenderPresent(mRenderer);
}