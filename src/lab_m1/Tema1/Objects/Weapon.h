#pragma once
#include <vector>
#include <chrono>
#include <glm/glm.hpp>

class NPC;
class Projectile;
class Game;

class Weapon {
public:
	NPC* parent;
	float velocity, cooldown;
	std::chrono::time_point<std::chrono::system_clock> lastshot;
	glm::vec3 bulletColor;

	Weapon();
	virtual ~Weapon() {};
	NPC* getOwner();
	void setColor(glm::vec3 color);
	void setParent(NPC* parent);
	bool canShoot();
	virtual std::vector<Projectile*> shoot(Game* game) = 0;
};

class Shotgun : public Weapon {
public:
	int fragmentsCount = 5;
	void setFramentsCount(int frags);
	std::vector<Projectile*> shoot(Game* game) override;
};

class Pistol : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};
