#include "Playground4.h"

#include "raymath.h"
#include "utility/Math.h"

namespace
{
    float GetRadius(float mass)
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

    class Attractor
    {
    public:
        Attractor() = default;

        Attractor(Vector2 position, float mass) : m_position(position), m_mass(mass)
        {
        }

        void Draw() const
        {
            DrawCircleLinesV(m_position, GetRadius(m_mass), GREEN);
        }

        bool IsContract(const Vector2& pos) const
        {
            return CheckCollisionPointCircle(pos, m_position, GetRadius(m_mass));
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

        float m_mass = 10.0f;
        Vector2 m_position = Vector2Zeros;
    };

    std::vector<Body> movers;
    Attractor attractor;
} // namespace

namespace playground
{
    CPlayground4::CPlayground4(IGame& game) : m_game(game)
    {
    }

    void CPlayground4::Init()
    {
        const float width = m_game.GetWindowWidth();
        const float height = m_game.GetWindowHeight();
        const auto& center = m_game.GetWindowCenterPosition();

        for (int32_t i = 0; i < 100; ++i) {
            Vector2 pos = Vector2(GetRandomValue(0, width), GetRandomValue(0, height));
            float mass = 1.0f + math::GetRandomValue() * 100.0f;
            movers.emplace_back(pos, mass);
        }
        attractor = Attractor(center, 10000.0f);
    }

    void CPlayground4::DeInit()
    {
    }

    void CPlayground4::Update()
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && attractor.IsContract(GetMousePosition())) {
            attractor.m_position = GetMousePosition();
        }
        for (auto& mover : movers) {
            Vector2 force = attractor.GetAttractForce(mover);
            mover.ApplyForce(force);
            mover.Update();
        }
    }

    void CPlayground4::DrawWorld()
    {
        attractor.Draw();
        for (auto& mover : movers) {
            mover.Draw();
        }
    }

    void CPlayground4::DrawUi()
    {
    }

    std::string_view CPlayground4::GetName() const
    {
        return "Playground 4";
    }
} // namespace playground
