#include "pch.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "SpriteManager.h"
#include "PhysicsEngine.h"
#include "PlayerDataManager.h"

GameObjectManager::GameObjectManager(LevelManager* levelManager, SpriteManager* spriteManager, PhysicsEngine* physicsEngine, TextureManager* textureManager, CameraManager* cameraManager, AudioManager* audioManager, GUIManager* guiManager):
	m_Started{false},
	m_PlayerDataManager{ new PlayerDataManager{} },
	m_LevelManager{ levelManager },
	m_SpirteManager{ spriteManager },
	m_PhysicsEngine{ physicsEngine },
	m_TextureManager{ textureManager },
	m_CameraManager{ cameraManager },
	m_AudioManager{ audioManager },
	m_GUIManager{ guiManager }
{
}

GameObjectManager::~GameObjectManager()
{
	Deload();
	m_CurrentGameObjects.clear();

	for (int index{ 0 }; index < m_DontDestroyGameObjects.size(); ++index) {
		delete m_DontDestroyGameObjects[index];
	}
	m_DontDestroyGameObjects.clear();
	delete m_PlayerDataManager;
}

void GameObjectManager::StartAll()
{
	
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->Start();
		}
	}
	m_Started = true;
}

void GameObjectManager::UpdateAll(float elapsedSec)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->Update(elapsedSec);
		}
	}
}

void GameObjectManager::DrawAll() const
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->Draw();
		}
	}
}

void GameObjectManager::AddObject(GameObject* gameObject)
{
	gameObject->SetManager(this);
	gameObject->Init();
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index)
	{
		if (m_CurrentGameObjects[index] == nullptr)
		{
			m_CurrentGameObjects[index] = gameObject;
			if (m_Started) {
				gameObject->Start();
				return;
			}
		}
	}
	m_CurrentGameObjects.push_back(gameObject);
	if (m_Started) {
		gameObject->Start();
	}
}

void GameObjectManager::DeleteObject(const GameObject* gameObject)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] == gameObject)
		{
			RemoveFromDestroy(gameObject);
			delete m_CurrentGameObjects[index];
			m_CurrentGameObjects[index] = nullptr;
			return;
		}
	}
}

void GameObjectManager::DontDestroy(GameObject* gameObject)
{
	for (int index{ 0 }; index < m_DontDestroyGameObjects.size(); ++index) {
		if (m_DontDestroyGameObjects[index] == nullptr) {
			m_DontDestroyGameObjects[index] = gameObject;
			return;
		}
	}
	m_CurrentGameObjects.push_back(gameObject);
}

void GameObjectManager::RemoveFromDestroy(const GameObject* gameObject)
{
	for (int index{ 0 }; index < m_DontDestroyGameObjects.size(); ++index) {
		if (m_DontDestroyGameObjects[index] == gameObject) {
			m_DontDestroyGameObjects[index] = nullptr;
			return;
		}
	}
}

void GameObjectManager::KeyDownEventAll(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->KeyDownEvent(e);
		}
	}
}

void GameObjectManager::KeyUpEventAll(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->KeyUpEvent(e);
		}
	}
}

void GameObjectManager::MouseMotionEventAll(const SDL_MouseMotionEvent& e)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->MouseMotionEvent(e);
		}
	}
}

void GameObjectManager::MouseDownEventAll(const SDL_MouseButtonEvent& e)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->MouseDownEvent(e);
		}
	}
}

void GameObjectManager::MouseUpEventAll(const SDL_MouseButtonEvent& e)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		if (m_CurrentGameObjects[index] != nullptr) {
			m_CurrentGameObjects[index]->MouseUpEvent(e);
		}
	}
}

void GameObjectManager::Deload()
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index) {
		delete m_CurrentGameObjects[index];
		m_CurrentGameObjects[index] = nullptr;
	}
	m_CurrentGameObjects.clear();

	m_SpirteManager->Deload();
	m_PhysicsEngine->Deload();

	m_Started = false;
}

GameObject* GameObjectManager::GetObjectBasedOfTag(const std::string& tag)
{
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index)
	{
		if (m_CurrentGameObjects[index] != nullptr) 
		{
			if (m_CurrentGameObjects[index]->m_Tag == tag) 
			{
				return m_CurrentGameObjects[index];
			}
		}
	}
	return nullptr;
}

std::vector<GameObject*> GameObjectManager::GetObjectsBasedOfTag(const std::string& tag)
{
	std::vector<GameObject*> found;
	for (int index{ 0 }; index < m_CurrentGameObjects.size(); ++index)
	{
		if (m_CurrentGameObjects[index] != nullptr)
		{
			if (m_CurrentGameObjects[index]->m_Tag == tag)
			{
				found.push_back(m_CurrentGameObjects[index]);
			}
		}
	}
	return found;
}

PlayerDataManager* GameObjectManager::GetPlayerDataManager()
{
	return m_PlayerDataManager;
}

LevelManager* GameObjectManager::GetLevelManager()
{
	return m_LevelManager;
}

SpriteManager* GameObjectManager::GetSpriteManager()
{
	return m_SpirteManager;
}

PhysicsEngine* GameObjectManager::GetPhysicsEngine()
{
	return m_PhysicsEngine;
}

TextureManager* GameObjectManager::GetTextureManager()
{
	return m_TextureManager;
}

CameraManager* GameObjectManager::GetCameraManager()
{
	return m_CameraManager;
}

AudioManager* GameObjectManager::GetAudioManager()
{
	return m_AudioManager;
}

GUIManager* GameObjectManager::GetGUIManager()
{
	return m_GUIManager;
}
