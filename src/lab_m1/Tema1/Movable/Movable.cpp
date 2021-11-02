#include "Movable.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>


glm::vec2 Entity::getPosition() {
    return pos;
}
void Entity::setPosition(glm::vec2 pos) {
    this->pos = pos;
}
void Entity::setDirection(glm::vec2 direction = glm::vec2(-1, 0)) {
    this->dir = glm::normalize(direction);
}
void Entity::setSize(glm::vec2 scale) {
    this->scale = scale;
    if (collider) collider->resize(scale);
}
void Entity::setColor(glm::vec3 color) {
    this->color = color;
}
void Entity::setVelocity(float velocity) {
    this->velocity = velocity;
}
void Entity::setCollider(Collider* col) {
    delete collider;
    collider = col;
    col->resize(scale);
}
Collider* Entity::getCollider() {
    return collider;
}
void Entity::addDisplacement(glm::vec2 disp) {
    pos += disp * velocity;
}

void Entity::addMesh(Mesh* mesh, float layer) {
    meshes.push_back({ mesh, layer });
    std::sort(meshes.begin(), meshes.end(), [](std::pair<Mesh*, float>& a, std::pair<Mesh*, float>& b) {
        return a.second < b.second;
    });
}

void Entity::Render(Shader* s, gfxc::Camera* camera) {
    s->Use();
    
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(pos, 1));
    glm::mat4 R = glm::rotate(glm::mat4(1), glm::orientedAngle({ 0, -1 }, dir), glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(scale, 1));

    glUniformMatrix4fv(s->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(s->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(s->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(T * R * S));
    glUniform3f(s->loc_color, color.x, color.y, color.z);

    for (auto mesh : meshes) {
        glUniform1f(s->loc_layer, mesh.second);
        mesh.first->Render();
    }
}

bool Entity::checkCollision(Entity* e) {
    if (!collider) return false;
    collider->checkCollision(pos, e->getPosition(), e->getCollider());
}
