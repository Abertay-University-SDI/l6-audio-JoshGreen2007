#include "BaseLevel.h"

// Use a member initialiser list to initialise the references
BaseLevel::BaseLevel(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: m_window(window), m_input(input), m_gameState(gameState), m_audio(audio)
{
	// Constructor body can be empty if the initialiser list does everything
}

// Begins rendering to the back buffer. Background colour set to light blue.
void BaseLevel::beginDraw()
{
	m_window.clear(sf::Color(100, 149, 237));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void BaseLevel::endDraw()
{
	m_window.display();
}