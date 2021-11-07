#include "Collider.h"

bool Collider::checkCollision(glm::vec2 pos1, glm::vec2 pos2, Collider* c) {
	if (dynamic_cast<AABB*>(c))
		return this->checkCollision(pos1, pos2, (AABB*)c);
	else if (dynamic_cast<Circle*>(c))
		return this->checkCollision(pos1, pos2, (Circle*)c);
	return false;
}

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


AABB::AABB(float w, float h) {
    this->w = w;
    this->h = h;
}

bool AABB::checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) {
    glm::vec2 min1, min2, max1, max2;
    min1 = pos1 - glm::vec2(w, h) / 2.f;
    max1 = pos1 + glm::vec2(w, h) / 2.f;
    min2 = pos2 - glm::vec2(c->w, c->h) / 2.f;
    max2 = pos2 + glm::vec2(c->w, c->h) / 2.f;

    return
        min1.x < max2.x&&
        max1.x > min2.x &&
        min1.y < max2.y&&
        max1.y > min2.y;
}
bool AABB::checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) {
    return c->checkCollision(pos2, pos1, this);
}

void AABB::resize(glm::vec2 scale) {
    scale *= 2.f;
    w = scale.x;
    h = scale.y;
}
