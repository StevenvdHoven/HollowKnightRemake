#pragma once
#include <vector>
#include "Transform.h"

class MapObject;
class Component;
class Collider;
class GameObjectManager;
class LevelManager;
class SpriteManager;
class PhysicsEngine;
class TextureManager;
class CameraManager;
class AudioManager;
class GUIManager;
class PlayerDataManager;
class GameObject
{
public:
	GameObject();
	GameObject(const MapObject* object);
	virtual ~GameObject();

	virtual void Init();
	virtual void Start();
	virtual void Update(float elapsedTime);
	virtual void Draw();

	virtual void KeyDownEvent(const SDL_KeyboardEvent& e);
	virtual void KeyUpEvent(const SDL_KeyboardEvent& e);
	virtual void MouseMotionEvent(const SDL_MouseMotionEvent& e);
	virtual void MouseDownEvent(const SDL_MouseButtonEvent& e);
	virtual void MouseUpEvent(const SDL_MouseButtonEvent& e);

	virtual void OnCollision(Collider* other);
	virtual void OnCollisionEnter(Collider* collider);
	virtual void OnCollisionStay(Collider* collider);
	virtual void OnCollisionExit(Collider* collider);

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	void SetManager(GameObjectManager* manager);

	void SetPosition(const Point2f& position, bool force = false);

	Point2f GetPosition();

	Vector2f GetFrontVector() const;
	Vector2f GetUpVector() const;

	template <class T>
	T* GetComponent();

	void AddComponent(Component* component);
	void DeleteComponent(const Component* component);

	void AddGameObject(GameObject* gameObject);
	void DeleteGameObject(const GameObject* gameObject);
	void AddToDontDestory(GameObject* gameObject);

	PlayerDataManager* GetPlayerDataManager();
	LevelManager* GetLevelManager();
	SpriteManager* GetSpriteManager();
	PhysicsEngine* GetPhysicsEngine();
	TextureManager* GetTextureManager();
	CameraManager* GetCameraManager();
	GameObjectManager* GetGameObjectManager();
	AudioManager* GetAudioManager();
	GUIManager* GetGUIManager();

	std::string m_Name;
	std::string m_Tag;
	Transform m_Transform;
private:

	GameObjectManager* m_Manager;

	std::vector<Component*> m_Components;
};

template<class T>
inline T* GameObject::GetComponent()
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		T* temp{ dynamic_cast<T*>(m_Components[index]) };
		if (temp != nullptr) {
			return temp;
		}
	}
	return nullptr;
}
