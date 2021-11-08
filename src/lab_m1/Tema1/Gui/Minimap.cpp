#include "Minimap.h"

Minimap::Minimap(Game* game) {
	this->game = game;
    frame = new Entity();
}

Minimap::~Minimap() {}

void Minimap::Render(Shader* shader) {
    glm::ivec2 resolution = game->window->GetResolution();
    
    //glViewport(0, 0, resolution.x / 5.f, resolution.y / 5.f);
    glScissor(0, 0, resolution.x / 6.5f, resolution.y / 6.5f);
    glClearColor(0, 0.047, 0.4, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glScissor(0, 0, resolution.x / 7.f, resolution.y / 7.f);
    glViewport(0, 0, resolution.x / 7.f, resolution.y / 7.f);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    auto camera = game->GetSceneCamera();
    camera->SetOrthographic(-(float)resolution.x, (float)resolution.x, -(float)resolution.y, (float)resolution.y, 0.01f, 400);

    for (auto prop : game->props)
        prop->Render(shader, game->GetSceneCamera());

    for (auto en : game->enemies) {
        en->Render(shader, game->GetSceneCamera());
        en->setDirection(game->player->getPosition() - en->getPosition());
    }
    
    if (game->pickup)
        game->pickup->Render(shader, game->GetSceneCamera());

    game->player->Render(shader, game->GetSceneCamera());

    
    // Sets the screen area where to draw
    glViewport(0, resolution.y, resolution.x, 0);
    glScissor(0, 0, resolution.x, resolution.y);
    camera->SetOrthographic(-(float)resolution.x / 2, (float)resolution.x / 2, -(float)resolution.y / 2, (float)resolution.y / 2, 0.01f, 400);

}
