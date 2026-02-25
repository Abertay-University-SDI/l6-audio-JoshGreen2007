#include "SoundObject.h"
#include <iostream> // Included for error reporting

// Constructor
// Uses a member initialiser list to construct m_sound.
// This is required in SFML 3.0 as sf::Sound needs a buffer upon creation.
SoundObject::SoundObject() : m_sound(m_buffer)
{
	// The constructor body can be empty because the real work
	// is done in the initialiser list above.
}

// Destructor
// No code is needed here because m_buffer and m_sound are not pointers.
// Their memory is automatically cleaned up when the SoundObject is destroyed.
SoundObject::~SoundObject()
{
}

// Loads a sound from the specified file into the sound buffer.
bool SoundObject::loadSound(const std::string& filename)
{
	if (!m_buffer.loadFromFile(filename))
	{
		// It's good practice to provide an error message if loading fails.
		std::cerr << "Error: Could not load sound file: " << filename << std::endl;
		return false;
	}

	return true;
}

// Provides writeable access to the internal sf::Sound object.
sf::Sound& SoundObject::getSound()
{
	return m_sound;
}

// Provides read-only access to the internal sf::Sound object.
// This is useful for functions that only need to read sound properties
// without changing them.
const sf::Sound& SoundObject::getSound() const
{
	return m_sound;
}
