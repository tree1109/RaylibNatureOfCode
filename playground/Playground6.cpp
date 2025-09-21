#include "Playground6.h"

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
            // Limit the velocity.
            if (Vector2Length(m_velocity) >= 500.0f) {
                m_velocity = Vector2Normalize(m_velocity) * 500.0f;
            }

            m_velocity += m_acceleration * GetFrameTime();
            m_position += m_velocity * GetFrameTime();

            m_angularVelocity += m_angularAcceleration * GetFrameTime();
            m_angle += m_angularVelocity * GetFrameTime();

            m_acceleration = Vector2Zeros;
            m_angularAcceleration = 0.0f;
        }

        void Draw() const
        {
            const float radius = GetRadius(m_mass);
            DrawCircleLinesV(m_position, radius, BLACK);

            Vector2 end = Vector2{radius, 0.0f};
            end = Vector2Rotate(end, m_angle);
            end += m_position;
            DrawLineV(m_position, end, BLACK);
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

        float m_mass = 1.0f;
        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
        float m_angle = 0.0f;
        float m_angularVelocity = 0.0f;
        float m_angularAcceleration = 0.0f;
    };

    std::vector<Body> movers;
} // namespace

namespace playground
{
    CPlayground6::CPlayground6(IGame& game) : m_game(game)
    {
    }

    void CPlayground6::Init()
    {
        const float width = m_game.GetWindowWidth();
        const float height = m_game.GetWindowHeight();

        for (int32_t i = 0; i < 100; ++i) {
            Vector2 pos = Vector2(GetRandomValue(0, width), GetRandomValue(0, height));
            movers.emplace_back(pos, GetRandomValue(10, 100));
        }
    }

    void CPlayground6::DeInit()
    {
    }

    void CPlayground6::Update()
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
            mover.m_angularVelocity = mover.m_acceleration.x;

            mover.Update();
        }
    }

    void CPlayground6::DrawWorld()
    {
        for (auto& mover : movers) {
            mover.Draw();
        }
    }

    void CPlayground6::DrawUi()
    {
    }

    std::string_view CPlayground6::GetName() const
    {
        return "Playground 6";
    }
} // namespace playground
