#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Core/MathUtils.h"

#include "Framework/Scene.h"
#include "Renderer/ParticleSystem.h"
#include "Framework/Emitter.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"

bool SpaceGame::Initialize()
{
    // load font
    m_font = std::make_shared<kiko::Font>("Starlight.ttf", 50);
    m_scoreText = std::make_unique<kiko::Text>(m_font);
    m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

    m_titleText = std::make_unique<kiko::Text>(m_font);
    m_titleText->Create(kiko::g_renderer, "EMU", kiko::Color{ 1, 1, 1, 1 });

    m_gameoverText = std::make_unique<kiko::Text>(m_font);
    m_gameoverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });

    m_hardmodeText = std::make_unique<kiko::Text>(m_font);
   

    // load audio
    kiko::g_audioSystem.AddAudio("Laser_Shoot", "Laser_Shoot.wav");
    kiko::g_audioSystem.AddAudio("EnemyDeath", "EnemyDeath.wav");
    kiko::g_audioSystem.AddAudio("FireBall", "FireBall.wav");
    kiko::g_audioSystem.AddAudio("MuchHigher", "MuchHigher.wav");
    kiko::g_audioSystem.PlayOneShot("MuchHigher", true);

    m_scene = std::make_unique<kiko::Scene>();


	return true;
}

void SpaceGame::Shutdown()
{
}

void SpaceGame::Update(float dt)
{
	switch (m_state)
    {
	    case SpaceGame::Title:
            if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
            {
                m_state = eState::StartGame;
            }
            if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_0))
            {
                SetHardmode();
            }

            break;
        case SpaceGame::StartGame:
            m_score = 0;
            m_lives = 3;
            m_state = eState::StartLevel;
            if (IsHardmode()) SetSpawnTime(1.0f);
            break;
        case SpaceGame::StartLevel:
            m_scene->RemoveAll();
        {
            std::unique_ptr<Player> player = std::make_unique<Player>(15.0f, kiko::DegreesToRadians(270), kiko::Transform{ {400, 300 }, 0, 3 }, kiko::g_manager.Get("PlayerShip.txt"));
            player->m_tag = "Player";
            player->m_game = this;
            player->SetDamping(0.9f);
            m_scene->Add(std::move(player));
        }
            m_state = eState::Game;
            break;
        case SpaceGame::Game:
        {
            m_spawnTimer += dt;
            if (m_spawnTimer >= m_spawnTime)
            {
                if ((kiko::randomf(1.0f) > 0.8f) && (m_score > 100))
                {
                    std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(250.0f, 200.0f), kiko::DegreesToRadians(270), kiko::Transform{ { kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight()) }, kiko::randomf(kiko::TwoPi), 2 }, kiko::g_manager.Get("SpecialEnemyShip.txt"));
                    enemy->SetFireTime(0.5f);
                    enemy->m_tag = "SpecialEnemy";
                    enemy->m_game = this;
                    m_scene->Add(std::move(enemy));
                }
                else
                {
                    std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(150.0f, 100.0f), kiko::DegreesToRadians(180), kiko::Transform{ { kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight()) }, kiko::randomf(kiko::TwoPi), 2 }, kiko::g_manager.Get("EnemyShip.txt"));
                    enemy->m_tag = "Enemy";
                    enemy->m_game = this;
                    m_scene->Add(std::move(enemy));
                }
                
                m_spawnTimer = 0;
                if (IsHardmode())
                {
                    kiko::Transform transform{ kiko::vec2(kiko::randomf(799.0f), 0.0f), kiko::DegreesToRadians(kiko::randomf(360.0f)), 0.5f };
                    std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, kiko::g_manager.Get("EnemyBullet.txt"));
                    weapon->m_tag = "EnemyBullet";
                    m_scene->Add(std::move(weapon));
                }
            }
            
            break;
        }
        case SpaceGame::PlayerDeadStart:
            m_stateTimer = 3;
            if (m_lives == 0) m_state = eState::GameOver;
            else m_state = eState::PlayerDead;
            break;
        case SpaceGame::PlayerDead:
            m_stateTimer -= dt;
            if (m_stateTimer <= 0) 
            {
                m_state = eState::StartLevel;
            }
            
            break;
        case SpaceGame::GameOver:
            m_stateTimer -= dt;
            if (m_stateTimer <= 0)
            {
                m_state = eState::Title;
            }
            break;
        default:
            break;
    }

    m_scoreText->Create(kiko::g_renderer, "Score " + std::to_string(m_score), {1, 0, 1, 1});
    m_scene->Update(dt);
    kiko::g_particleSystem.Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer)
{
    if (m_state == eState::Title)
    {
        m_titleText->Draw(renderer, 400, 300);
        IsHardmode() ? m_hardmodeText->Create(kiko::g_renderer, "HARDMODE ON", kiko::Color{ 1,0,0,1 }) : m_hardmodeText->Create(kiko::g_renderer, "HARDMODE OFF", kiko::Color{ 1,0,0,1 });
        m_hardmodeText->Draw(renderer, 500, 20);
    }
    if (m_state == eState::GameOver)
    {
        m_gameoverText->Draw(renderer, 400, 300);
    }
    m_scoreText->Draw(renderer, 40, 20);
    m_scene->Draw(renderer);
    kiko::g_particleSystem.Draw(renderer);
}
