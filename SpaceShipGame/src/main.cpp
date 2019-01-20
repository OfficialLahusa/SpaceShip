#include "def.h"
#include "States/GameState.hpp"
#include <cmath>
#include <iostream>

//#include <windows.h>
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd);

int main()
{
	sse::Game game("Bomberman - Recreated by Lasse Huber-Saffer", sf::VideoMode().getDesktopMode() , [](sse::GameDataRef data) { data->machine.AddState(sse::StateRef(new sse::GameState(data))); });
	return EXIT_SUCCESS;
}