#include "Weapon.h"
#include "lab_m1/Tema1/Colliders/Collider.h"
#include "lab_m1/Tema1/Game.h"
#include <iostream>

std::vector<Projectile*> Fists::shoot(Game* game) {
	if (!canShoot()) return {};
	for (auto en : game->enemies) {
		if (parent->checkCollision(en)) {
			float angle = glm::dot(en->getPosition() - parent->getPosition(), parent->getDirection());
			if (angle > 75.f) {
				en->health -= 1;
			}
			break;
		}
	}
	lastshot = std::chrono::system_clock::now();
	return {};
}

std::vector<Projectile*> Pistol::shoot(Game* game) {
	if (!canShoot()) return {};
	auto bullet = new Projectile(this, 50);
	bullet->setPosition(parent->getPosition());
	bullet->setDirection(parent->getDirection());
	bullet->addMesh(game->meshes["bullet"], glm::vec3(0.18431372549), {}, glm::vec2(0.4));
	bullet->addMesh(game->meshes["bullet"], glm::vec3(0.949, 0.941, 0.074), {}, glm::vec2(0.3));
	bullet->setCollider(new Circle(0.5));
	bullet->setVelocity(bullet->getVelocity() * 4.f);
	lastshot = std::chrono::system_clock::now();
	return {bullet};
}

std::vector<Projectile*> Shotgun::shoot(Game* game) {
	if (!canShoot()) return {};
	parent->getDirection();
	std::vector<Projectile*> proj;
	for (int i = 0; i < fragmentsCount; i++) {
		auto parDir = parent->getDirection() * parent->getSize();
		auto dir = glm::rotate(glm::mat4(1), glm::radians(-45 + i * 90.f / fragmentsCount), glm::vec3(0, 0, 1)) * glm::vec4(parDir, 0, 1);

		auto bullet = new Projectile(this, 20);
		bullet->setPosition(parent->getPosition() + parent->getDirection() * parent->getSize());
		bullet->setDirection(dir);
		bullet->addMesh(game->meshes["bullet"], glm::vec3(0.18431372549), {}, glm::vec2(0.3));
		bullet->addMesh(game->meshes["bullet"], glm::vec3(0.725, 0.086, 0.274), {}, glm::vec2(0.2));
		bullet->setCollider(new Circle);
		bullet->setVelocity(bullet->getVelocity() * 4.f);
		proj.push_back(bullet);
	}
	
	lastshot = std::chrono::system_clock::now();
	return proj;
}

Weapon::Weapon() {
	lastshot = std::chrono::system_clock::now();
	cooldown = 500;
}

void Weapon::setParent(NPC* parent) {
	this->parent = parent;
}

bool Weapon::canShoot() {
	auto now = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastshot);
	return diff.count() > cooldown;
}
