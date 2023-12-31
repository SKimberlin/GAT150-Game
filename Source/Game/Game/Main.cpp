#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "Renderer/Font.h"
#include "Renderer/Text.h"
#include "Audio/AudioSystem.h"
#include "Renderer/Texture.h"
#include "Physics/PhysicsSystem.h"

#include "Framework/Framework.h"

#include "SpaceGame.h"

#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <functional>

using namespace std;

void print(int i)
{
	cout << i << endl;
}

class A
{
public:
	void print(int i)
	{
		cout << i << endl;
	}
};

int main(int argc, char* argv[])
{
	//void (*func_ptr)(int) = print;
	//func_ptr(3);

	std::function<void(int)> func_ptr;
	func_ptr = print;

	A a;
	func_ptr = std::bind(&A::print, &a, std::placeholders::_1);
	func_ptr(4);
	

	INFO_LOG("Main Start");

	kiko::MemoryTracker::Initialize();

	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");
	
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC195", 800, 600);
	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	
	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();
	
	//main game loop
	bool quit = false;
	while (!quit)
	{
		kiko::g_time.Tick();

		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}
		
		game->Update(kiko::g_time.GetDeltaTime());

		kiko::g_audioSystem.Update();
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());
		

		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		game->Draw(kiko::g_renderer);

		kiko::g_renderer.EndFrame();

	}

	return 0;
}
