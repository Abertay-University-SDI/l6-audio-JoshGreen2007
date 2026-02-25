#include "Animation.h"

// Constructor. Sets initial values.
Animation::Animation()
{
	m_currentFrame = 0;
	m_elapsedTime = 0.f;
	m_isPlaying = true;
	m_isLooping = true;
	m_animationSpeed = 0.0f;
	m_isFlipped = false;
}

// Adds animation frame. Rect object represent a single sprite frame
void Animation::addFrame(sf::IntRect rect)
{
	m_frames.push_back(rect);
}

// Returns the size of the animation. The number of frames.
int Animation::getSize()
{
	return static_cast<int>(m_frames.size());
}

// Get current frame from the animation. Flip the returning frame if required.
sf::IntRect Animation::getCurrentFrame()
{
	if (m_frames.empty()) return sf::IntRect();

	m_frame = m_frames[m_currentFrame];

	if (m_isFlipped)
	{
		// We negate the width to flip the sprite horizontally
		m_frame = sf::IntRect(
			{ m_frame.position.x + m_frame.size.x, m_frame.position.y },
			{ -m_frame.size.x, m_frame.size.y }
		);
	}

	return m_frame;
}

// Check if the animation should progress, if so move to next frame, or loop back to the start
void Animation::animate(float dt)
{
	if (!m_isPlaying) return;	// nothing playing, let's leave.
	
	m_elapsedTime += dt; 
	
	if (m_elapsedTime < m_animationSpeed) return;	// not enough time has passed to change frame.

	m_currentFrame++;
	
	if (m_currentFrame >= static_cast<int>(m_frames.size()))
	{
		if (m_isLooping)
		{
			m_currentFrame = 0;
		}
		else
		{
			m_currentFrame = static_cast<int>(m_frames.size()) - 1;
			setPlaying(false);
		}
	}
	m_elapsedTime = 0;
	
}

// Reset animation.
void Animation::reset()
{
	m_currentFrame = 0;
	m_elapsedTime = 0.f;
}

void Animation::stop()
{
	setPlaying(false);
	m_currentFrame = 0;
	m_elapsedTime = 0.f;
}

// Set animation speed, in Frames per Second.
void Animation::setFrameSpeed(float speed)
{
	m_animationSpeed = speed;
}