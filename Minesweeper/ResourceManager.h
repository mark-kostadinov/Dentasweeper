#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

using namespace sf;

class ResourceManager final
{
public:
	static ResourceManager& Get()
	{
		static ResourceManager instance;
		return instance;
	}

	void LoadAssets();

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager& other) = delete;

	void LoadTextures();
	void LoadImages();
	void LoadFonts();
	void LoadSFX();

public:
	Image m_cursorImage;
	Image m_appIconImage;
	Texture m_coveredTexture;
	Texture m_coveredTextureForward;
	Texture m_coveredTextureReversed;
	Texture m_uncoveredTextureVoid;
	Texture m_uncoveredTexture1;
	Texture m_uncoveredTexture2;
	Texture m_uncoveredTexture3;
	Texture m_uncoveredTexture4;
	Texture m_uncoveredTexture5;
	Texture m_uncoveredTexture6;
	Texture m_uncoveredTexture7;
	Texture m_uncoveredTexture8;
	Texture m_flaggedTexture;
	Texture m_flaggedMineTexture;
	Texture m_mineTexture;
	Texture m_backgroundTexture;
	Font m_font;
	Music m_backgroundMusic;
	Music m_endgameBackgroundMusic;
	SoundBuffer m_gameLostSoundBuffer;
	SoundBuffer m_gameWonSoundBuffer;
	SoundBuffer m_cellUncoveredSoundBuffer;
	SoundBuffer m_cellFlaggedSoundBuffer;
	SoundBuffer m_cellUnflaggedSoundBuffer;
};
