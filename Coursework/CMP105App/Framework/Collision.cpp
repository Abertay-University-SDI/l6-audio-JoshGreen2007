#include "Collision.h"

// Check AABB for collision. Returns true if collision occurs.
bool Collision::checkBoundingBox(const GameObject& s1, const GameObject& s2)
{
	return s1.getCollisionBox().findIntersection(s2.getCollisionBox()).has_value();
}

bool Collision::checkBoundingBox(const GameObject& s1, const sf::Vector2i& point)
{
	return s1.getCollisionBox().contains(static_cast<sf::Vector2f>(point));
}

// Check bounding circle collision. Returns true if collision occurs.
bool Collision::checkBoundingCircle(const GameObject& s1, const GameObject& s2)
{
	// Calculations are cleaner with references
	float radius1 = s1.getSize().x / 2;
	float radius2 = s2.getSize().x / 2;

	// Calculate center positions
	float xpos1 = s1.getPosition().x + radius1;
	float xpos2 = s2.getPosition().x + radius2;
	float ypos1 = s1.getPosition().y + radius1;
	float ypos2 = s2.getPosition().y + radius2;

	float dx = xpos2 - xpos1;
	float dy = ypos2 - ypos1;
	float distSqr = dx * dx + dy * dy;
	float radiiSumSqr = (radius1 + radius2) * (radius1 + radius2);

	return distSqr < radiiSumSqr;
}

