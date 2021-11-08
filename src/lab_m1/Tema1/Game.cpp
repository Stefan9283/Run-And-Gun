#include "Game.h"
#include <iostream>
#include "Colliders/Collider.h"
#include "Colors.h"

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

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(-(float)resolution.x / 2, (float)resolution.x / 2,
                                -(float)resolution.y / 2, (float)resolution.y / 2, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    guiCamera.SetPosition(glm::vec3(0, 0, 50));
    guiCamera.SetOrthographic(-(float)resolution.x / 2, (float)resolution.x / 2,
                            -(float)resolution.y / 2, (float)resolution.y / 2, 0.01f, 400);
    guiCamera.Update();

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
        Mesh* mesh = new Mesh("quad_wire");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::HW1, "models"), "quad_wireframe.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        playAreaScale = glm::vec2(3000, 4000);

        auto floor = new Entity();
        floor->addMesh(meshes["quad"], { 0.498, 0.509, 0.643 });
        floor->setSize(playAreaScale);
        props.emplace_back(floor);

        auto barrierL = new Entity({- 50 - playAreaScale.x, 0});
        barrierL->addMesh(meshes["quad"], { 0.047, 0.176, 0.282 });
        barrierL->setCollider(new AABB);
        barrierL->setSize(glm::vec2(100, playAreaScale.y + 150));
        mapBarriers.push_back(barrierL);

        auto barrierR = new Entity({ 50 + playAreaScale.x, 0 });
        barrierR->addMesh(meshes["quad"], { 0.047, 0.176, 0.282 });
        barrierR->setCollider(new AABB);
        barrierR->setSize(glm::vec2(100, playAreaScale.y + 150));
        mapBarriers.push_back(barrierR);


        auto barrierU = new Entity({ 0, 50 + playAreaScale.y});
        barrierU->addMesh(meshes["quad"], { 0.047f, 0.176f, 0.282f });
        barrierU->setCollider(new AABB);
        barrierU->setSize(glm::vec2(playAreaScale.x + 150, 100));
        mapBarriers.push_back(barrierU);

        auto barrierD = new Entity({ 0, - 50 - playAreaScale.y});
        barrierD->addMesh(meshes["quad"], { 0.047f, 0.176f, 0.282f });
        barrierD->setCollider(new AABB);
        barrierD->setSize(glm::vec2(playAreaScale.x + 150, 100));
        mapBarriers.push_back(barrierD);
    }

    {
        auto prop = new Entity(glm::vec2(100, 200));
        prop->addMesh(meshes["quad"], { 0.423, 0.4, 0.921 });
        prop->setSize(glm::vec2(100));
        prop->setCollider(new AABB);
        prop->setSize({ 100, 100 });
        props.emplace_back(prop);
    }
   
    pickup = nullptr;

    {
        player = new Player({ 0, 0 });
        // torso
        player->addMesh(meshes["circle"], { 0.047, 0.176, 0.282 }, {}, glm::vec2(1.2f));
        player->addMesh(meshes["circle"], { 0.078, 0.364, 0.627 });
        // eyes
        player->addMesh(meshes["circle"], { 0.047, 0.176, 0.282 }, { -0.75, -0.55 }, glm::vec2(0.65f));
        player->addMesh(meshes["circle"], { 0.047, 0.176, 0.282 }, {  0.75, -0.55 }, glm::vec2(0.65f));
        player->addMesh(meshes["circle"], { 0.180, 0.545, 0.752 }, { -0.75, -0.55 }, glm::vec2(0.5f));
        player->addMesh(meshes["circle"], { 0.180, 0.545, 0.752 }, {  0.75, -0.55 }, glm::vec2(0.5f));

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
    checkCollisions(deltaTimeSeconds);
    //spawnEnemy();
    spawnPickUp();
    for (auto prop : props)
        prop->Render(shaders["SimpleColor"], GetSceneCamera());
    for (auto barrier : mapBarriers)
        barrier->Render(shaders["SimpleColor"], GetSceneCamera());
    
    if (player->getHealth() != 0)
        player->Render(shaders["SimpleColor"], GetSceneCamera());
    
    for (auto proj : projectiles) {
        proj->goForward(deltaTimeSeconds);
        proj->Render(shaders["SimpleColor"], GetSceneCamera());
    }


    for (auto en : enemies) {
        {
            float minDist = playAreaScale.x + playAreaScale.y;
            Projectile* closestProj = nullptr;
            for (auto proj : projectiles) {
                float dist = glm::length(proj->getPosition() - en->getPosition());
                if (dist < minDist) {
                    minDist = dist;
                    closestProj = proj;
                }
            }
            if (closestProj)
                en->avoidEntity(deltaTimeSeconds / 2, closestProj);
        }
        en->setDirection(player->getPosition() - en->getPosition());
        if (player->getHealth() != 0)
            en->goForward(deltaTimeSeconds);

        en->Render(shaders["SimpleColor"], GetSceneCamera());
    }

    if (pickup)
        pickup->Render(shaders["SimpleColor"], GetSceneCamera());

    renderHealthBars();
    minimap->Render(shaders["SimpleColor"]);
}
void Game::OnInputUpdate(float deltaTime, int mods) {
    if (player->getHealth() == 0) return;
    
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

    camera->SetOrthographic(-(float)halfresolution.x, (float)halfresolution.x, -(float)halfresolution.y, (float)halfresolution.y, 0.01f, 400);
    camera->Update();
    guiCamera.SetOrthographic(-(float)halfresolution.x, (float)halfresolution.x, -(float)halfresolution.y, (float)halfresolution.y, 0.01f, 400);
    guiCamera.Update();
}

