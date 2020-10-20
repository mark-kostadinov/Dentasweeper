#include "ResourceManager.h"
#include "Utils.h"

using namespace sf;

void ResourceManager::LoadAssets()
{
	LoadImages();
	LoadTextures();
	LoadFonts();
	LoadSFX();
}

void ResourceManager::LoadTextures()
{
	if (!m_coveredTexture.loadFromFile("Assets/tooth.jpg"))
		LOG_WARNING("Failed to load the covered texture!");

	if (!m_coveredTextureForward.loadFromFile("Assets/toothForward.jpg"))
		LOG_WARNING("Failed to load the covered forward texture!");

	if (!m_coveredTextureReversed.loadFromFile("Assets/toothReversed.jpg"))
		LOG_WARNING("Failed to load the covered reversed texture!");

	if (!m_uncoveredTextureVoid.loadFromFile("Assets/0.jpg"))
		LOG_WARNING("Failed to load the (0) uncovered texture!");

	if (!m_uncoveredTexture1.loadFromFile("Assets/1.jpg"))
		LOG_WARNING("Failed to load the (1) uncovered texture!");

	if (!m_uncoveredTexture2.loadFromFile("Assets/2.jpg"))
		LOG_WARNING("Failed to load the (2) uncovered texture!");

	if (!m_uncoveredTexture3.loadFromFile("Assets/3.jpg"))
		LOG_WARNING("Failed to load the (3) uncovered texture!");

	if (!m_uncoveredTexture4.loadFromFile("Assets/4.jpg"))
		LOG_WARNING("Failed to load the (4) uncovered texture!");

	if (!m_uncoveredTexture5.loadFromFile("Assets/5.jpg"))
		LOG_WARNING("Failed to load the (5) uncovered texture!");

	if (!m_uncoveredTexture6.loadFromFile("Assets/6.jpg"))
		LOG_WARNING("Failed to load the (6) uncovered texture!");

	if (!m_uncoveredTexture7.loadFromFile("Assets/7.jpg"))
		LOG_WARNING("Failed to load the (7) uncovered texture!");

	if (!m_uncoveredTexture8.loadFromFile("Assets/8.jpg"))
		LOG_WARNING("Failed to load the (8) uncovered texture!");

	if (!m_flaggedTexture.loadFromFile("Assets/flag.jpg"))
		LOG_WARNING("Failed to load the flagged texture!");

	if (!m_flaggedMineTexture.loadFromFile("Assets/flaggedMine.jpg"))
		LOG_WARNING("Failed to load the flagged mine texture!");

	if (!m_mineTexture.loadFromFile("Assets/mine.jpg"))
		LOG_WARNING("Failed to load the mine texture!");

	if (!m_backgroundTexture.loadFromFile("Assets/background.jpg"))
		LOG_WARNING("Failed to load the background texture!");

	m_coveredTexture.setSmooth(true);
	m_uncoveredTextureVoid.setSmooth(true);
	m_uncoveredTexture1.setSmooth(true);
	m_uncoveredTexture2.setSmooth(true);
	m_uncoveredTexture3.setSmooth(true);
	m_uncoveredTexture4.setSmooth(true);
	m_uncoveredTexture5.setSmooth(true);
	m_uncoveredTexture6.setSmooth(true);
	m_uncoveredTexture7.setSmooth(true);
	m_uncoveredTexture8.setSmooth(true);
	m_flaggedTexture.setSmooth(true);
	m_flaggedMineTexture.setSmooth(true);
	m_mineTexture.setSmooth(true);
	m_backgroundTexture.setSmooth(true);
}

void ResourceManager::LoadImages()
{
	if (!m_cursorImage.loadFromFile("Assets/cursorImage.png"))
		LOG_WARNING("Failed to load the mouse cursor image!");

	if (!m_appIconImage.loadFromFile("Assets/icon.png"))
		LOG_WARNING("Failed to load the app icon image!");
}

void ResourceManager::LoadFonts()
{
	if (!m_font.loadFromFile("Assets/Fonts/TitanOne-Regular.ttf"))
		LOG_WARNING("Failed to load the main font!");
}

void ResourceManager::LoadSFX()
{
	if (!m_backgroundMusic.openFromFile("Assets/SFX/backgroundMusic.ogg"))
		LOG_WARNING("Failed to load the background music!");

	if (!m_endgameBackgroundMusic.openFromFile("Assets/SFX/endgameMusic.ogg"))
		LOG_WARNING("Failed to load the endgame background music!");

	if (!m_gameLostSoundBuffer.loadFromFile("Assets/SFX/gameLost.ogg"))
		LOG_WARNING("Failed to load the game lost sfx!");

	if (!m_gameWonSoundBuffer.loadFromFile("Assets/SFX/gameWon.ogg"))
		LOG_WARNING("Failed to load the game won sfx!");

	if (!m_cellUncoveredSoundBuffer.loadFromFile("Assets/SFX/click.ogg"))
		LOG_WARNING("Failed to load the cell uncovered sfx!");

	if (!m_cellFlaggedSoundBuffer.loadFromFile("Assets/SFX/flagged.ogg"))
		LOG_WARNING("Failed to load the cell flagged sfx!");

	if (!m_cellUnflaggedSoundBuffer.loadFromFile("Assets/SFX/unflagged.ogg"))
		LOG_WARNING("Failed to load the cell unflagged sfx!");
}
