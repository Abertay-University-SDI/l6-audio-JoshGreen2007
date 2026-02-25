// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once

// Disable the warning for the SFML headers
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#include <SFML/Graphics.hpp>
#pragma warning(pop)

#include "Input.h"
#include "AudioManager.h"
#include "GameState.h"
#include <string>
#include <iostream>

class BaseLevel
{
public:

	BaseLevel(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud);
	virtual ~BaseLevel() = default;

	virtual void handleInput(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void render() = 0;

protected:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	// Dependencies stored as references as we need these but do not own them (therefore should not destroy them)
	sf::RenderWindow& m_window;
	Input& m_input;
	GameState& m_gameState;
	AudioManager& m_audio;
};