glm::vec3 getHealthBarColor(float healthRatio) {
    glm::vec3 color;
    if (healthRatio > 0.75) {
        color = glm::mix(glm::vec3(1, 0.701, 0.098), glm::vec3(0, 1, 0), (healthRatio - 0.75) / 0.25);
    }
    else if (healthRatio > 0.5) {
        color = glm::mix(glm::vec3(0.662, 0.2, 0.227), glm::vec3(1, 0.701, 0.098), (healthRatio - 0.50) / 0.25);
    }
    else {
        color = glm::mix(glm::vec3(0, 0, 0), glm::vec3(0.662, 0.2, 0.227), healthRatio / 0.5);
    }
    return color;
}

void Game::renderHealthBars() {
    glm::vec2 resolution = window->GetResolution();

    auto shader = shaders["SimpleColor"];
    shader->Use();
    

    // Enemy HealthBar
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniform2f(shader->loc_offset, 0, 0);
    glUniform2f(shader->loc_scale, 1, 1);

    for (auto en : enemies) {
        auto size = en->getSize();
        auto position = en->getPosition();

        auto model =
            glm::translate(glm::mat4(1), glm::vec3(position.x - size.x, position.y + 2.5f * size.y, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(size.x, size.y / 5, 1)) *
            glm::translate(glm::mat4(1), glm::vec3(1, -1, 0));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

        float healthRatio = en->getHealth() / (float)en->getMaxHealth();
        auto color = getHealthBarColor(healthRatio);
        glUniform3f(shader->loc_color, color.x, color.y, color.z);

        meshes["quad_wire"]->Render();

        model =
            glm::translate(glm::mat4(1), glm::vec3(position.x - size.x, position.y + 2.5f * size.y, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(size.x * en->getHealth() / en->getMaxHealth(), size.y / 5, 1)) *
            glm::translate(glm::mat4(1), glm::vec3(1, -1, 0));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

        meshes["quad"]->Render();
    }

    {
        // player HealthBar
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(guiCamera.GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(guiCamera.GetProjectionMatrix()));

        float healthRatio = player->getHealth() / (float)player->getMaxHealth();
        auto color = getHealthBarColor(healthRatio);

        glUniform3f(shader->loc_color, color.x, color.y, color.z);

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
            glm::value_ptr(
                glm::translate(glm::mat4(1), glm::vec3(-resolution.x * 3 / 2, resolution.y / 2, 0)) *
                glm::scale(glm::mat4(1), glm::vec3(2 * resolution.x, resolution.y / 100, 1)) *
                glm::translate(glm::mat4(1), glm::vec3(1, -1, 0))
            ));
        meshes["quad_wire"]->Render();

        auto model =
            glm::translate(glm::mat4(1), glm::vec3(-resolution.x / 2, resolution.y / 2, 0)) *
            glm::scale(glm::mat4(1), glm::vec3(resolution.x / 2.f * player->getHealth() / player->getMaxHealth(), resolution.y / 100, 1)) *
            glm::translate(glm::mat4(1), glm::vec3(1, -1, 0));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

        meshes["quad"]->Render();
    }


    

}
void Game::checkCollisions(float deltaTimeSeconds) {
    for (int i = projectiles.size() - 1; i >= 0; i--) {
        auto proj = projectiles[i];
        bool collided = false;
        if (!proj->isSource(player) && proj->checkCollision(player)) {
            player->onCollision(proj);
            collided = true;
        } else {
            for (int i = enemies.size() - 1; i >= 0; i--)
                if (!proj->isSource(enemies[i]) && enemies[i]->checkCollision(proj)) {
                    enemies[i]->onCollision(proj);
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
    }

    for (auto en : enemies) {
        if (en->checkCollision(player)) {
            player->onCollision(en);
            en->onCollision(player);
        }
    }

    for (int i = enemies.size() - 1; i >= 0; i--) {
        if (enemies[i]->getHealth() <= 0)
            enemies.erase(enemies.begin() + i);
    }

    if (pickup && pickup->checkCollision(player)) {
        player->pickUp(pickup);
        delete pickup;
        pickup = nullptr;
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
        // torso
        e->addMesh(meshes["quad"], {}, {}, glm::vec2(1.2f));
        e->addMesh(meshes["quad"], { 0.247059, 0, 0.443137 });
        // eyes
        e->addMesh(meshes["quad"], {}, { -0.75, -0.65 }, glm::vec2(0.65f));
        e->addMesh(meshes["quad"], {}, { 0.75, -0.65 }, glm::vec2(0.65f));
        e->addMesh(meshes["quad"], { 0.380392, 0, 0.580392 }, { -0.75, -0.65 }, glm::vec2(0.5f));
        e->addMesh(meshes["quad"], { 0.380392, 0, 0.580392 }, { 0.75, -0.65 }, glm::vec2(0.5f));

        e->setVelocity(e->getVelocity() * 0.5);
        e->setCollider(new AABB);
        e->setSize(glm::vec2(50));
        enemies.emplace_back(e);
    }
}
void Game::spawnPickUp() {
    if (pickup) return;

    glm::vec2 dir = glm::rotate(glm::mat4(1), glm::radians((float)(rand() % 360)), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
    float distance = rand() % 100 + 100;
    glm::vec2 pos = player->getPosition() + dir * distance;

    int type = rand() % 5;
    pickup = new PickUp(pos, type);
    pickup->setDirection({ 0, 1 });
    pickup->setCollider(new Circle);
    pickup->setSize(glm::vec2(30));

    bool noCollisions = true;
    for (auto prop : props)
        if (prop->checkCollision(pickup)) {
            noCollisions = false;
            break;
        }

    if (noCollisions && -playAreaScale.x < pos.x && pos.x < playAreaScale.x &&
        -playAreaScale.y < pos.y && pos.y < playAreaScale.y) {
        
        switch (type) {
        case SHOTGUN_PICKUP:
            pickup->addMesh(meshes["circle"], glm::vec3(0), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 0.741, 0.294, 0.294 });
            pickup->addMesh(meshes["bullet"], glm::vec3(0), {}, glm::vec2(0.4));
            pickup->addMesh(meshes["bullet"], glm::vec3(0.388, 0, 0), {}, glm::vec2(0.3));
            break;
        case GUN_PICKUP:
            pickup->addMesh(meshes["circle"], glm::vec3(0), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 0.949, 0.941, 0.074 });
            pickup->addMesh(meshes["bullet"], glm::vec3(0), {}, glm::vec2(0.4));
            pickup->addMesh(meshes["bullet"], glm::vec3(0.949, 0.941, 0.074), {}, glm::vec2(0.3));
            break;
        case HEALTH_PICKUP:
            pickup->addMesh(meshes["circle"], glm::vec3(0.117, 0.341, 0.309), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 0.537, 0.709, 0.686 });
            pickup->addMesh(meshes["quad"], glm::vec3(0.117, 0.341, 0.309), {}, glm::vec2(0.8, 0.2));
            pickup->addMesh(meshes["quad"], glm::vec3(0.117, 0.341, 0.309), {}, glm::vec2(0.2, 0.8));
            break;
        case COOLDOWN_PICKUP:
            pickup->addMesh(meshes["circle"], glm::vec3(0.388, 0.039, 0.062), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 0.568, 0.121, 0.152 });
            pickup->addMesh(meshes["quad"], glm::vec3(0.388, 0.039, 0.062), {-0.55, 0}, glm::vec2(0.2, 0.1));
            pickup->addMesh(meshes["quad"], glm::vec3(0.388, 0.039, 0.062), {}, glm::vec2(0.2, 0.1));
            pickup->addMesh(meshes["quad"], glm::vec3(0.388, 0.039, 0.062), {0.55, 0}, glm::vec2(0.2, 0.1));
            break;
        case SPEED_PICKUP:
            pickup->addMesh(meshes["circle"], glm::vec3(0.196, 0.756, 0.803), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 0.090, 0.843, 0.627 });
            for (int i = 0; i < 3; i++) {
                float step = 0.3f;
                pickup->addMesh(meshes["quad"], glm::vec3(0.196, 0.756, 0.803), { -0.5 + step * i,   0.15 - step * i }, glm::vec2(0.1, 0.3));
                pickup->addMesh(meshes["quad"], glm::vec3(0.196, 0.756, 0.803), { -0.25 + step * i,   0.4 - step * i }, glm::vec2(0.3, 0.1));
            }
            pickup->setDirection({ -1, 1 });

            break;
            // TODO
        default:
            pickup->addMesh(meshes["circle"], glm::vec3(0), {}, glm::vec2(1.2f));
            pickup->addMesh(meshes["circle"], { 1, 0, 0 });
            break;
        }
        
    } else {
        delete pickup;
        pickup = nullptr;
    }
}
