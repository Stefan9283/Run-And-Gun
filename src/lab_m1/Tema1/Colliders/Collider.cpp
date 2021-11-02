#include "Collider.h"
#include "AABB.h"
#include "Circle.h"

bool Collider::checkCollision(glm::vec2 pos1, glm::vec2 pos2, Collider* c) {
	if (dynamic_cast<AABB*>(c))
		return this->checkCollision(pos1, pos2, (AABB*)c);
	else if (dynamic_cast<Circle*>(c))
		return this->checkCollision(pos1, pos2, (Circle*)c);
	return false;
}
