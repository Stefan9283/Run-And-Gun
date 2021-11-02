#include "Collider.h"
#include "Circle.h"
#include "AABB.h"
#include <iostream>

Circle::Circle(float radius) {
	this->radius = radius;
}

bool Circle::checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) {
	glm::vec2 min, max, closestPoint;
	min = pos2 - glm::vec2(c->w, c->h) / 2.f;
	max = pos2 + glm::vec2(c->w, c->h) / 2.f;
	closestPoint = glm::max(min, glm::min(pos1, max));
	return glm::distance(closestPoint, pos1) < radius;
}
bool Circle::checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) {
	return glm::distance(pos1, pos2) < c->radius + radius;
}

void Circle::resize(glm::vec2 scale) {
	radius = glm::max(scale.x, scale.y);
}
