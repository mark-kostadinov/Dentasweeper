#include "GameRenderer.h"
#include <iterator>

GameRenderer::GameRenderer(const GameRenderer& other)
{

}

void GameRenderer::RenderFrame(RenderWindow& window)
{
	window.clear();

	for (auto it = m_renderList.begin(); it < m_renderList.end(); ++it)
	{
		if (Drawable* item = *it)
		{
			window.draw(*item);
		}
		else
		{
			m_renderList.erase(it);
		}
	}

	window.display();
}

void GameRenderer::AddToRenderList(Drawable& item)
{
	m_renderList.push_back(&item);
}

void GameRenderer::ClearRenderList()
{
	m_renderList.clear();
}

GameRenderer::~GameRenderer()
{
	ClearRenderList();
}
