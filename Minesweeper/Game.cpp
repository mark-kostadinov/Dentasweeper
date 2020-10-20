#include "Game.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "SFML/Graphics/Text.hpp"

//#define UNCOVER_ALL

Game::Game(const RenderWindow& window, GameRenderer& renderer)
	: m_state(GameState::PLAYING)
	, m_gameClock()
	, m_score(0)
	, m_scoreFromFlaggedMines(0)
	, m_cellAnimUpdateTimer(0)
	, m_renderer(renderer)
	, m_grid(nullptr)
	, m_background(nullptr)
	, m_playAgainButton(nullptr)
	, m_playAgainText(nullptr)
	, m_gameTimeText(nullptr)
	, m_gameOverText(nullptr)
	, m_scoreText(nullptr)
	, m_endgameSound(nullptr)
	, m_cellUncoveredSound(nullptr)
	, m_cellFlaggedSound(nullptr)
	, m_cellUnflaggedSound(nullptr)
{
	Initalise(window);
}

Game::~Game()
{
	CleanUp();
}

void Game::Initalise(const RenderWindow& window)
{
	const Vector2u windowSize = window.getSize();
	const float windowMinSize = fmin((float)windowSize.x, (float)windowSize.y);
	const float windowOffsetX = windowSize.x > windowSize.y ? windowSize.x - windowSize.y : 0.0f;
	const float windowOffsetY = windowSize.y > windowSize.x ? windowSize.y - windowSize.x : 0.0f;

	const float cellSize = fmax(windowMinSize * 0.02f, 40.0f);
	const int numCells = (int)(windowMinSize / cellSize);

	m_grid = new Grid(GridSettings(numCells, cellSize, windowOffsetX * 0.5f, windowOffsetY * 0.5f));

	m_scoreText = new Text(String("Score: 0"), ResourceManager::Get().m_font, 30);
	m_scoreText->setPosition(Vector2f(20.0f, 30.0f));
	m_scoreText->setFillColor(Color(51, 218, 255));
	m_scoreText->setOutlineColor(Color::Black);
	m_scoreText->setOutlineThickness(2.0f);

	m_gameTimeText = new Text(String("Time: 0"), ResourceManager::Get().m_font, 30);
	m_gameTimeText->setPosition(Vector2f(410.0f, 30.0f));
	m_gameTimeText->setFillColor(Color(255, 188, 64));
	m_gameTimeText->setOutlineColor(Color::Black);
	m_gameTimeText->setOutlineThickness(2.0f);

	m_cellUncoveredSound = new Sound(ResourceManager::Get().m_cellUncoveredSoundBuffer);
	m_cellUncoveredSound->setVolume(50.0f);
	m_cellFlaggedSound = new Sound(ResourceManager::Get().m_cellFlaggedSoundBuffer);
	m_cellFlaggedSound->setVolume(50.0f);
	m_cellUnflaggedSound = new Sound(ResourceManager::Get().m_cellUnflaggedSoundBuffer);
	m_cellUnflaggedSound->setVolume(50.0f);

	m_background = new Sprite(ResourceManager::Get().m_backgroundTexture);

	AddAssetsToRenderList();
}

void Game::SetState(GameState state)
{
	m_state = state;
}

void Game::ProcessEvents(RenderWindow& window)
{
	Event event;

	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();

		if (event.type == Event::MouseButtonPressed)
		{
			const Vector2f mousePos = Vector2f((float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y);

			auto findSelectedCell = [](Grid& grid, const Vector2f& mousePos)->Cell*
			{
				for (int x = 0; x < grid.m_settings.m_numCells; ++x)
				{
					for (int y = 0; y < grid.m_settings.m_numCells; ++y)
					{
						if (Cell* cell = grid.m_cells[x][y])
						{
							if (cell->getGlobalBounds().contains(mousePos))
								return cell;
						}
					}
				}

				return nullptr;
			};

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (IsEndgame())
				{
					if (m_playAgainButton && m_playAgainButton->getGlobalBounds().contains(mousePos))
						Restart(window);
				}
				
				if (IsPlaying())
				{
					if (Cell* cell = findSelectedCell(*m_grid, mousePos))
					{
						if (cell->IsMine())
							SetState(GameState::LOST);

						if (cell->IsCovered() || cell->IsFlagged())
						{
							cell->Uncover();
							m_grid->m_totalNumCellsUncovered++;
							m_score++;

							if (m_cellUncoveredSound)
								m_cellUncoveredSound->play();

							if (!cell->IsMine() && cell->m_adjacentMinesCount == 0)
								m_score += m_grid->RevealAdjacentCells(cell);
						}
					}
				}
			}
			else if (Mouse::isButtonPressed(Mouse::Right))
			{
				if (IsPlaying())
				{
					if (Cell* cell = findSelectedCell(*m_grid, mousePos))
					{
						if (!cell->IsUncovered())
						{
							if (cell->IsFlagged())
							{
								cell->Unflag();

								if (m_cellUnflaggedSound)
									m_cellUnflaggedSound->play();

								if (cell->IsMine())
									m_scoreFromFlaggedMines -= 2;
							}
							else
							{
								cell->Flag();

								if (m_cellFlaggedSound)
									m_cellFlaggedSound->play();

								if (cell->IsMine())
									m_scoreFromFlaggedMines += 2;
							}
						}
					}
				}
			}
		}
	}
}

