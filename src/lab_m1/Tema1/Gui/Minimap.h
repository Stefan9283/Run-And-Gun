#pragma once
#include "lab_m1/Tema1/Game.h"

class Minimap {
	Game* game;
public:
	Minimap(Game* game);
	~Minimap();
	void Render(Shader* shader);
	Entity* frame;
};