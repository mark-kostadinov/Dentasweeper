#include "Cell.h"
#include "ResourceManager.h"
#include "SFML/System/Sleep.hpp"
#include "Utils.h"

using namespace sf;

Cell::Cell(const float size /* = 20.0f */)
	: RectangleShape(Vector2f(size, size))
	, m_state(CellState::COVERED)
	, m_adjacentMinesCount(0)
	, m_isMine(false)
	, m_hadReversedTexture(false)
	, m_isAnimProcessTriggered(false)
	, m_animProcessingThread(&Cell::ProcessAnimation, this)
{
	Initialise();
}

Cell::Cell(const Cell& other)
	: RectangleShape(other)
	, m_state(other.m_state)
	, m_adjacentMinesCount(other.m_adjacentMinesCount)
	, m_isMine(other.m_isMine)
	, m_hadReversedTexture(other.m_hadReversedTexture)
	, m_isAnimProcessTriggered(false)
	, m_animProcessingThread(&Cell::ProcessAnimation, this)
{
	Initialise();
}

Cell::~Cell()
{
	m_animProcessingThread.terminate();
}

void Cell::Initialise()
{
	// Half are going to look one way, the rest - the other way
	setTexture(rand() % 2 == 0 ? &ResourceManager::Get().m_coveredTexture : &ResourceManager::Get().m_coveredTextureReversed);
	setOutlineColor(Color::Black);
	setOutlineThickness(-1.0f);

	if (rand() % 5 == 0) // ~20% of them are going to be mines
		m_isMine = true;
}

void Cell::Uncover()
{
	if (m_isMine)
	{
		if (IsFlagged())
		{
			setTexture(&ResourceManager::Get().m_flaggedMineTexture);
		}
		else
		{
			setTexture(&ResourceManager::Get().m_mineTexture);
		}
	}
	else
	{
		switch (m_adjacentMinesCount)
		{
			case 0:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTextureVoid);
				break;
			}
			case 1:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture1);
				break;
			}
			case 2:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture2);
				break;
			}
			case 3:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture3);
				break;
			}
			case 4:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture4);
				break;
			}
			case 5:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture5);
				break;
			}
			case 6:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture6);
				break;
			}
			case 7:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture7);
				break;
			}
			case 8:
			{
				setTexture(&ResourceManager::Get().m_uncoveredTexture8);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	m_state = CellState::UNCOVERED;
}

void Cell::Flag()
{
	setTexture(&ResourceManager::Get().m_flaggedTexture);
	m_state = CellState::FLAGGED;
}

void Cell::Unflag()
{
	setTexture(rand() % 2 == 0 ? &ResourceManager::Get().m_coveredTexture : &ResourceManager::Get().m_coveredTextureReversed);
	m_state = CellState::COVERED;
}

void Cell::SetIsMine(bool val)
{
	m_isMine = val;
}

void Cell::TriggerAnim()
{
	if (!m_isAnimProcessTriggered)
	{
		m_animProcessingThread.launch();
		m_isAnimProcessTriggered = true;
	}
}

void Cell::ProcessAnimation()
{
	m_threadLock.lock();

	SwitchTexture();
	sf::sleep(sf::seconds(2.0f));
	SwitchTexture();
	m_isAnimProcessTriggered = false;

	m_threadLock.unlock();
}

void Cell::SwitchTexture()
{
	if (IsCovered())
	{
		if (const Texture* cellTexture = getTexture())
		{
			if (cellTexture == &ResourceManager::Get().m_coveredTexture)
			{
				setTexture(&ResourceManager::Get().m_coveredTextureForward);
				m_hadReversedTexture = false;
			}
			else if (cellTexture == &ResourceManager::Get().m_coveredTextureReversed)
			{
				setTexture(&ResourceManager::Get().m_coveredTextureForward);
				m_hadReversedTexture = true;
			}
			else
			{
				setTexture(m_hadReversedTexture ? &ResourceManager::Get().m_coveredTexture : &ResourceManager::Get().m_coveredTextureReversed);
			}
		}
	}
	else
	{
		m_animProcessingThread.terminate();
	}
}
