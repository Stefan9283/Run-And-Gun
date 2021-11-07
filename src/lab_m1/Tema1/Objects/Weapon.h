#pragma once
#include <vector>
#include <chrono>

class NPC;
class Projectile;
class Game;

class Weapon {
public:
	NPC* parent;
	float velocity, cooldown;
	std::chrono::time_point<std::chrono::system_clock> lastshot;
	Weapon();
	void setParent(NPC* parent);
	bool canShoot();
	virtual std::vector<Projectile*> shoot(Game* game) = 0;
};

class Fists : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};

class Shotgun : public Weapon {
public:
	int fragmentsCount = 5;
	std::vector<Projectile*> shoot(Game* game) override;
};

class Pistol : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};
