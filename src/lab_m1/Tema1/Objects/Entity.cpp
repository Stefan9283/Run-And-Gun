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
Collider* Entity::getCollider() {
    return collider;
}
void Entity::addMesh(Mesh* mesh, glm::vec3 color, glm::vec2 offset, glm::vec2 scale) {
    meshes.push_back({ scale, color, mesh, offset });
}
void Entity::Render(Shader* s, gfxc::Camera* camera, float zoom) {
    s->Use();
    
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(pos, 1));
    glm::mat4 R = glm::rotate(glm::mat4(1), glm::orientedAngle({ 0, -1 }, dir), glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(scale, 1));

    glm::mat4 view = glm::scale(glm::mat4(1), glm::vec3(zoom, zoom, 1)) * camera->GetViewMatrix();
    glUniformMatrix4fv(s->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
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
NPC::NPC(glm::vec2 pos, int type) {
    setPosition(pos);
    this->type = type;
    weapon = nullptr;
}
NPC::~NPC() {
    delete weapon;
}
void NPC::setWeapon(Weapon* weapon) {
    delete this->weapon;
    this->weapon = weapon;
    weapon->setParent(this);
}
Weapon* NPC::getWeapon() {
    return weapon;
}
float NPC::getHealth() {
    return health;
}
float NPC::getMaxHealth() {
    return max_health;
}
void NPC::addHealth(float points) {
    health = glm::clamp(health + points, 0.f, max_health);
}
int NPC::getType() {
    return type;
}
std::vector<Projectile*> NPC::shoot(Game* game) {
    if (health != 0 && weapon)
        return weapon->shoot(game);
    return {};
}
#pragma endregion

#pragma region Projectile
Projectile::Projectile(Weapon* weapon, float remainingTime) {
    this->remainingTime = remainingTime;
    source = weapon;
    source_npc = weapon->getOwner()->getType();

}
bool Projectile::getSourceType() {
    return source_npc;
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
float Projectile::getDamage() {
    return damage;
}
#pragma endregion

#pragma region Enemy
void Enemy::onCollision(Projectile* p) {
    if (!p->getSourceType())
        addHealth(-p->getDamage());
}
void Enemy::onCollision(NPC* e) {
    if (dynamic_cast<Player*>(e)) {
        addHealth(- getHealth());
    }
};
void Enemy::avoidEntity(float dt, Entity* e, std::vector<Entity*> barriers) {
    auto oldDirection = getDirection();
    auto oldPos = getPosition();
    auto newDir = oldPos - e->getPosition();

    setDirection(newDir);
    auto oldVelocity = getVelocity();
    setVelocity(oldVelocity * 2.5f);
    goForward(dt);
    setVelocity(oldVelocity);
    setDirection(oldDirection);

    bool collision = false;
    for (auto b : barriers)
        if (b->checkCollision(this)) {
            collision = true;
            break;
        }
    if (collision)
        setPosition(oldPos);
}
#pragma endregion

#pragma region Player
void Player::onCollision(NPC* e) {
    switch (e->getType())
    {
    case KAMIKAZE_TYPE:
        addHealth(-3);
    case GUNNER_TYPE:
        addHealth(-1);
    default:
        break;
    }
}
void Player::onCollision(Projectile* p) {
    if (p->getSourceType() != PLAYER_TYPE)
        addHealth(- p->getDamage());
}
void Player::pickUp(PickUp* p) {
    p->getPickedUp(this);
}
#pragma endregion

#pragma region PickUp
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
            ((Weapon*)item)->setColor({ 0.725, 0.086, 0.274 });
            p->setWeapon((Weapon*)item);
            break;
        case GUN_PICKUP:
            ((Weapon*)item)->setColor({ 0.949, 0.941, 0.074 });
            p->setWeapon((Weapon*)item);
            break;
        case COOLDOWN_PICKUP:
            p->getWeapon()->cooldown = value;
            break;
        case SPEED_PICKUP:
            p->setVelocity(p->getVelocity() * value);
            break;
    }
    type = - type;
}
#pragma endregion
