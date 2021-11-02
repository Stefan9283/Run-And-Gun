#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>
#include <core/gpu/shader.h>
#include <components/camera.h>
#include <lab_m1/Tema1/Colliders/Collider.h>

class Entity {
private:
    glm::vec2 pos{}, dir{ 0, -1}, scale{ 50, 50 };
    float velocity = 1.f;
    std::vector<std::pair<Mesh*, float>> meshes;
    Collider* collider = nullptr;
    glm::vec3 color = glm::vec3(1);
public:
    Entity() = default;
    void setColor(glm::vec3 color);
    void setVelocity(float velocity);
    void setDirection(glm::vec2 direction);
    void setSize(glm::vec2 scale);
    void setPosition(glm::vec2 pos);
    glm::vec2 getPosition();
    void setCollider(Collider* col);
    Collider* getCollider();
    void addDisplacement(glm::vec2 disp);
    void addMesh(Mesh* mesh, float layer);
    void Render(Shader* s, gfxc::Camera* camera);
    bool checkCollision(Entity* e);
};

