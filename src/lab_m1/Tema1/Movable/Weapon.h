#pragma once
#include <vector>

class NPC;
class Projectile;
class Game;

class Weapon {
public:
	NPC* parent;
	float velocity, cooldown;
	Weapon() = default;
	void setParent(NPC* parent);
	virtual std::vector<Projectile*> shoot(Game* game) = 0;
};

class Fists : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};

class Pistol : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};

class Shotgun : public Weapon {
public:
	std::vector<Projectile*> shoot(Game* game) override;
};