void Game::ProcessLogic()
{
	if (IsPlaying())
	{
		const int elapsedTimeSeconds = (int)m_gameClock.getElapsedTime().asSeconds();

		// Update cell "animation"
		if (m_cellAnimUpdateTimer < elapsedTimeSeconds)
		{
			m_cellAnimUpdateTimer = elapsedTimeSeconds;

			auto FindRandomCoveredCell = [](Grid* grid)->Cell*
			{
				// Choose 1 random cell to update its texture
				int xRand = rand() % grid->m_settings.m_numCells - 0;
				int yRand = rand() % grid->m_settings.m_numCells - 0;

				if (Cell* cell = grid->m_cells[xRand][yRand])
				{
					if (cell->IsCovered())
						return cell;
				}

				return nullptr;
			};

			Cell* targetCell = nullptr;

			while (!targetCell)
				targetCell = FindRandomCoveredCell(m_grid);

			if (targetCell)
				targetCell->TriggerAnim();
		}

		// Check if game's won
		if (m_grid->m_totalNumCellsUncovered == (int)(std::pow(m_grid->m_settings.m_numCells, 2) - m_grid->m_totalNumMines) 
			&& m_grid->m_totalNumMines == (int)(m_scoreFromFlaggedMines * 0.5f))
		{
			SetState(GameState::WON);
			m_score += std::max(0, m_scoreFromFlaggedMines);
		}

		// Update game score and timer
		if (m_scoreText)
			m_scoreText->setString(String("Score: " + std::to_string(m_score)));

		if (m_gameTimeText)
		{
			int gameTimeSeconds = elapsedTimeSeconds;
			int gameTimeMinutes = gameTimeSeconds >= 60 ? gameTimeSeconds / 60 : 0;
			gameTimeSeconds -= gameTimeMinutes * 60;
			std::string secondsString = gameTimeSeconds >= 10 ? std::to_string(gameTimeSeconds) : "0" + std::to_string(gameTimeSeconds);
			m_gameTimeText->setString(String("Time: " + std::to_string(gameTimeMinutes) + ":" + secondsString));
		}
	}

	if (IsWon() || IsLost())
	{
		ResourceManager::Get().m_backgroundMusic.stop();

		m_grid->RevealAllCells();
		GenerateEndGameAssets();

		SetState(GameState::ENDGAME);
	}

	if (IsEndgame())
	{
		if (m_endgameSound && m_endgameSound->getStatus() != SoundSource::Playing && ResourceManager::Get().m_endgameBackgroundMusic.getStatus() != SoundSource::Playing)
		{
			ResourceManager::Get().m_endgameBackgroundMusic.setVolume(50.0f);
			ResourceManager::Get().m_endgameBackgroundMusic.play();
			ResourceManager::Get().m_endgameBackgroundMusic.setLoop(true);
		}
	}
}

void Game::AddAssetsToRenderList()
{
	m_renderer.AddToRenderList(*m_background);

	for (int x = 0; x < m_grid->m_settings.m_numCells; ++x)
	{
		for (int y = 0; y < m_grid->m_settings.m_numCells; ++y)
		{
			if (Cell* cell = m_grid->m_cells[x][y])
			{
				m_renderer.AddToRenderList(*cell);

#ifdef UNCOVER_ALL
				cell->Uncover();
#endif // UNCOVER_ALL
			}
		}
	}

	m_renderer.AddToRenderList(*m_scoreText);
	m_renderer.AddToRenderList(*m_gameTimeText);
}

void Game::CleanUp()
{
	SafeDelete(m_grid);
	SafeDelete(m_background);
	SafeDelete(m_playAgainButton);
	SafeDelete(m_playAgainText);
	SafeDelete(m_gameTimeText);
	SafeDelete(m_gameOverText);
	SafeDelete(m_scoreText);
	SafeDelete(m_endgameSound);
	SafeDelete(m_cellUncoveredSound);
	SafeDelete(m_cellFlaggedSound);
	SafeDelete(m_cellUnflaggedSound);
}

void Game::GenerateEndGameAssets()
{
	m_playAgainButton = new Sprite(Texture());
	m_playAgainButton->setColor(Color(255, 255, 255, 80));
	m_playAgainButton->setTextureRect(IntRect(0, 0, 200, 50));
	m_playAgainButton->setPosition(Vector2f(200.0f, 725.0f));
	m_renderer.AddToRenderList(*m_playAgainButton);

	m_playAgainText = new Text(String("Play again"), ResourceManager::Get().m_font, 32);
	m_playAgainText->setPosition(Vector2f(210.0f, 730.0f));
	m_playAgainText->setFillColor(Color(255, 41, 91));
	m_playAgainText->setOutlineColor(Color::Black);
	m_playAgainText->setOutlineThickness(2.0f);
	m_renderer.AddToRenderList(*m_playAgainText);

	m_gameOverText = new Text(String(IsWon() ? "Congratulations!" : "Better luck next time!"), ResourceManager::Get().m_font, 46);
	m_gameOverText->setPosition(IsWon() ? Vector2f(85.0f, 340.0f) : Vector2f(30.0f, 340.0f));
	m_gameOverText->setFillColor(Color(23, 255, 162));
	m_gameOverText->setOutlineColor(Color::Black);
	m_gameOverText->setOutlineThickness(4.0f);
	m_renderer.AddToRenderList(*m_gameOverText);

	m_endgameSound = new Sound(IsWon() ? ResourceManager::Get().m_gameWonSoundBuffer : ResourceManager::Get().m_gameLostSoundBuffer);
	m_endgameSound->setVolume(50.0f);
	m_endgameSound->play();
}

void Game::Restart(const RenderWindow& window)
{
	m_renderer.ClearRenderList();
	CleanUp();
	m_gameClock.restart();
	m_score = 0;
	m_scoreFromFlaggedMines = 0;
	m_cellAnimUpdateTimer = 0;

	SetState(GameState::PLAYING);
	Initalise(window);

	ResourceManager::Get().m_endgameBackgroundMusic.stop();
	ResourceManager::Get().m_backgroundMusic.play();
}
