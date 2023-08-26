#include "PlatformerGame.h"
#include "Core/Math/MathUtils.h"


#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

#include "Renderer/Renderer.h"

bool PlatformerGame::Initialize()
{

    // load audio
    kiko::g_audioSystem.AddAudio("Laser_Shoot", "Laser_Shoot.wav");
    kiko::g_audioSystem.AddAudio("EnemyDeath", "EnemyDeath.wav");
    kiko::g_audioSystem.AddAudio("FireBall", "FireBall.wav");
    kiko::g_audioSystem.AddAudio("MuchHigher", "MuchHigher.wav");
    kiko::g_audioSystem.PlayOneShot("MuchHigher", true);

    m_scene = std::make_unique<kiko::Scene>();
    m_scene->Load("Scenes/PlatformerGameScene.json");
    m_scene->Initialize();

    // add events
    EVENT_SUBSCRIBE("OnAddPoints", PlatformerGame::OnAddPoints);
    EVENT_SUBSCRIBE("OnAddPoints", PlatformerGame::OnPlayerDead);

    //kiko::EventManager::Instance().Subscribe("OnAddPoints", this, std::bind(&PlatformerGame::OnAddPoints, this, std::placeholders::_1));
    //kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&PlatformerGame::OnPlayerDead, this, std::placeholders::_1));
    m_scene = std::make_unique<kiko::Scene>();
    m_scene->Load("Scenes/PlatformerScene.json");
    m_scene->Initialize();

    return true;
}

void PlatformerGame::Shutdown()
{
}

void PlatformerGame::Update(float dt)
{
    switch (m_state)
    {
    case PlatformerGame::Title:
        m_scene->GetActorByName("Title")->active = true;
        m_scene->GetActorByName("Background")->active = false;

        break;
    case PlatformerGame::StartGame:

        break;
    case PlatformerGame::StartLevel:

        break;
    case PlatformerGame::Game:
    {
        break;
    }
    case PlatformerGame::PlayerDeadStart:

        break;
    case PlatformerGame::PlayerDead:

        break;
    case PlatformerGame::GameOver:

        break;
    default:
        break;
    }

    m_scene->Update(dt);
    kiko::g_particleSystem.Update(dt);
}

void PlatformerGame::Draw(kiko::Renderer& renderer)
{
    m_scene->Draw(renderer);
    kiko::g_particleSystem.Draw(renderer);
}

void PlatformerGame::OnAddPoints(const kiko::Event& event)
{
    m_score += std::get<int>(event.data);
}

void PlatformerGame::OnPlayerDead(const kiko::Event& event)
{
    m_lives--;
    m_state = eState::PlayerDeadStart;
}
