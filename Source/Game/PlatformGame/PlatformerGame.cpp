#include "PlatformerGame.h"
#include "Core/Math/MathUtils.h"


#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

bool PlatformerGame::Initialize()
{

    // load audio
    
    // add events
    EVENT_SUBSCRIBE("OnPlayerDead", PlatformerGame::OnPlayerDead);
    EVENT_SUBSCRIBE("OnLevelComplete", PlatformerGame::OnLevelComplete);
    EVENT_SUBSCRIBE("OnCoinPickup", PlatformerGame::OnCoinPickup);

    //kiko::EventManager::Instance().Subscribe("OnAddPoints", this, std::bind(&PlatformerGame::OnAddPoints, this, std::placeholders::_1));
    //kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&PlatformerGame::OnPlayerDead, this, std::placeholders::_1));
    m_scene = std::make_unique<kiko::Scene>();
    

    

    return true;
}

void PlatformerGame::Shutdown()
{
}

void PlatformerGame::Update(float dt)
{
    switch (m_state)
    {
    case PlatformerGame::Load:
        m_scene->Load("Scenes/PlatformerScene.json");
        m_scene->Load("Scenes/prototypes.json");
        m_scene->Initialize();
        m_state = eState::Title;
        break;
    case PlatformerGame::Title:
    {
        m_scene->GetActorByName("Title")->active = true;
        m_scene->GetActorByName("Lives")->active = false;
        
        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
        {
            m_state = eState::StartGame;
        }
    }
        break;
    case PlatformerGame::StartGame:
        m_state = eState::StartLevel;

        break;
    case PlatformerGame::StartLevel:
        m_scene->GetActorByName("Lives")->active = true;
        
        
        LoadLevel();
        
        m_state = eState::Game;

        break;
    case PlatformerGame::Game:
    {
        break;
    }
    case PlatformerGame::PlayerDeadStart:
        m_scene->RemoveAll();
        if (m_lives <= 0)
        {
            m_state = eState::GameOverStart;
        }
        else 
        {
            m_state = eState::PlayerDead;
        }

        break;
    case PlatformerGame::PlayerDead:
        m_state = eState::StartLevel;
        break;
    case PlatformerGame::GameOverStart:
        m_state = eState::GameOver;
        break;
    case PlatformerGame::GameOver:
        m_lives = 3;
        m_state = eState::Title;
        break;
    default:
        break;
    }

    m_scene->Update(dt);
    kiko::g_particleSystem.Update(dt);
}

void PlatformerGame::LoadLevel()
{
    m_scene->RemoveAll();
    switch (m_levelCount)
    {
    case 1:
        m_scene->Load("Scenes/Level1.json");
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 100, 40 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 100, 420 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 1600, 490 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        break;
    case 2:
        m_scene->Load("Scenes/Level2.json");
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 1600, 90 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 60, 420 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        {
            auto actor = INSTANTIATE(Actor, "Coin");
            actor->transform.position = { 1600, 740 };
            actor->Initialize();
            ADD_ACTOR(actor);
        }
        break;
    case 3:
        m_scene->Load("Scenes/Level3.json");
        break;
    case 4:
        m_scene->Load("Scenes/Level4.json");
        break;
    case 5:
        m_scene->Load("Scenes/Level5.json");
        break;
    default:
        m_scene->Load("Scenes/Level1.json");
        break;

    }
    m_scene->Initialize();
    {
        auto actor = INSTANTIATE(Actor, "Player");
        actor->transform.position = { 100, 900 };
        actor->m_game = this;
        actor->Initialize();
        ADD_ACTOR(actor);
    }
    {
        auto actor = INSTANTIATE(Actor, "Enemy");
        actor->transform.position = { 800, 900 };
        actor->Initialize();
        ADD_ACTOR(actor);
    }
    m_coinsNeeded = 3;
}

void PlatformerGame::Draw(kiko::Renderer& renderer)
{
    m_scene->Draw(renderer);
    kiko::g_particleSystem.Draw(renderer);
}

void PlatformerGame::OnCoinPickup(const kiko::Event& event)
{
    m_coinsNeeded--;
}

void PlatformerGame::OnPlayerDead(const kiko::Event& event)
{
    m_lives--;
    m_state = eState::PlayerDeadStart;
}

void PlatformerGame::OnLevelComplete(const kiko::Event& event)
{
    if (m_levelCount == 5)
    {

    }
    else
    {
        m_levelCount++;
        m_state = eState::StartLevel;
    }
}
