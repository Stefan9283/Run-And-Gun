#pragma once
#include <glm/glm.hpp>

class AABB;
class Circle;

class Collider {
public:
	virtual void resize(glm::vec2 scale) = 0;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Collider* c);
	virtual bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) = 0;
	virtual bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) = 0;
};

