#pragma once

#include "SFML/Window/Event.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Grid.h"
#include "GameRenderer.h"
#include "SFML/System/Time.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Audio/Sound.hpp"

using namespace sf;

enum GameState 
{
	PLAYING, 
	WON, 
	LOST,
	ENDGAME
};

class Game final
{
public:
	Game(const RenderWindow& window, GameRenderer& renderer);
	~Game();
	Game(const Game& other) = delete;

	void SetState(GameState state);

	bool IsPlaying() const { return m_state == GameState::PLAYING; }
	bool IsWon() const { return m_state == GameState::WON; }
	bool IsLost() const { return m_state == GameState::LOST; }
	bool IsEndgame() const { return m_state == GameState::ENDGAME; }

	void ProcessEvents(RenderWindow& window);
	void ProcessLogic();

private:
	void Initalise(const RenderWindow& window);
	void Restart(const RenderWindow& window);
	void AddAssetsToRenderList();
	void CleanUp();
	void GenerateEndGameAssets();

private:
	GameState m_state;
	Clock m_gameClock;
	int m_score;
	int m_scoreFromFlaggedMines;
	int m_cellAnimUpdateTimer;

	GameRenderer& m_renderer;
	Grid* m_grid;

	Sprite* m_background;
	Sprite* m_playAgainButton;
	Text* m_playAgainText;
	Text* m_gameTimeText;
	Text* m_gameOverText;
	Text* m_scoreText;

	Sound* m_endgameSound;
	Sound* m_cellUncoveredSound;
	Sound* m_cellFlaggedSound;
	Sound* m_cellUnflaggedSound;
};
