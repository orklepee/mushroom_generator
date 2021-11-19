#include "MenuScene.h"

void MenuScene::OnCreate()
{
}

void MenuScene::OnDestroy()
{
}

// Put all events within this OnUpdate() function call
void MenuScene::OnUpdate(float dt)
{
	static std::array<std::string, 2> MenuItems = {
		"play",
		"exit"
	};
	static int Selection = 0;

	// Consider how input will be handled? Get mouse and/or keys?

};