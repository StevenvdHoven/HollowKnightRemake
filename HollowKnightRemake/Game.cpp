#include "pch.h"
#include "Game.h"
#include "Texture.h"
#include "MapBuilder.h"
#include "GUI.h"
#include "LevelManager.h"
#include "GameObject.h"
#include "SpriteManager.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "PhysicsEngine.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "GUIManager.h"
#include "AudioManager.h"
#include "PlayerDataManager.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_TextureManager{ new TextureManager() },
	m_LevelManager{ new LevelManager() },
	m_SpriteManager{ new SpriteManager() },
	m_PhysicsEngine{ new PhysicsEngine() },
	m_CameraManager{ new CameraManager(GetViewPort())},
	m_EnemyManager{ new EnemyManager()},
	m_GUIManager{new GUIManager()},
	m_AudioManager{ new AudioManager()},
	m_GameObjectManager{ new GameObjectManager(m_LevelManager,m_SpriteManager,m_PhysicsEngine,m_TextureManager,m_CameraManager,m_AudioManager,m_GUIManager) },
	m_MapBuilder{ new MapBuilder(GetViewPort(), m_TextureManager,m_GUIManager) },
	m_Playing{ true },
	m_Debug{ false }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_LevelManager->CreateLevels(m_MapBuilder, m_GameObjectManager, m_EnemyManager);
	m_LevelManager->LoadLevel("menu", "", .1f);
	m_MapBuilder->SetHideUI(m_Playing);
}

void Game::Cleanup()
{
	delete m_GameObjectManager;
	delete m_TextureManager;
	delete m_LevelManager;
	delete m_SpriteManager;
	delete m_PhysicsEngine;
	delete m_CameraManager;
	delete m_EnemyManager;
	delete m_MapBuilder;
	delete m_GUIManager;
	delete m_AudioManager;
}

void Game::Update(float elapsedSec)
{
	if (m_Playing) 
	{
		m_LevelManager->Update(elapsedSec);
		m_PhysicsEngine->Update(elapsedSec);
		m_GameObjectManager->UpdateAll(elapsedSec);
	}
	else 
	{
		m_MapBuilder->Update(elapsedSec);
	}

	m_GUIManager->UpdateUI();
}

void Game::Draw() const
{
	Transform screenScale{ Vector2f{},0,Vector2f{GetViewPort().width / 1920.f, GetViewPort().height / 1080.f} };
	screenScale.ApplyTransformation();
	
	ClearBackground();

	if (m_Playing) 
	{

		Transform trans{ m_CameraManager->GetTransform() };

		const Vector2f scale{ trans.Scale };
		trans.Position = trans.Position * scale.x + Vector2f{1920*0.5f,1080 *0.5f};
		
		
		//Transform scaleTrans;
		//scaleTrans.Scale = scale;

		//scaleTrans.ApplyTransformation();
		trans.ApplyTransformation();
		
		m_SpriteManager->Draw(m_CameraManager->GetCameraPosition());
		
		//m_PhysicsEngine->Draw();
		if(m_Debug) m_GameObjectManager->DrawAll();
		trans.ResetTransformation();
		m_LevelManager->Draw();
		//scaleTrans.ResetTransformation();
		
	}
	else 
	{
		m_MapBuilder->Draw();
	}
	m_GUIManager->DrawUI();
	screenScale.ResetTransformation();
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	m_GUIManager->OnKeyDownUI(e);

	if (e.keysym.scancode == SDL_SCANCODE_F1) 
	{
		m_Debug = !m_Debug;
	}

	if (!m_Playing) 
	{
		m_MapBuilder->OnKeyEventDown(e);
	}
	else 
	{
		m_GameObjectManager->KeyDownEventAll(e);
	}

}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	m_GUIManager->OnKeyUpUI(e);

	if (!m_Playing) 
	{
		m_MapBuilder->OnKeyEventUp(e);
	}
	else 
	{
		m_GameObjectManager->KeyUpEventAll(e);
	}

#if !Release 
	if (e.keysym.sym == SDLK_LCTRL)
	{

		m_Playing = !m_Playing;

		m_MapBuilder->SetHideUI(m_Playing);
		if (m_Playing) {
			m_LevelManager->CreateLevels(m_MapBuilder, m_GameObjectManager,m_EnemyManager);
			m_LevelManager->LoadLevel("menu","", .1f);
		}
		else 
		{
			m_GameObjectManager->Deload();
			m_GameObjectManager->GetPlayerDataManager()->SavePlayerData(PlayerData{});
			m_LevelManager->DeleteLevels();
			m_AudioManager->StopAll();
			m_Debug = false;
		}
	}
#endif
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	const Point2f mousePos{ Point2f(e.x,e.y) };

	if (!m_Playing) {
		m_MapBuilder->OnMouseMotion(mousePos);
	}
	else {
		m_GameObjectManager->MouseMotionEventAll(e);
	}

	m_GUIManager->SetMousePosition(mousePos);
}

void Game::ProcessMouseWheelEvent(const SDL_MouseMotionEvent& e)
{
	if (!m_Playing) {
		m_MapBuilder->OnScroll(e.yrel);
	}
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_Playing) 
	{
		m_MapBuilder->OnMouseClick(e.button == SDL_BUTTON_RIGHT);
	}
	else 
	{
		m_GameObjectManager->MouseDownEventAll(e);
	}

	m_GUIManager->OnClickDown();
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_Playing) 
	{
		m_MapBuilder->OnMouseClickUp();
	}
	else 
	{
		m_GameObjectManager->MouseUpEventAll(e);
	}

	m_GUIManager->OnClickUp();
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
