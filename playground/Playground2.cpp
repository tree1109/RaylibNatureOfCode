#include "Playground2.h"

#include "raymath.h"
#include "utility/Math.h"

namespace
{
    class Body
    {
    public:
        Body()
        {
        }

        void Update()
        {
            // Control.
            if (false) {
                if (IsKeyDown(KEY_W)) {
                    m_acceleration.y -= 100.0f * GetFrameTime();
                }
                if (IsKeyDown(KEY_S)) {
                    m_acceleration.y += 100.0f * GetFrameTime();
                }
                if (IsKeyDown(KEY_A)) {
                    m_acceleration.x -= 100.0f * GetFrameTime();
                }
                if (IsKeyDown(KEY_D)) {
                    m_acceleration.x += 100.0f * GetFrameTime();
                }
            }

            // Random.
            if (false) {
                m_acceleration = math::GetRandomDirection() * GetRandomValue(0, 1000);
            }

            // Towards the mouse.
            if (false) {
                auto mouseVector = GetMousePosition() - m_position;
                m_acceleration = mouseVector / (1.0f + Vector2Length(mouseVector));
                m_acceleration *= 1000.0f;
            }

            // Limit the velocity.
            if (Vector2Length(m_velocity) >= 500.0f) {
                m_velocity = Vector2Normalize(m_velocity) * 500.0f;
            }

            m_velocity += m_acceleration * GetFrameTime();
            m_position += m_velocity * GetFrameTime();
            // CheckEdge();
        }

        void Draw() const
        {
            DrawCircleV(m_position, 4.0f, BLACK);
        }

        void CheckEdge()
        {
            if (m_position.x > GetScreenWidth()) {
                m_position.x -= GetScreenWidth();
            }
            else if (m_position.x < 0.0f) {
                m_position.x += GetScreenWidth();
            }
            if (m_position.y > GetScreenHeight()) {
                m_position.y -= GetScreenHeight();
            }
            else if (m_position.y < 0.0f) {
                m_position.y += GetScreenHeight();
            }
        }

        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
    };

    std::array<Body, 100> movers;
} // namespace

namespace playground
{
    CPlayground2::CPlayground2(IGame& game) : m_game(game)
    {
    }

    void CPlayground2::Init()
    {
        const auto& center = m_game.GetWindowCenterPosition();
        for (auto& mover : movers) {
            mover.m_position = center;
        }
    }

    void CPlayground2::DeInit()
    {
    }

    void CPlayground2::Update()
    {
        if (true) {
            movers[0].m_acceleration = GetMousePosition() - movers[0].m_position;
            for (int32_t i = 0; i < movers.size() - 1; ++i) {
                auto acc = movers[i].m_position - movers[i + 1].m_position;
                float len = Vector2Length(acc);
                movers[i + 1].m_acceleration = acc * len * 10.0f;
                // movers[i + 1].m_acceleration = acc * 100.0f;
            }
        }

        for (auto& mover : movers) {
            mover.Update();
        }
    }

    void CPlayground2::DrawWorld()
    {
        for (const auto& mover : movers) {
            mover.Draw();
        }

        DrawCircleLinesV(movers[0].m_position, 8.0f, RED);
        for (int32_t i = 0; i < movers.size() - 1; ++i) {
            // DrawLineV(movers[i].m_position, movers[i + 1].m_position, BLACK);
            DrawLineEx(movers[i].m_position, movers[i + 1].m_position, 4.0f, BLACK);
        }
    }

    void CPlayground2::DrawUi()
    {
    }

    std::string_view CPlayground2::GetName() const
    {
        return "Playground 2 - Vectors and Acceleration";
    }
} // namespace playground
