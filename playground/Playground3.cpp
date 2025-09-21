#include "Playground3.h"

#include "raymath.h"
#include "utility/Math.h"

namespace
{
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
            // WallForce();

            const bool contactGround = m_position.y + GetRadius() >= GetScreenHeight() - 1.0f;
            if (contactGround) {
                const float c = 0.1f;
                const float normal = m_mass * 9.8f;
                const float frictionSize = c * normal;
                Vector2 friction = Vector2Normalize(m_velocity) * -1.0f * frictionSize;
                ApplyForce(friction);
            }


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
            DrawCircleLinesV(m_position, GetRadius(), BLACK);
        }

        void ApplyForce(const Vector2& force)
        {
            m_acceleration += force / m_mass;
        }

        void WallBounce()
        {
            if (m_position.x + GetRadius() > GetScreenWidth()) {
                m_position.x = GetScreenWidth() - GetRadius();
                m_velocity.x *= -0.9f;
            }
            else if (m_position.x - GetRadius() < 0.0f) {
                m_position.x = GetRadius();
                m_velocity.x *= -0.9f;
            }
            if (m_position.y + GetRadius() > GetScreenHeight()) {
                m_position.y = GetScreenHeight() - GetRadius();
                m_velocity.y *= -0.9f;
            }
            else if (m_position.y - GetRadius() < 0.0f) {
                m_position.y = GetRadius();
                m_velocity.y *= -0.9f;
            }
        }

        void WallForce()
        {
            const float leftDistance = m_position.x - GetRadius();
            const float rightDistance = GetScreenWidth() - m_position.x - GetRadius();
            const float topDistance = m_position.y - GetRadius();
            const float bottomDistance = GetScreenHeight() - m_position.y - GetRadius();

            const float wallForceDistance = 64.0f;
            const float wallForce = 100.0f * m_mass;
            // wall force, closer, more force.
            if (leftDistance < wallForceDistance) {
                float wallForceFactor = 1.0f - leftDistance / wallForceDistance;
                Vector2 force = Vector2(wallForce * wallForceFactor, 0.0f);
                ApplyForce(force);
            }
            if (rightDistance < wallForceDistance) {
                float wallForceFactor = 1.0f - rightDistance / wallForceDistance;
                Vector2 force = Vector2(-wallForce * wallForceFactor, 0.0f);
                ApplyForce(force);
            }
            if (topDistance < wallForceDistance) {
                float wallForceFactor = 1.0f - topDistance / wallForceDistance;
                Vector2 force = Vector2(0.0f, wallForce * wallForceFactor);
                ApplyForce(force);
            }
            if (bottomDistance < wallForceDistance) {
                float wallForceFactor = 1.0f - bottomDistance / wallForceDistance;
                Vector2 force = Vector2(0.0f, -wallForce * wallForceFactor);
                ApplyForce(force);
            }
        }

        float GetRadius() const
        {
            return m_mass * 4.0f;
        }

        float m_mass = 1.0f;
        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
    };

    class Liquid
    {
    public:
        Liquid() = default;

        Liquid(Vector2 position, Vector2 size, float c)
        {
            m_rect.x = position.x - size.x * 0.5f;
            m_rect.y = position.y - size.y * 0.5f;
            m_rect.width = size.x;
            m_rect.height = size.y;
            m_c = c;
        }

        bool IsContract(const Body& mover)
        {
            return CheckCollisionCircleRec(mover.m_position, mover.GetRadius(), m_rect);
        }

        Vector2 CalculateDragForce(const Body& mover)
        {
            const float speed = Vector2Length(mover.m_velocity);
            const float dragMagnitude = m_c * speed * speed;
            Vector2 dragForce = Vector2Negate(Vector2Normalize(mover.m_velocity)) * dragMagnitude;
            return dragForce;
        }

        void Draw() const
        {
            DrawRectangleLinesEx(m_rect, 1.0f, BLUE);
        }

        Rectangle m_rect = {0.0f, 0.0f, 100.0f, 100.0f};
        float m_c = 0.1f;
    };

    std::vector<Body> movers;
    Liquid liquid;
    Vector2 gravity;
} // namespace

namespace playground
{
    CPlayground3::CPlayground3(IGame& game) : m_game(game)
    {
    }

    void CPlayground3::Init()
    {
        const int32_t windowWidth = m_game.GetWindowWidth();
        const int32_t windowHeight = m_game.GetWindowHeight();
        const auto& center = m_game.GetWindowCenterPosition();

        for (int32_t i = 0; i < 100; ++i) {
            Vector2 pos = Vector2(GetRandomValue(0, windowWidth), GetRandomValue(0, windowHeight));
            float mass = 1.0f + math::GetRandomValue() * 4.0f;
            movers.emplace_back(pos, mass);
        }

        // constexpr float gravityConst = 9.8f;
        constexpr float gravityConst = 98.0f;
        gravity = {0.0f, gravityConst};

        constexpr float c = 0.5f;
        // constexpr float c = 0.9f;
        liquid = Liquid(center, {100.0f, 100.0f}, c);
    }

    void CPlayground3::DeInit()
    {
    }

    void CPlayground3::Update()
    {
        for (auto& mover : movers) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 wind = Vector2(100.0f, 0.0f);
                mover.ApplyForce(wind);
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                const float distanceToMouse = Vector2Distance(mover.m_position, GetMousePosition());
                if (distanceToMouse < 64.0f) {
                    Vector2 toMouseForce = Vector2Normalize(GetMousePosition() - mover.m_position) * 1000.0f;
                    mover.ApplyForce(toMouseForce * mover.m_mass);
                }
            }
            if (liquid.IsContract(mover)) {
                Vector2 dragForce = liquid.CalculateDragForce(mover);
                mover.ApplyForce(dragForce);
            }
            mover.ApplyForce(gravity * mover.m_mass);
            mover.Update();
        }
    }

    void CPlayground3::DrawWorld()
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            DrawCircleLinesV(GetMousePosition(), 64.0f, GREEN);
        }
        for (const auto& mover : movers) {
            mover.Draw();
        }
        liquid.Draw();

        DrawCircleLinesV(movers[0].m_position, 8.0f, RED);
    }

    void CPlayground3::DrawUi()
    {
    }

    std::string_view CPlayground3::GetName() const {
        return "Playground 3 - Forces and Motion";
    }
} // namespace playground
