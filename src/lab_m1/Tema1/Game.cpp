#include "Game.h"
#include <iostream>
#include "Colliders/AABB.h"
#include "Colliders/Circle.h"

Game::Game() {}
Game::~Game() {
    for (auto* prop : props)
        delete prop;
    for (auto* en : enemies)
        delete en;
}
void Game::Init() {
    glm::ivec2 halfresolution = window->GetResolution() / 2;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(-(float)halfresolution.x, (float)halfresolution.x, -(float)halfresolution.y, (float)halfresolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    {
        Mesh* mesh = new Mesh("circle");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "circle.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("player");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "player.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("enemy");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "enemy2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    auto floor = new Entity();
    floor->addMesh(meshes["quad"], 100);
    floor->setSize(glm::vec2(1000));
    floor->setColor({ 0.498, 0.509, 0.643 });
    props.emplace_back(floor);

    for (int i = 0; i < 10; i++) {
        auto prop = new Entity(glm::vec2(i * 100, 200));
        prop->addMesh(meshes["quad"], 99);
        prop->setSize(glm::vec2(100));
        prop->setColor({ 0.423, 0.4, 0.921 });
        prop->setCollider(new AABB);
        prop->setSize({ 10, 10 });
        props.emplace_back(prop);
    }

    player = new NPC({ 0, 0 });
    player->addMesh(meshes["player"], 50);
    player->setColor({ 0, 0, 1 });
    player->setCollider(new Circle);
    player->setSize(glm::vec2(50));
    player->addWeapon(new Fists);

    NPC* e = new NPC({ 100, 100 });
    e->addMesh(meshes["enemy"], 1);
    e->setCollider(new AABB);
    e->setSize(glm::vec2(50));
    enemies.emplace_back(e);

    {
        Shader* shader = new Shader("SimpleColor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "SimpleVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "SimpleFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}
    
void Game::Update(float deltaTimeSeconds) {
    for (auto en : enemies) {
        // UPDATE HEALTHBARS
        en->Render(shaders["SimpleColor"], GetSceneCamera());
        en->setDirection(player->getPosition() - en->getPosition());
        en->goForward(deltaTimeSeconds);
    }
    player->Render(shaders["SimpleColor"], GetSceneCamera());
    
    for (auto prop : props)
        prop->Render(shaders["SimpleColor"], GetSceneCamera());
    
    // RenderMesh(meshes["quad"], glm::vec3(-200, .5f, 0), glm::vec3(50.f));
    // RenderMesh(meshes["circle"], glm::vec3(1, 200.5f, 0), glm::vec3(50.f, 100, 50));
}
void Game::OnInputUpdate(float deltaTime, int mods) {
    // if WASD check collision
    {
        auto pos = player->getPosition();
        if (window->KeyHold(GLFW_KEY_W))
            player->addToPosition({ 0, 10 });
        if (window->KeyHold(GLFW_KEY_S))
            player->addToPosition({ 0, -10 });
        for (auto prop : props)
            if (prop->checkCollision(player)) {
                player->setPosition(pos);
                break;
            }
    }
    
    {
        auto pos = player->getPosition();

        if (window->KeyHold(GLFW_KEY_A))
            player->addToPosition({ -10, 0 });
        if (window->KeyHold(GLFW_KEY_D))
            player->addToPosition({ 10, 0 });
        for (auto prop : props)
            if (prop->checkCollision(player)) {
                player->setPosition(pos);
                break;
            }
    }
    
    auto cam = GetSceneCamera();
    cam->SetPosition({ player->getPosition(), 50 });
    cam->Update();

}
void Game::OnKeyPress(int key, int mods) {}
void Game::OnKeyRelease(int key, int mods) {}
void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    glm::ivec2 resolution = window->GetResolution() / 2;
    player->setDirection(glm::vec2(mouseX - resolution.x, resolution.y - mouseY));
}
void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // shoot and calculate direction
    if (button - 1 == GLFW_MOUSE_BUTTON_LEFT)
        player->shoot(this);
}
void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) { /* maybe use to change player weapon? */ }
    
void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
void Game::FrameEnd() {}
void Game::OnWindowResize(int width, int height) {
    glm::ivec2 halfresolution = window->GetResolution() / 2;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(-(float)halfresolution.x, (float)halfresolution.x, -(float)halfresolution.y, (float)halfresolution.y, 0.01f, 400);
    camera->Update();
}
