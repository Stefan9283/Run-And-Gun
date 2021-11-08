#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>
#include <core/gpu/shader.h>
#include <components/camera.h>
#include <lab_m1/Tema1/Colliders/Collider.h>
#include <lab_m1/Tema1/Objects/Weapon.h>


struct SubMesh {
    glm::vec2 scale;
    glm::vec3 color;
    Mesh* mesh;
    glm::vec2 offset;
};

class Entity {
private:
    glm::vec2 pos{}, dir{ 0, -1}, scale{ 50, 50 };
    std::vector<SubMesh> meshes;
    float velocity = 300.f;
    Collider* collider = nullptr;
public:
    Entity(glm::vec2 pos = {});
    ~Entity();

    void setSize(glm::vec2 scale);
    glm::vec2 getSize();

    void setPosition(glm::vec2 pos);
    glm::vec2 getPosition();
    
    void addToPosition(glm::vec2 displacement);
    void goForward(float dt);
    void avoidEntity(float dt, Entity* e);

    void setDirection(glm::vec2 direction);
    glm::vec2 getDirection();

    void setVelocity(float velocity);
    float getVelocity();
    
    void setCollider(Collider* col);
    Collider* getCollider();
    bool checkCollision(Entity* e);

    void addMesh(Mesh* mesh, glm::vec3 color, glm::vec2 offset = {}, glm::vec2 scale = { 1.f, 1.f });
    void Render(Shader* s, gfxc::Camera* camera);
};

class Projectile : public Entity {
    Weapon* source;
    float remainingTime;
    float damage = 1;
public:
    Projectile(Weapon* parent, float remainingTime);
    bool isSource(NPC* e);
    float getRemainingTime();
    void decrementRemainingTime(float dt);
    void setDamage(float damage);
    int getDamage();
};

class NPC : public Entity {
public:
    Weapon* weapon;
    float health = 10, max_health = 10;
    NPC(glm::vec2 pos);
    ~NPC();
    
    virtual void onCollision(Projectile* p) {};
    virtual void onCollision(Entity* e) {};
    virtual void onCollision(NPC* e) {};
    int getHealth();
    int getMaxHealth();
    void addHealth(int points);
    std::vector<Projectile*> shoot(Game* game);
    void addWeapon(Weapon* weapon);
};


class Enemy : public NPC {
public:
    Enemy(glm::vec2 pos) : NPC(pos) {};
    void onCollision(Projectile* p) override;
    void onCollision(NPC* e) override;
};

class Kamikaze : public Enemy {
public:
    Kamikaze(glm::vec2 pos) : Enemy(pos) {};
};

class PickUp;

class Player : public NPC {
public:
    Player(glm::vec2 pos) : NPC(pos) {};
    void onCollision(NPC* e) override;
    void onCollision(Projectile* p) override;
    void pickUp(PickUp* p);
};


#define SHOTGUN_PICKUP 0
#define GUN_PICKUP 1
#define HEALTH_PICKUP 2
#define COOLDOWN_PICKUP 3
#define SPEED_PICKUP 4

class PickUp : public Entity {
public:
    void* item = nullptr;
    int value = 0;
    int type = 0;
    
    PickUp(glm::vec2 pos, int type) : Entity(pos) {
        this->type = type;
        switch (type) {
            case HEALTH_PICKUP:
                value = rand() % 3 * 3;
                break;
            case SHOTGUN_PICKUP:
                item = new Shotgun;
                break;
            case GUN_PICKUP:
                item = new Pistol;
                break;
            case COOLDOWN_PICKUP:
                value = (1 + rand() % 4) * 100;
                break;
            case SPEED_PICKUP:
                value = 1 / (1 + rand() % 10) + 1;
                break;
        } 
    }
    ~PickUp();
    void getPickedUp(Player* p);
};
