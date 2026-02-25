#include "GameObject.h"

GameObject::GameObject()
{
	m_input = nullptr;
	m_window = nullptr;
	m_alive = true;
	m_collider = false;
	m_velocity = { 0.f, 0.f }; // Default velocity
}


// Override this function to provide input handling for the derived class
void GameObject::handleInput(float dt)
{
}

// override this function to provide functionality
void GameObject::update(float dt)
{
}

// Sets the velocity of the sprite
void GameObject::setVelocity(sf::Vector2f vel)
{
	m_velocity = vel;
}
void GameObject::setVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
}

// get sprite velocity
sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

// Returns collision box + position, giving the collision box in the correct position
sf::FloatRect GameObject::getCollisionBox() const{
	
	return sf::FloatRect(
		{ 
			m_collisionBox.position.x + getPosition().x - getOrigin().x, 
			m_collisionBox.position.y + getPosition().y - getOrigin().y 
		}, 
		{
			m_collisionBox.size.x, 
			m_collisionBox.size.y
		});
}

void GameObject::setCollisionBox(float x, float y, float width, float height)
{
	m_collisionBox = sf::FloatRect({ x, y }, { width, height });
}

void GameObject::setCollisionBox(sf::FloatRect fr)
{
	m_collisionBox = fr;
}

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void GameObject::collisionResponse(GameObject& collider)
{
}
