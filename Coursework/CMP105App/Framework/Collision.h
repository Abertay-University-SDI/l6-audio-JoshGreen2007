// Collision Class
// Provides default functions for testing collision detection. Functions for standard axis aligned bounding box, and bounding circle.
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once
#include "GameObject.h"

// Static class provide collision detection functions.
class Collision
{
public:
	
	// Check AABB for collision. Returns true if collision occurs.
	static bool checkBoundingBox(const GameObject& s1, const GameObject& s2);
	
	// check for collision between gameobject and point. Returns true if collision occurs.
	static bool checkBoundingBox(const GameObject& s1, const sf::Vector2i& point);

	// Check bounding circle collision. Returns true if collision occurs.
	static bool checkBoundingCircle(const GameObject& s1, const GameObject& s2);

};