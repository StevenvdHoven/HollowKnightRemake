#include "pch.h"
#include "Menu.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GUIManager.h"
#include "AudioManager.h"

Menu::Menu(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager):
	Level(map,levelManager,gameObjectManager)
{
}

void Menu::Update(float elpasedSec)
{
	Level::Update(elpasedSec);
}

void Menu::Draw() const
{
	Level::Draw();
}

void Menu::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
{
	Level::Load(gameObjectManager, enemyManager, parameter);
	CreateUI();
}

void Menu::Deload()
{
	m_MenuParent->SetActive(false);
	Level::Deload();
}

void Menu::CreateUI()
{
	TextureManager* manager{ m_GameObjectManager->GetTextureManager() };
	GUIManager* guiManager{ m_GameObjectManager->GetGUIManager() };

	m_MenuParent = new GUI();
	guiManager->AddGUI(m_MenuParent);

	m_BackgroundImage = new GUIimage
	{
		Transform{Vector2f{1920 * 0.5f,1080 * 0.5f},0,Vector2f{.7f,.7f}},
		manager->GetTexture("UI/Menu_BG.png"),
	};
	m_BackgroundImage->SetParent(m_MenuParent);

	m_TitleImage = new GUIimage
	{
		Transform{Vector2f{1920 * 0.5f,1080 * 0.75},0,Vector2f{1,1}},
		manager->GetTexture("UI/Title.png"),
	};
	m_TitleImage->SetParent(m_MenuParent);

	std::function<void()> onStartGame
	{
		[this]()
		{
			m_LevelManager->LoadLevel(level_01);
		}
	};

	m_StartGameButton = new Button
	{
		Rectf{1920 * 0.5f  - 250,1080 * 0.5f - 35,500,70},
		Color4f{0,0,0,0},
		white,
		"Start Game",
		guiManager->GetFont(trajan_pro_regular_40),
		ButtonTypes::Up,
		onStartGame
	};

	std::function<void()> onQuitGame
	{
		[this]()
		{
			SDL_Quit();
		}
	};

	m_QuitGameButton = new Button
	{
		Rectf{1920 * 0.5f - 250,1080 * 0.5f - (100 + 35),500,70},
		Color4f{0,0,0,0},
		white,
		"Quit Game",
		guiManager->GetFont(trajan_pro_regular_40),
		ButtonTypes::Up,
		onQuitGame
	};

	m_StartGameButton->SetParent(m_MenuParent);
	m_QuitGameButton->SetParent(m_MenuParent);
	
	AudioManager* audioManager{ m_GameObjectManager->GetAudioManager() };
	m_MainSoundTrack = audioManager->GetAudioStream("SoundTrack/S19 Crossroads Main.mp3");
	m_MainSoundTrack->Play(1);

	m_MainBassTrack = audioManager->GetAudioStream("SoundTrack/S19 Crossroads Bass.mp3");
	m_MainBassTrack->Play(1);
}
