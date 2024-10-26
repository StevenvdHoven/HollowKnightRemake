#pragma once
#include "Level.h"
#include "GUI.h"
#include "GUIimage.h"
#include "GUIText.h"
#include "Button.h"
#include "SoundStream.h"

class Menu final : public Level 
{
public:
	Menu(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager);

	void Update(float elpasedSec) override;
	void Draw() const override;

	void Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter = "") override;
	void Deload() override;

private:
	void CreateUI();

	GUI* m_MenuParent;

	GUIimage* m_TitleImage;
	GUIimage* m_BackgroundImage;
	Button* m_StartGameButton;
	Button* m_QuitGameButton;

	SoundStream* m_MainSoundTrack;
	SoundStream* m_MainBassTrack;
};

