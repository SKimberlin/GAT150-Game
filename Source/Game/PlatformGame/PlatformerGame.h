#pragma once
#include "Framework/Event/EventManager.h"
#include "Framework/Game.h"
#include "Renderer/Text.h"

#define ADD_ACTOR(actor) m_scene->Add(std::move(actor))


class PlatformerGame : public kiko::Game, kiko::IEventListener
{
public:
	enum eState
	{
		Load,
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOverStart,
		GameOver
	};
public:


	// Inherited via Game
	virtual bool Initialize() override;

	virtual void Shutdown() override;

	virtual void Update(float dt) override;

	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	void LoadLevel();

	void OnCoinPickup(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);
	void OnLevelComplete(const kiko::Event& event);

	void SetSpawnTime(float spawnTime) { m_spawnTime = spawnTime; }

private:
	eState m_state = eState::Load;
	float m_spawnTimer = 0.0f;
	float m_spawnTime = 3.0f;
	float m_stateTimer = 0;
	int m_levelCount = 1;
	


};