#include "Playground5.h"

#include "raymath.h"
#include "utility/Math.h"

namespace
{
    static float GetRadius(float mass)
    {
        // Volume as mass.
        return powf(mass / PI, 1.0f / 3.0f) * 2.0f;
    }

    class Body
    {
    public:
        Body() = default;

        Body(Vector2 position, float mass) : m_position(position), m_mass(mass)
        {
        }

        void Update()
        {
            WallBounce();

            // Limit the velocity.
            if (Vector2Length(m_velocity) >= 500.0f) {
                m_velocity = Vector2Normalize(m_velocity) * 500.0f;
            }

            m_velocity += m_acceleration * GetFrameTime();
            m_position += m_velocity * GetFrameTime();

            m_acceleration = Vector2Zeros;
        }

        void Draw() const
        {
            DrawCircleLinesV(m_position, GetRadius(m_mass), BLACK);
        }

        void ApplyForce(const Vector2& force)
        {
            m_acceleration += force / m_mass;
        }

        Vector2 GetAttractForce(const Body& mover) const
        {
            constexpr float G = 9.8f;

            // Calculate the force of attraction.
            Vector2 force = m_position - mover.m_position;
            const float distance = Clamp(Vector2Length(force), 4.0f, 4096.0f);
            force = Vector2Normalize(force);

            // Calculate the strength of the force.
            const float strength = G * m_mass * mover.m_mass / (distance * distance);
            force *= strength;

            return force;
        }

        void WallBounce()
        {
            const float radius = GetRadius(m_mass);

            if (m_position.x + radius > GetScreenWidth()) {
                m_position.x = GetScreenWidth() - radius;
                m_velocity.x *= -0.9f;
            }
            else if (m_position.x - radius < 0.0f) {
                m_position.x = radius;
                m_velocity.x *= -0.9f;
            }
            if (m_position.y + radius > GetScreenHeight()) {
                m_position.y = GetScreenHeight() - radius;
                m_velocity.y *= -0.9f;
            }
            else if (m_position.y - radius < 0.0f) {
                m_position.y = radius;
                m_velocity.y *= -0.9f;
            }
        }

        float m_mass = 1.0f;
        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
    };

    std::vector<Body> movers;
} // namespace

namespace playground
{
    CPlayground5::CPlayground5(IGame& game) : m_game(game)
    {
    }

    void CPlayground5::Init()
    {
        const float width = m_game.GetWindowWidth();
        const float height = m_game.GetWindowHeight();
        const auto& center = m_game.GetWindowCenterPosition();
        // Random.
        if (false) {
            for (int32_t i = 0; i < 100; ++i) {
                Vector2 pos = Vector2(GetRandomValue(0, width), GetRandomValue(0, height));
                movers.emplace_back(pos, GetRandomValue(10, 100));
            }
        }
        // Two body.
        if (false) {
            constexpr float offset = 10.0f;
            movers.emplace_back(center + Vector2{0.0f, offset}, 100.0f);
            movers.emplace_back(center + Vector2{0.0f, -offset}, 100.0f);
            movers[0].m_velocity = Vector2{5.0f, 0.0f};
            movers[1].m_velocity = Vector2{-5.0f, 0.0f};
        }
        // Galaxy.
        if (true) {
            for (int32_t i = 0; i < 100; ++i) {
                Vector2 pos =
                    Vector2Rotate(Vector2UnitX, math::GetRandomValue(0.0f, PI * 2.0f)) * GetRandomValue(10, 200);
                pos += center;
                movers.emplace_back(pos, GetRandomValue(10, 100));
                Vector2 direction = Vector2Rotate(center - pos, 90.0f * DEG2RAD);
                float distance = Vector2Length(center - pos);
                movers.back().m_velocity = direction * 10.0f / distance;
            }
        }
    }

    void CPlayground5::DeInit()
    {
    }

    void CPlayground5::Update()
    {
        for (auto& mover : movers) {
            for (auto& otherMover : movers) {
                if (&mover != &otherMover) {
                    Vector2 force = otherMover.GetAttractForce(mover);
                    mover.ApplyForce(force);
                }
            }
        }

        for (auto& mover : movers) {
            mover.Update();
        }
    }

    void CPlayground5::DrawWorld()
    {
        for (auto& mover : movers) {
            mover.Draw();
        }
    }

    void CPlayground5::DrawUi()
    {
    }

    std::string_view CPlayground5::GetName() const
    {
        return "Playground 5";
    }
} // namespace playground
