#include "Game.hpp"
#include <particle.hpp>
#include <groundcontact.hpp>

const float Game::WINDOW_WIDTH = 1024.0f;
const float Game::WINDOW_HEIGHT = 728.0f;

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), maxContacts(10) {}

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
	mWindow = SDL_CreateWindow("Game Engine", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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

	resolver = new ParticleContactResolver(5);

	mCharacter = new Particle();

	float x = WINDOW_WIDTH / 2.0f;
	float y = WINDOW_HEIGHT / 2.0f;
	float z = 0.0f;

	mCharacter->setPosition(x, y, z);
	mCharacter->setMass(1.0);
	mCharacter->setAcceleration(Vector3::GRAVITY);
	mCharacter->setDamping(0.99);

	particles.push_back(mCharacter);

	GroundContact* groundContactGenerator = new GroundContact();
	groundContactGenerator->init(particles);
	contactGenerators.push_back(groundContactGenerator);

	contacts = new ParticleContact[maxContacts];

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
}

unsigned Game::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;

	for (std::vector<ParticleContactGenerator*>::iterator g = contactGenerators.begin(); g != contactGenerators.end(); g++)
	{
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		// Maximum number of collisions reached
		if (limit <= 0) break;
	}

	// Return the current number of collisions
	return maxContacts - limit;
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

	mCharacter->integrate(deltaTime);

	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		resolver->setIterations(usedContacts * 2);
		resolver->resolveContacts(contacts, usedContacts, deltaTime);
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
	Vector3 currentPosition = mCharacter->getPosition();

	SDL_Rect character = {
		static_cast<int>(WINDOW_WIDTH - (currentPosition.x + 50)),
		static_cast<int>(WINDOW_HEIGHT - (currentPosition.y + 50)),
		100,
		100,
	};
	SDL_RenderFillRect(mRenderer, &character);

	// Swap the front and back buffers
	SDL_RenderPresent(mRenderer);
}
