#include "Weapon.h"
#include "lab_m1/Tema1/Colliders/Collider.h"
#include "lab_m1/Tema1/Game.h"
#include <iostream>

std::vector<Projectile*> Pistol::shoot(Game* game) {
	if (!canShoot()) return {};
	auto bullet = new Projectile(this, 50);
	bullet->setPosition(parent->getPosition());
	bullet->setDirection(parent->getDirection());
	bullet->addMesh(game->meshes["bullet"], glm::vec3(0.184f), {}, glm::vec2(0.4f));
	bullet->addMesh(game->meshes["bullet"], glm::vec3(bulletColor), {}, glm::vec2(0.3f));
	bullet->setCollider(new Circle(0.5f));
	bullet->setVelocity(bullet->getVelocity() * 4.f);
	bullet->setDamage(5);
	lastshot = std::chrono::system_clock::now();
	return { bullet };
}

void Shotgun::setFramentsCount(int frags) {
	fragmentsCount = frags;
}
std::vector<Projectile*> Shotgun::shoot(Game* game) {
	if (!canShoot()) return {};
	std::vector<Projectile*> proj;
	for (int i = 0; i < fragmentsCount; i++) {
		auto parDir = parent->getDirection() * parent->getSize();
		auto dir = glm::rotate(glm::mat4(1), glm::radians(-45 + i * 90.f / fragmentsCount), glm::vec3(0, 0, 1)) * glm::vec4(parDir, 0, 1);

		auto bullet = new Projectile(this, 20);
		bullet->setPosition(parent->getPosition() + parent->getDirection() * parent->getSize());
		bullet->setDirection(dir);
		bullet->addMesh(game->meshes["bullet"], glm::vec3(0.184f), {}, glm::vec2(0.9f));
		bullet->addMesh(game->meshes["bullet"], glm::vec3(bulletColor), {}, glm::vec2(0.7f));
		bullet->setCollider(new Circle);
		bullet->setSize({ 18, 18 });
		bullet->setVelocity(bullet->getVelocity() * 4.f);
		proj.push_back(bullet);
	}
	
	lastshot = std::chrono::system_clock::now();
	return proj;
}

Weapon::Weapon() {
	lastshot = std::chrono::system_clock::now();
	cooldown = 500;
	bulletColor = glm::vec3(1);
}
NPC* Weapon::getOwner() {
	return parent;
}
void Weapon::setColor(glm::vec3 color) {
	this->bulletColor = color;
}
void Weapon::setParent(NPC* parent) {
	this->parent = parent;
}
bool Weapon::canShoot() {
	auto now = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastshot);
	return diff.count() > cooldown;
}
