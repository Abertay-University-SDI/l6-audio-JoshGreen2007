// SoundObject
// Loads a sound file and provides access to it for playback.
// @author Paul Robertson 
// @reviser William Kavanagh (2025)

#pragma once
// Disable the warning for the SFML headers
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#include <SFML/Audio.hpp>
#pragma warning(pop)
#include <string>

class SoundObject
{
public:
	// Constructor
	SoundObject();

	// Destructor
	~SoundObject();

	// Loads a sound from a file.
	// Returns true on success, false on failure.
	bool loadSound(const std::string& filename);

	// Returns a reference to the sound for playback and manipulation.
	sf::Sound& getSound();

	// Returns a constant reference to the sound.
	const sf::Sound& getSound() const;

private:
	// The buffer holds the raw audio data in memory.
	sf::SoundBuffer m_buffer;

	// The sound is a playable instance that uses the buffer's data.
	sf::Sound m_sound;

	// A description string name for the sound object for management

};