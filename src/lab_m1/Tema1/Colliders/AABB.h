#pragma once

class Circle;

class AABB : public Collider {
public:
	float w, h;
	AABB() = default;
	AABB(float w, float h);
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) override;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) override;
	void resize(glm::vec2 scale) override;
};

