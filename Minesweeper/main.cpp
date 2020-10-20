#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <cmath>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "Utils.h"
#include "ResourceManager.h"
#include "GameRenderer.h"
#include "Game.h"

using namespace sf;

int main()
{
	// Use random seed based on the current time
	srand((unsigned int)time(0));

	// Load assets
	ResourceManager::Get().LoadAssets();

	// Play the background music
	ResourceManager::Get().m_backgroundMusic.setVolume(50.0f);
	ResourceManager::Get().m_backgroundMusic.play();
	ResourceManager::Get().m_backgroundMusic.setLoop(true);

	ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(600, 800), "Dentasweeper", Style::Titlebar | Style::Close, contextSettings);
	window.setPosition(Vector2i((int)((VideoMode::getDesktopMode().width - 600) * 0.5f), (int)((VideoMode::getDesktopMode().height - 800) * 0.4f)));

	Cursor cursor;
	cursor.loadFromPixels(ResourceManager::Get().m_cursorImage.getPixelsPtr(), ResourceManager::Get().m_cursorImage.getSize(), { 5, 5 });
	window.setMouseCursor(cursor);
	window.setIcon(ResourceManager::Get().m_appIconImage.getSize().x, ResourceManager::Get().m_appIconImage.getSize().y, ResourceManager::Get().m_appIconImage.getPixelsPtr());

	GameRenderer renderer;
	Game game(window, renderer);

	// Game loop
	while (window.isOpen())
	{
		game.ProcessEvents(window);
		game.ProcessLogic();
		renderer.RenderFrame(window);
	}

	return EXIT_SUCCESS;
}