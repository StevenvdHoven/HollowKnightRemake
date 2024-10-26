#pragma once
#include <vector>

class GameObject;
class LevelManager;
class SpriteManager;
class PhysicsEngine;
class TextureManager;
class CameraManager;
class AudioManager;
class GUIManager;
class PlayerDataManager;
class GameObjectManager final
{
public:
	GameObjectManager(LevelManager* levelManager, SpriteManager* spriteManager, PhysicsEngine* physicsEngine, TextureManager* textureManager, CameraManager* cameraManager, AudioManager* audioManager, GUIManager* guiManager);
	~GameObjectManager();

	void StartAll();

	void UpdateAll(float elapsedSec);
	void DrawAll() const;

	void AddObject(GameObject* gameObject);
	void DeleteObject(const GameObject* gameObject);
	void DontDestroy(GameObject* gameObject);
	void RemoveFromDestroy(const GameObject* gameObject);

	void KeyDownEventAll(const SDL_KeyboardEvent& e);
	void KeyUpEventAll(const SDL_KeyboardEvent& e);
	void MouseMotionEventAll(const SDL_MouseMotionEvent& e);
	void MouseDownEventAll(const SDL_MouseButtonEvent& e);
	void MouseUpEventAll(const SDL_MouseButtonEvent& e);

	void Deload();

	GameObject* GetObjectBasedOfTag(const std::string& tag);
	std::vector<GameObject*> GetObjectsBasedOfTag(const std::string& tag);

	PlayerDataManager* GetPlayerDataManager();
	LevelManager* GetLevelManager();
	SpriteManager* GetSpriteManager();
	PhysicsEngine* GetPhysicsEngine();
	TextureManager* GetTextureManager();
	CameraManager* GetCameraManager();
	AudioManager* GetAudioManager();
	GUIManager* GetGUIManager();

private:
	bool m_Started;

	std::vector<GameObject*> m_CurrentGameObjects;
	std::vector<GameObject*> m_DontDestroyGameObjects;


	PlayerDataManager* m_PlayerDataManager;
	LevelManager* m_LevelManager;
	SpriteManager* m_SpirteManager;
	PhysicsEngine* m_PhysicsEngine;
	TextureManager* m_TextureManager;
	CameraManager* m_CameraManager;
	AudioManager* m_AudioManager;
	GUIManager* m_GUIManager;
};

