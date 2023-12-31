#pragma once
#include "Framework/Event/EventManager.h"
#include "Framework/Game.h"
#include "Renderer/Text.h"


class SpaceGame : public kiko::Game, kiko::IEventListener
{
public:
	enum eState
	{
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

	void OnAddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);

	bool IsHardmode() { return m_hardmode; }
	void SetHardmode() { m_hardmode ? m_hardmode = false : m_hardmode = true; }

	void SetSpawnTime(float spawnTime) { m_spawnTime = spawnTime; }

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0.0f;
	float m_spawnTime = 3.0f;
	float m_stateTimer = 0;
	bool m_hardmode = false;

	std::shared_ptr<kiko::Font> m_font;
	std::shared_ptr<kiko::Text> m_scoreText;
	std::shared_ptr<kiko::Text> m_titleText;
	std::shared_ptr<kiko::Text> m_gameoverText;
	std::shared_ptr<kiko::Text> m_hardmodeText;
	std::shared_ptr<kiko::Text> m_highscoreText;


};