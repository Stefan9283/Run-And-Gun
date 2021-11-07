#pragma once
#include <glm/glm.hpp>

class AABB;
class Circle;

class Collider {
public:
    virtual ~Collider() = default;
	virtual void resize(glm::vec2 scale) = 0;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Collider* c);
	virtual bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) = 0;
	virtual bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) = 0;
};

class Circle : public Collider {
public:
	float radius = 1.f;
	Circle() = default;
	Circle(float radius);
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) override;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) override;
	void resize(glm::vec2 scale) override;
};

class AABB : public Collider {
public:
	float w, h;
	AABB() = default;
	AABB(float w, float h);
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) override;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) override;
	void resize(glm::vec2 scale) override;
};
