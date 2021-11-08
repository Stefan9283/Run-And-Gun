#include "Entity.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

#pragma region Entity
glm::vec2 Entity::getPosition() {
    return pos;
}
void Entity::addToPosition(glm::vec2 displacement) {
    this->pos += displacement;
}
void Entity::setPosition(glm::vec2 pos) {
    this->pos = pos;
}
void Entity::setDirection(glm::vec2 direction = glm::vec2(-1, 0)) {
    this->dir = glm::normalize(direction);
}
glm::vec2 Entity::getDirection() {
    return dir;
}
void Entity::setVelocity(float velocity) {
    this->velocity = velocity;
}
float Entity::getVelocity() {
    return velocity;
}
void Entity::setSize(glm::vec2 scale) {
    this->scale = scale;
    if (collider) collider->resize(scale);
}
glm::vec2 Entity::getSize() {
    return scale;
}
Entity::Entity(glm::vec2 pos) {
    this->pos = pos;
}
Entity::~Entity() {
    delete collider;
}
void Entity::setCollider(Collider* col) {
    delete collider;
    collider = col;
    col->resize(scale);
}
void Entity::goForward(float dt) {
    pos += velocity * dir * dt;
}
void Entity::avoidEntity(float dt, Entity* e) {
    auto oldDirection = dir;
    auto newDir = pos - e->getPosition();
    setDirection(newDir);
    goForward(dt);
    setDirection(dir);
}
Collider* Entity::getCollider() {
    return collider;
}
void Entity::addMesh(Mesh* mesh, glm::vec3 color, glm::vec2 offset, glm::vec2 scale) {
    meshes.push_back({ scale, color, mesh, offset });
}
void Entity::Render(Shader* s, gfxc::Camera* camera) {
    s->Use();
    
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(pos, 1));
    glm::mat4 R = glm::rotate(glm::mat4(1), glm::orientedAngle({ 0, -1 }, dir), glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(scale, 1));

    glUniformMatrix4fv(s->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(s->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(s->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(T * R * S));

    for (auto &mesh : meshes) {
        glUniform3f(s->loc_color, mesh.color.x, mesh.color.y, mesh.color.z);
        glUniform2f(s->loc_offset, mesh.offset.x, mesh.offset.y);
        glUniform2f(s->loc_scale, mesh.scale.x, mesh.scale.y);
        mesh.mesh->Render();
    }
}
bool Entity::checkCollision(Entity* e) {
    if (!collider) return false;
    return collider->checkCollision(pos, e->getPosition(), e->getCollider());
}
#pragma endregion

#pragma region NPC
NPC::NPC(glm::vec2 pos) {
    setPosition(pos);
    weapon = nullptr;
}
NPC::~NPC() {
    delete weapon;
}
void NPC::addWeapon(Weapon* weapon) {
    delete this->weapon;
    this->weapon = weapon;
    weapon->setParent(this);
}
int NPC::getHealth() {
    return health;
}
int NPC::getMaxHealth() {
    return max_health;
}
void NPC::addHealth(int points) {
    health = glm::clamp(health + points, health, max_health);
}
std::vector<Projectile*> NPC::shoot(Game* game) {
    if (health != 0)
        return weapon->shoot(game);
    return {};
}
#pragma endregion

#pragma region Projectile
Projectile::Projectile(Weapon* parent, float remainingTime) {
    this->remainingTime = remainingTime;
    source = parent;
}
bool Projectile::isSource(NPC* e) {
    return e->weapon == source;
}
float Projectile::getRemainingTime() {
    return remainingTime;
}
void Projectile::decrementRemainingTime(float dt) {
    remainingTime -= dt;
}
void Projectile::setDamage(float damage) {
    this->damage = damage;
}
int Projectile::getDamage() {
    return damage;
}
#pragma endregion

#pragma region Enemy
void Enemy::onCollision(Projectile* p) {
    if (!p->isSource(this))
        health -= p->getDamage();
}
void Enemy::onCollision(NPC* e) {
    if (dynamic_cast<Player*>(e)) {
        health = 0;
    }
};
#pragma endregion

void Player::onCollision(NPC* e) {
    if (dynamic_cast<Kamikaze*>(e)) {
        health = glm::max(health - 1, 0.f);
    }
}

void Player::onCollision(Projectile* p) {
    // TODO
}

void Player::pickUp(PickUp* p) {
    p->getPickedUp(this);
}

PickUp::~PickUp() {
    if (type > 0) {
        switch (type) {
            case SHOTGUN_PICKUP:
                delete (Shotgun*)item;
                break;
            case GUN_PICKUP:
                delete (Pistol*)item;
                break;
        }
    }
}

void PickUp::getPickedUp(Player* p) {
    switch (type) {
        case HEALTH_PICKUP:
            p->addHealth(value);
            break;
        case SHOTGUN_PICKUP:
            p->addWeapon((Weapon*)item);
            break;
        case GUN_PICKUP:
            p->addWeapon((Weapon*)item);
            break;
        case COOLDOWN_PICKUP:
            p->weapon->cooldown = value;
            break;
        case SPEED_PICKUP:
            p->setVelocity(p->getVelocity() * value);
            break;
    }
    type = - type;
}
