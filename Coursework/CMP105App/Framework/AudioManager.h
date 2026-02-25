// Audio Manager Class
// Manages storage and playback of multiple sound and music objects.
// Maintains lists of Sound and Music for playback in game
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once

#include "SoundObject.h"
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory> // Required for std::unique_ptr
#include <iostream> // errors

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	// Loads a sound from a file and associates it with a tag for later use.
	void addSound(const std::string& tag, const std::string& filename);

	// Plays a sound based on the tag it was loaded with.
	void playSoundbyName(const std::string& tag);

	// Stops all currently playing sounds.
	void stopAllSounds();

	// Returns a pointer to a sound for direct manipulation (e.g., setting volume, looping).
	// Returns nullptr if the tag is not found.
	sf::Sound* getSound(const std::string& tag);

	// Loads a music file for streaming and associates it with a tag.
	void addMusic(const std::string& tag, const std::string& filename);

	// Plays a music stream based on its tag.
	void playMusicbyName(const std::string& tag);

	// Stops all currently playing music streams.
	void stopAllMusic();

	// Returns a pointer to a music stream for direct manipulation.
	// Returns nullptr if the tag is not found.
	sf::Music* getMusic(const std::string& tag);

private:
	// Using std::map for efficient, key-based lookup of sounds.
	// The key is the tag (e.g., "player_shoot"), the value is the SoundObject.
	// Maps are very good, here are the docs: https://cplusplus.com/reference/map/map/
	std::map<std::string, SoundObject> m_sounds;

	// Using std::map for music streams.
	// sf::Music is non-copyable, so we store it in a std::unique_ptr.
	// This manages the memory for us automatically.
	std::map<std::string, std::unique_ptr<sf::Music>> m_musicStreams;
};