#pragma once

#include "components/simple_scene.h"
#include "Movable/Entity.h"

class Game : public gfxc::SimpleScene
{
public:
    Game();
    ~Game() override;

    void Init() override;

    // TODO
    NPC* player;
    std::vector<NPC*> enemies;
    std::vector<Entity*> props;
    std::vector<Projectile*> projectiles;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
};
