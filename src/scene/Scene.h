#pragma once

#include "../Game.h"

class Scene
{
	// This allows Game class to access private members of Scene
	// This does not mean that Scene has access to Game, though.
	friend class Game;
public:
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float dt) = 0;
};
//using ScenePtr = std::shared_ptr<Scene>;