#include "Weapon.h"
#include "lab_m1/Tema1/Game.h"
#include <iostream>

std::vector<Projectile*> Fists::shoot(Game* game) {
	for (auto en : game->enemies) {
		if (parent->checkCollision(en)) {
			float angle = glm::dot(en->getPosition() - parent->getPosition(), parent->getDirection());
			if (angle > 75.f) {
				en->health -= 1;
			}
			break;
		}
	}
	return {};
}

std::vector<Projectile*> Pistol::shoot(Game* game) {
	return {};
}

std::vector<Projectile*> Shotgun::shoot(Game* game) {
	return {};
}

void Weapon::setParent(NPC* parent) {
	this->parent = parent;
}
