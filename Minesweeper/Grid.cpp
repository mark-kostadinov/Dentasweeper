#include "Grid.h"
#include "Utils.h"

Grid::Grid(const GridSettings& settings)
	: m_settings(settings)
	, m_totalNumCellsUncovered(0)
	, m_totalNumMines(0)
{
	Initialise();
}

Grid::Grid(const Grid& other)
	: m_settings(other.m_settings)
	, m_totalNumCellsUncovered(other.m_totalNumCellsUncovered)
	, m_totalNumMines(other.m_totalNumMines)
{
	Initialise();
}

Grid::~Grid()
{
	for (int x = 0; x < m_settings.m_numCells; ++x)
	{
		for (int y = 0; y < m_settings.m_numCells; ++y)
			SafeDelete(m_cells[x][y]);
	}
}

void Grid::Initialise()
{
	// Cap the number of cells
	if (m_settings.m_numCells > s_maxNumCells)
		m_settings.m_numCells = s_maxNumCells;

	const int& maxCells = m_settings.m_numCells;

	// Initial setup
	for (int x = 0; x < maxCells; ++x)
	{
		for (int y = 0; y < maxCells; ++y)
		{
			Cell* cell = new Cell(m_settings.m_cellSize);
			cell->setPosition(sf::Vector2f(x * m_settings.m_cellSize, y * m_settings.m_cellSize));
			cell->move(sf::Vector2f(m_settings.m_windowOffsetX, m_settings.m_windowOffsetY));

			if (cell->IsMine())
				m_totalNumMines++;

			m_cells[x][y] = cell;
		}
	}

	// We need to make sure we have at least one mine
	if (m_totalNumMines == 0)
	{
		Cell& randomCell = *m_cells[rand() % maxCells][rand() % maxCells];
		randomCell.SetIsMine(true);
		m_totalNumMines++;
	}

	// Once we have all of them initialised, count the surrounding mines for each cell
	for (int x = 0; x < maxCells; ++x)
	{
		for (int y = 0; y < maxCells; ++y)
		{
			Cell& cell = *m_cells[x][y];

			Cell* up = y > 0 ? m_cells[x][y - 1] : nullptr;
			Cell* down = y + 1 < maxCells ? m_cells[x][y + 1] : nullptr;
			Cell* right = x + 1 < maxCells ? m_cells[x + 1][y] : nullptr;
			Cell* left = x > 0 ? m_cells[x - 1][y] : nullptr;
			Cell* upRight = x + 1 < maxCells && y > 0 ? m_cells[x + 1][y - 1] : nullptr;
			Cell* upLeft = x > 0 && y > 0 ? m_cells[x - 1][y - 1] : nullptr;
			Cell* downRight = x + 1 < maxCells && y + 1 < maxCells ? m_cells[x + 1][y + 1] : nullptr;
			Cell* downLeft = x > 0 && y + 1 < maxCells ? m_cells[x - 1][y + 1] : nullptr;

			auto increaseAdjMineCount = [&cell](Cell* target)
			{
				if (target && target->IsMine())
					cell.m_adjacentMinesCount++;
			};

			increaseAdjMineCount(up);
			increaseAdjMineCount(down);
			increaseAdjMineCount(right);
			increaseAdjMineCount(left);
			increaseAdjMineCount(upRight);
			increaseAdjMineCount(upLeft);
			increaseAdjMineCount(downRight);
			increaseAdjMineCount(downLeft);
		}
	}
}

int Grid::RevealAdjacentCells(Cell* cell)
{
	const int& maxCells = m_settings.m_numCells;
	int cellsRevealed = 0;

	for (int x = 0; x < maxCells; ++x)
	{
		for (int y = 0; y < maxCells; ++y)
		{
			if (cell == m_cells[x][y])
			{
				Cell* up = y > 0 ? m_cells[x][y - 1] : nullptr;
				Cell* down = y + 1 < maxCells ? m_cells[x][y + 1] : nullptr;
				Cell* right = x + 1 < maxCells ? m_cells[x + 1][y] : nullptr;
				Cell* left = x > 0 ? m_cells[x - 1][y] : nullptr;
				Cell* upRight = x + 1 < maxCells && y > 0 ? m_cells[x + 1][y - 1] : nullptr;
				Cell* upLeft = x > 0 && y > 0 ? m_cells[x - 1][y - 1] : nullptr;
				Cell* downRight = x + 1 < maxCells && y + 1 < maxCells ? m_cells[x + 1][y + 1] : nullptr;
				Cell* downLeft = x > 0 && y + 1 < maxCells ? m_cells[x - 1][y + 1] : nullptr;

				auto revealAdjacentCellsRecursively = [this, &cellsRevealed](Cell* cell)
				{
					if (cell && !cell->IsUncovered())
					{
						cell->Uncover();
						m_totalNumCellsUncovered++;
						cellsRevealed++;

						if (cell->m_adjacentMinesCount == 0)
							cellsRevealed += RevealAdjacentCells(cell);
					}
				};

				revealAdjacentCellsRecursively(up);
				revealAdjacentCellsRecursively(down);
				revealAdjacentCellsRecursively(right);
				revealAdjacentCellsRecursively(left);
				revealAdjacentCellsRecursively(upRight);
				revealAdjacentCellsRecursively(upLeft);
				revealAdjacentCellsRecursively(downRight);
				revealAdjacentCellsRecursively(downLeft);

				return cellsRevealed;
			}
		}
	}

	return cellsRevealed;
}

void Grid::RevealAllCells()
{
	const int& maxCells = m_settings.m_numCells;

	for (int x = 0; x < maxCells; ++x)
	{
		for (int y = 0; y < maxCells; ++y)
		{
			if (Cell* cell = m_cells[x][y])
			{
				if (!cell->IsUncovered())
					cell->Uncover();
			}
		}
	}
}
