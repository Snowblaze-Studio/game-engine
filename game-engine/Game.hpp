#pragma once

#ifndef GAME_HPP // include guard
#define GAME_HPP

#include <vector>
#include <SDL.h>

#include <particle.hpp>
#include <rigidbody.hpp>
#include <pcontactresolver.hpp>
#include <pcontactgenerator.hpp>

class Game
{
public:
	const static float WINDOW_WIDTH;
	const static float WINDOW_HEIGHT;

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

	unsigned generateContacts();

	SDL_Window* mWindow;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	bool mIsRunning;

	std::vector<Particle*> particles;

	RigidBody* mCharacter;

	ParticleContactResolver* resolver;

	std::vector<ParticleContactGenerator*> contactGenerators;

	ParticleContact* contacts;

	unsigned maxContacts;
};

#endif
