#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Thread.hpp"
#include "SFML/System/Mutex.hpp"

using namespace sf;

enum CellState 
{
	COVERED,
	UNCOVERED,
	FLAGGED
};

class Cell final : public RectangleShape
{
public:
	Cell(const float size = 20.0f);
	~Cell();
	Cell(const Cell& other);

	void Uncover();
	void Flag();
	void Unflag();
	void SetIsMine(bool val);
	void TriggerAnim();

	bool IsCovered() const { return m_state == CellState::COVERED; }
	bool IsUncovered() const { return m_state == CellState::UNCOVERED; }
	bool IsFlagged() const { return m_state == CellState::FLAGGED; }
	bool IsMine() const { return m_isMine; }

private:
	void Initialise();
	void ProcessAnimation();
	void SwitchTexture();

public:
	int m_adjacentMinesCount;

private:
	CellState m_state;
	bool m_isMine;
	bool m_hadReversedTexture;
	bool m_isAnimProcessTriggered;
	Thread m_animProcessingThread;
	Mutex m_threadLock;
};
