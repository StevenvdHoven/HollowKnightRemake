#pragma once
#include "GameObject.h"

class LevelManager;
class SpriteManager;
class PhysicsEngine;
class TextureManager;
class CameraManager;
class AudioManager;
class GUIManager;
class PlayerDataManager;
class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init();
	virtual void Start();
	virtual void Update(float elapsedSec);
	virtual void Draw() const;

	virtual void KeyDownEvent(const SDL_KeyboardEvent& e);
	virtual void KeyUpEvent(const SDL_KeyboardEvent& e);
	virtual void MouseMotionEvent(const SDL_MouseMotionEvent& e);
	virtual void MouseDownEvent(const SDL_MouseButtonEvent& e);
	virtual void MouseUpEvent(const SDL_MouseButtonEvent& e);

	virtual void OnCollisionEnter(Collider* collider);
	virtual void OnCollisionStay(Collider* collider);
	virtual void OnCollisionExit(Collider* collider);

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual void OnDelete();

	GameObject* GetGameObject() const;
	void SetHolder(GameObject* object);

	template<class T>
	T* GetComponent();

	PlayerDataManager* GetPlayerDataManager();
	LevelManager* GetLevelManager();
	SpriteManager* GetSpriteManager();
	PhysicsEngine* GetPhysicsEngine();
	TextureManager* GetTextureManager();
	CameraManager* GetCameraManager();
	AudioManager* GetAudioManager();
	GUIManager* GetGUIManager();
	GameObjectManager* GetGameObjectManager();

	void SetEnabled(bool enabled);
	bool IsEnabled();
private:
	GameObject* m_Holder;
	bool m_IsEnabled;
	
};

template<class T>
inline T* Component::GetComponent()
{
	return m_Holder->GetComponent<T>();;
}
