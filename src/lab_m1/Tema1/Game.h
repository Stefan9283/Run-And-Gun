#pragma once

#include "components/simple_scene.h"
#include "Objects/Entity.h"
#include "Gui/Minimap.h"

class Game : public gfxc::SimpleScene
{
    friend class Pistol; 
    friend class Shotgun;
    friend class Minimap;
public:
    ~Game() override;
    void Init() override;

    float score;
    Player* player;
    PickUp* pickup;
    std::vector<NPC*> enemies;
    std::vector<Entity*> props;
    std::vector<Projectile*> projectiles;
    std::vector<Entity*> mapBarriers;
    Minimap* minimap;
    glm::vec2 playAreaScale;
    gfxc::Camera guiCamera;

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
    
    void renderHealthBars();
    void displayGameOverToConsole();
    void checkCollisions(float deltaTimeSeconds);
    void spawnEnemy();
    void spawnPickUp();
    void respawnPlayer();
};
