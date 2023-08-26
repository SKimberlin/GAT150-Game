#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Core/Math/MathUtils.h"


#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

bool SpaceGame::Initialize()
{
    // load font

    m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Starlight.ttf", 50));
    m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

    m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Starlight.ttf", 50));
    m_titleText->Create(kiko::g_renderer, "EMU", kiko::Color{ 1, 1, 1, 1 });

    m_gameoverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Starlight.ttf", 50));
    m_gameoverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });

    m_hardmodeText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Starlight.ttf", 50));
   

    // load audio
    kiko::g_audioSystem.AddAudio("Laser_Shoot", "Laser_Shoot.wav");
    kiko::g_audioSystem.AddAudio("EnemyDeath", "EnemyDeath.wav");
    kiko::g_audioSystem.AddAudio("FireBall", "FireBall.wav");
    kiko::g_audioSystem.AddAudio("MuchHigher", "MuchHigher.wav");
    kiko::g_audioSystem.PlayOneShot("MuchHigher", true);

    m_scene = std::make_unique<kiko::Scene>();
    m_scene->Load("Scenes/SpacegameScene.json");
    m_scene->Initialize();

    // add events
    EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnAddPoints);
    EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnPlayerDead);

    //kiko::EventManager::Instance().Subscribe("OnAddPoints", this, std::bind(&SpaceGame::OnAddPoints, this, std::placeholders::_1));
    //kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&SpaceGame::OnPlayerDead, this, std::placeholders::_1));


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
            m_scene->GetActorByName("Title")->active = true;
            if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
            {
                m_state = eState::StartGame;
            }
            if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_0) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_0))
            {
                SetHardmode();
            }
            m_scene->GetActorByName("Background")->active = false;

            break;
        case SpaceGame::StartGame:
            m_score = 0;
            m_lives = 3;
            m_state = eState::StartLevel;
            if (IsHardmode()) SetSpawnTime(1.0f);
            break;
        case SpaceGame::StartLevel:
            m_scene->RemoveAll();
            m_scene->GetActorByName("Background")->active = true;
        {
            // create player
            auto player = INSTANTIATE(Player, "Player");
            player->Initialize();
            m_scene->Add(std::move(player));
            

            

            // create components
            //auto renderComponent = CREATE_CLASS(SpriteRenderComponent); //std::make_unique<kiko::SpriteComponent>();
            //renderComponent->m_texture = GET_RESOURCE(kiko::Texture, "Main Ship - Base - Full health.png", kiko::g_renderer);
            //player->AddComponent(std::move(renderComponent));

            //auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent);
            //physicsComponent->m_damping = 0.9f;
            //player->AddComponent(std::move(physicsComponent));

            //auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
            //collisionComponent->m_radius = 20.0f;
            //player->AddComponent(std::move(collisionComponent));

            //player->Initialize();
            //m_scene->Add(std::move(player));
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
                    
                }
                else
                {
                    
                }
                
                m_spawnTimer = 0;
                if (IsHardmode())
                {
                    /*kiko::Transform transform{ kiko::vec2(kiko::randomf(799.0f), 0.0f), kiko::DegreesToRadians(kiko::randomf(360.0f)), 0.5f };
                    std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform );
                    weapon->tag = "EnemyBullet";
                    m_scene->Add(std::move(weapon));*/
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
    m_scene->Draw(renderer);
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
    kiko::g_particleSystem.Draw(renderer);
}

void SpaceGame::OnAddPoints(const kiko::Event& event)
{
    m_score += std::get<int>(event.data);

}

void SpaceGame::OnPlayerDead(const kiko::Event& event)
{
    m_lives--;
    m_state = eState::PlayerDeadStart;
}
