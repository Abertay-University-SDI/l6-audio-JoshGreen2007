// Animation Class
// Controls a single animation
// Stores a list of frames, animation speed, looping and lipped state.
// Returns current frame of animation based on time and updating.
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once
// Disable the warning for the SFML headers
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include <vector>

class Animation
{
public:
	
	Animation();

	// Create animation
	void addFrame(sf::IntRect rect);

	// Set the delay between frames in seconds (e.g. 0.1f)
	void setFrameSpeed(float speed);
	float getFrameSpeed() { return m_animationSpeed; };

	int getSize();

	// update/reset animation
	void animate(float dt);
	void reset();

	// get current frame of animation
	sf::IntRect getCurrentFrame();

	// Animation controls
	void setFlipped(bool flip) { m_isFlipped = flip; };
	bool getFlipped() { return m_isFlipped; };

	void setLooping(bool loop) { m_isLooping = loop; };
	bool getLooping() { return m_isLooping; };

	void setPlaying(bool play) { m_isPlaying = play; };
	bool getPlaying() { return m_isPlaying; };

	void stop();
	void pause() { m_isPlaying = false; };

protected:
	std::vector<sf::IntRect> m_frames;
	sf::IntRect m_frame;
	int m_currentFrame;
	float m_animationSpeed;
	bool m_isFlipped;
	bool m_isLooping;
	bool m_isPlaying;
	float m_elapsedTime;
};