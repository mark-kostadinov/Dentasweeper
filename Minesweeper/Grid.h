#pragma once

#include "Cell.h"

const static int s_maxNumCells = 50;

struct GridSettings
{
	GridSettings(const int numCells = 20, const float cellSize = 20.0f, const float windowOffsetX = 0.0f, const float windowOffsetY = 0.0f)
		: m_numCells(numCells)
		, m_cellSize(cellSize)
		, m_windowOffsetX(windowOffsetX)
		, m_windowOffsetY(windowOffsetY)
	{
	}

	int m_numCells;
	float m_cellSize;
	float m_windowOffsetX;
	float m_windowOffsetY;
};

class Grid final
{
public:
	Grid(const GridSettings& settings);
	~Grid();
	Grid(const Grid& other);

	int RevealAdjacentCells(Cell* cell);
	void RevealAllCells();

private:
	void Initialise();

public:
	GridSettings m_settings;
	Cell* m_cells[s_maxNumCells][s_maxNumCells];
	int m_totalNumCellsUncovered;
	int m_totalNumMines;
};
