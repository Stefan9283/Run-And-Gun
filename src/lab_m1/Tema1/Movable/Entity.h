#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>
#include <core/gpu/shader.h>
#include <components/camera.h>
#include <lab_m1/Tema1/Colliders/Collider.h>
#include <lab_m1/Tema1/Movable/Weapon.h>

class Entity {
private:
    glm::vec2 pos{}, dir{ 0, -1}, scale{ 50, 50 };
    std::vector<std::pair<Mesh*, float>> meshes;
    float velocity = 10.f;
    Collider* collider = nullptr;
    glm::vec3 color = glm::vec3(1);
public:
    Entity(glm::vec2 pos = {});
    void setColor(glm::vec3 color);

    void setSize(glm::vec2 scale);
    void setPosition(glm::vec2 pos);
    glm::vec2 getPosition();
    
    void addToPosition(glm::vec2 displacement);
    void goForward(float dt);
    
    void setDirection(glm::vec2 direction);
    glm::vec2 getDirection();
    
    void setCollider(Collider* col);
    Collider* getCollider();
    bool checkCollision(Entity* e);

    void addMesh(Mesh* mesh, float layer);
    void Render(Shader* s, gfxc::Camera* camera);
};

class Projectile : public Entity {
    Weapon* source;
    float remainingTime;
public:
    Projectile(Weapon* parent, float remainingTime);
};

class NPC : public Entity {
public:
    Weapon* weapon;
    float health = 10, max_health = 10;
    NPC(glm::vec2 pos);
    
    virtual void onCollision(Projectile* p) {};
    virtual void onCollision(Entity* e) {};
    std::vector<Projectile*> shoot(Game* game);
    void addWeapon(Weapon* weapon);
};


