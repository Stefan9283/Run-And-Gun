#pragma once

class AABB;

class Circle : public Collider {
public:
	float radius;
	Circle() = default;
	Circle(float radius);
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, AABB* c) override;
	bool checkCollision(glm::vec2 pos1, glm::vec2 pos2, Circle* c) override;
	void resize(glm::vec2 scale) override;
};

