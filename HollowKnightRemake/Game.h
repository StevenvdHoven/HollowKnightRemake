#pragma once
#include "BaseGame.h"

class GameObjectManager;
class GUIManager;
class TextureManager;
class LevelManager;
class SpriteManager;
class PhysicsEngine;
class CameraManager;
class EnemyManager;
class AudioManager;
class MapBuilder;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseWheelEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	
	TextureManager* m_TextureManager;
	LevelManager* m_LevelManager;
	SpriteManager* m_SpriteManager;
	PhysicsEngine* m_PhysicsEngine;
	CameraManager* m_CameraManager;
	EnemyManager* m_EnemyManager;
	GUIManager* m_GUIManager;
	AudioManager* m_AudioManager;
	GameObjectManager* m_GameObjectManager;
	MapBuilder* m_MapBuilder;
	bool m_Playing;
	bool m_Debug;
};