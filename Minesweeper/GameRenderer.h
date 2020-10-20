#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

using namespace sf;

class GameRenderer final
{
public:
	GameRenderer() = default;
	~GameRenderer();
	GameRenderer(const GameRenderer& other);

	void RenderFrame(RenderWindow& window);
	void AddToRenderList(Drawable& item);
	void ClearRenderList();

private:
	std::vector<Drawable*> m_renderList;
};
