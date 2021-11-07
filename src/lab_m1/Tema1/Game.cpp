#include "Game.h"
#include <iostream>
#include "Colliders/Collider.h"


Game::~Game() {
    for (auto* prop : props)
        delete prop;
    for (auto* en : enemies)
        delete en;
}
void Game::Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glDepthFunc(GL_ALWAYS);

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
        Mesh* mesh = new Mesh("bullet");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "bullet.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        playAreaScale = glm::vec2(3000, 4000);


        auto floor = new Entity();
        floor->addMesh(meshes["quad"], 300, { 0.498, 0.509, 0.643 });
        floor->setSize(playAreaScale);
        props.emplace_back(floor);

        auto barrierL = new Entity({- 50 - playAreaScale.x, 0});
        barrierL->addMesh(meshes["quad"], 300, { 0.047, 0.176, 0.282 });
        barrierL->setCollider(new AABB);
        barrierL->setSize(glm::vec2(100, playAreaScale.y + 150));
        mapBarriers.push_back(barrierL);

        auto barrierR = new Entity({ 50 + playAreaScale.x, 0 });
        barrierR->addMesh(meshes["quad"], 300, { 0.047, 0.176, 0.282 });
        barrierR->setCollider(new AABB);
        barrierR->setSize(glm::vec2(100, playAreaScale.y + 150));
        mapBarriers.push_back(barrierR);


        auto barrierU = new Entity({ 0, 50 + playAreaScale.y});
        barrierU->addMesh(meshes["quad"], 300, { 0.047, 0.176, 0.282 });
        barrierU->setCollider(new AABB);
        barrierU->setSize(glm::vec2(playAreaScale.x + 150, 100));
        mapBarriers.push_back(barrierU);

        auto barrierD = new Entity({ 0, - 50 - playAreaScale.y});
        barrierD->addMesh(meshes["quad"], 300, { 0.047, 0.176, 0.282 });
        barrierD->setCollider(new AABB);
        barrierD->setSize(glm::vec2(playAreaScale.x + 150, 100));
        mapBarriers.push_back(barrierD);
    }

    {
        auto prop = new Entity(glm::vec2(100, 200));
        prop->addMesh(meshes["quad"], 99, { 0.423, 0.4, 0.921 });
        prop->setSize(glm::vec2(100));
        prop->setCollider(new AABB);
        prop->setSize({ 100, 100 });
        props.emplace_back(prop);
    }
   
    {
        player = new NPC({ 0, 0 });
        // eyes
        player->addMesh(meshes["circle"], 50, { 0.047, 0.176, 0.282 }, { -0.75, -0.55 }, 0.65f);
        player->addMesh(meshes["circle"], 50, { 0.047, 0.176, 0.282 }, {  0.75, -0.55 }, 0.65f);
        player->addMesh(meshes["circle"], 49, { 0.180, 0.545, 0.752 }, { -0.75, -0.55 }, 0.5f);
        player->addMesh(meshes["circle"], 49, { 0.180, 0.545, 0.752 }, {  0.75, -0.55 }, 0.5f);
        // torso
        player->addMesh(meshes["circle"], 52, { 0.047, 0.176, 0.282 }, {}, 1.2f);
        player->addMesh(meshes["circle"], 51, { 0.078, 0.364, 0.627 });

        player->setCollider(new Circle);
        player->setSize(glm::vec2(50));
        player->addWeapon(new Shotgun);
        player->setVelocity(player->getVelocity() * 1.5f);
    }

    //{
    ////https://colorhunt.co/palette/125c133e7c17f4a442e8e1d9
    ////https://colorhunt.co/palette/000000aa14f0bc8cf2eeeeee
    ////https://colorhunt.co/palette/2f86a634be822fdd92f2f013
    ////https://colorhunt.co/palette/0000001500503f0071610094
    //    NPC* e = new NPC({ -100, 100 });
    //    // eyes
    //    e->addMesh(meshes["quad"], 46, {}, { -0.75, -0.65 }, 0.65f);
    //    e->addMesh(meshes["quad"], 46, {}, { 0.75, -0.65 }, 0.65f);
    //    e->addMesh(meshes["quad"], 45, { 0.380392, 0, 0.580392 }, { -0.75, -0.65 }, 0.5f);
    //    e->addMesh(meshes["quad"], 45, { 0.380392, 0, 0.580392 }, { 0.75, -0.65 }, 0.5f); 
    //    // torso
    //    e->addMesh(meshes["quad"], 48, {}, {}, 1.2f);
    //    e->addMesh(meshes["quad"], 47, { 0.247059, 0, 0.443137 });
    //    e->setCollider(new AABB);
    //    e->setSize(glm::vec2(50));
    //    enemies.emplace_back(e);
    //}

    //{
    //    NPC* e = new NPC({ 100, 100 });
    //    // torso
    //    e->addMesh(meshes["circle"], 51, {}, {}, 1.2f);
    //    e->addMesh(meshes["circle"], 50, { 0.313, 0.062, 0.003 });
    //    // eyes
    //    e->addMesh(meshes["circle"], 50, {}, { -0.75, -0.65 }, 0.65f);
    //    e->addMesh(meshes["circle"], 50, {}, { 0.75, -0.65 }, 0.65f);
    //    e->addMesh(meshes["circle"], 49, { 0.815, 0, 0 }, { -0.75, -0.65 }, 0.5f);
    //    e->addMesh(meshes["circle"], 49, { 0.815, 0, 0 }, { 0.75, -0.65 }, 0.5f);
    //    e->setCollider(new AABB);
    //    e->setSize(glm::vec2(50));
    //    enemies.emplace_back(e);
    //}

    {
        Shader* shader = new Shader("SimpleColor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "SimpleVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "SimpleFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("Minimap");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "MinimapVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "shaders", "MinimapFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    minimap = new Minimap(this);
}
void Game::Update(float deltaTimeSeconds) {
    checkBulletCollision(deltaTimeSeconds);
    spawnEnemy();
    glClearColor(0, 0, 0, 1);
    
    for (auto prop : props)
        prop->Render(shaders["SimpleColor"], GetSceneCamera());
    
    for (auto barrier : mapBarriers)
        barrier->Render(shaders["SimpleColor"], GetSceneCamera());

   
    player->Render(shaders["SimpleColor"], GetSceneCamera());

    
    for (auto proj : projectiles) {
        proj->goForward(deltaTimeSeconds);
        proj->Render(shaders["SimpleColor"], GetSceneCamera());
    }

    for (auto en : enemies) {
        // TODO UPDATE HEALTHBARS
        en->Render(shaders["SimpleColor"], GetSceneCamera());
        en->setDirection(player->getPosition() - en->getPosition());
        if (glm::distance2(en->getPosition(), player->getPosition()) > 1)
            en->goForward(deltaTimeSeconds);
    }
    
    // TODO draw minimap 
    minimap->Render(shaders["SimpleColor"]);
}
void Game::OnInputUpdate(float deltaTime, int mods) {
    // if WASD check collision

    {
        auto pos = player->getPosition();
        if (window->KeyHold(GLFW_KEY_W))
            player->addToPosition({ 0, deltaTime * player->getVelocity() });
        if (window->KeyHold(GLFW_KEY_S))
            player->addToPosition({ 0, -deltaTime * player->getVelocity() });
        for (auto prop : props)
            if (prop->checkCollision(player)) {
                player->setPosition(pos);
                break;
            }
        for (auto barrier : mapBarriers)
            if (barrier->checkCollision(player)) {
                player->setPosition(pos);
                break;
            }
    }
    
    {
        auto pos = player->getPosition();

        if (window->KeyHold(GLFW_KEY_A))
            player->addToPosition({ -deltaTime * player->getVelocity(), 0 });
        if (window->KeyHold(GLFW_KEY_D))
            player->addToPosition({ deltaTime * player->getVelocity(), 0 });
        for (auto prop : props)
            if (prop->checkCollision(player)) {
                player->setPosition(pos);
                break;
            }
        for (auto barrier : mapBarriers)
            if (barrier->checkCollision(player)) {
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
    if (button - 1 == GLFW_MOUSE_BUTTON_LEFT) {
        auto bullets = player->shoot(this);
        projectiles.insert(projectiles.end(), bullets.begin(), bullets.end());
    }
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
    float ratio = halfresolution.x / halfresolution.y;

    camera->SetOrthographic(-(float)halfresolution.x, (float)halfresolution.x, -(float)halfresolution.y * ratio, (float)halfresolution.y * ratio, 0.01f, 400);
    camera->Update();
}

void Game::checkBulletCollision(float deltaTimeSeconds) {
    for (int i = projectiles.size() - 1; i >= 0; i--) {
        auto proj = projectiles[i];
        bool collided = false;
        if (!proj->isSource(player) && proj->checkCollision(player)) {
            player->onCollision(proj);
            collided = true;
        } else {
            for (auto en : enemies)
                if (!proj->isSource(en) && en->checkCollision(proj)) {
                    en->onCollision(proj);
                    collided = true;
                    break;
                }
            if (!collided) {
                for (auto prop : props)
                    if (prop->checkCollision(proj)) {
                        collided = true;
                        break;
                    }
            }
            if (!collided) {
                for (auto barrier : mapBarriers)
                    if (barrier->checkCollision(proj)) {
                        collided = true;
                        break;
                    }
            }
        }

        if (collided || proj->getRemainingTime() <= -1) {
            projectiles.erase(projectiles.begin() + i);
            continue;
        }

        proj->decrementRemainingTime(deltaTimeSeconds);


        for (int i = enemies.size() - 1; i >= 0; i--) {
            if (enemies[i]->getHealth() <= 0)
                enemies.erase(enemies.begin() + i);
        }

    }

}
void Game::spawnEnemy() {
    if (enemies.size() > 5) return;

    glm::vec2 dir = glm::rotate(glm::mat4(1), glm::radians((float)(rand() % 360)), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
    float distance = rand() % 100 + 1000;
    glm::vec2 pos = player->getPosition() + dir * distance;
    if (-playAreaScale.x < pos.x && pos.x < playAreaScale.x &&
        -playAreaScale.y < pos.y && pos.y < playAreaScale.y) {
        NPC* e = new Kamikaze({ pos });
        // eyes
        e->addMesh(meshes["quad"], 46, {}, { -0.75, -0.65 }, 0.65f);
        e->addMesh(meshes["quad"], 46, {}, { 0.75, -0.65 }, 0.65f);
        e->addMesh(meshes["quad"], 45, { 0.380392, 0, 0.580392 }, { -0.75, -0.65 }, 0.5f);
        e->addMesh(meshes["quad"], 45, { 0.380392, 0, 0.580392 }, { 0.75, -0.65 }, 0.5f); 
        // torso
        e->addMesh(meshes["quad"], 48, {}, {}, 1.2f);
        e->addMesh(meshes["quad"], 47, { 0.247059, 0, 0.443137 });
        e->setVelocity(e->getVelocity() * 0.5);
        e->setCollider(new AABB);
        e->setSize(glm::vec2(50));
        enemies.emplace_back(e);
    }
}
