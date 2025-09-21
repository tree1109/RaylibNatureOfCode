#include "Playground7.h"

#include "raymath.h"
#include "utility/Math.h"

namespace
{
    class SpaceShip
    {
    public:
        SpaceShip() = default;

        SpaceShip(Vector2 position) : m_position(position)
        {
        }

        void Update()
        {
            m_isThrusting = Vector2LengthSqr(m_acceleration) > 0.0f;

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
            // Ship body.
            constexpr float WIDTH = 50.0f;
            constexpr float HEIGHT = 60.0f;
            Vector2 front = Vector2{HEIGHT * 0.5f, 0.0f};
            Vector2 back = Vector2{-HEIGHT * 0.5f, -WIDTH * 0.5f};
            Vector2 back2 = Vector2{-HEIGHT * 0.5f, WIDTH * 0.5f};
            front = Vector2Rotate(front, m_angle);
            back = Vector2Rotate(back, m_angle);
            back2 = Vector2Rotate(back2, m_angle);
            front += m_position;
            back += m_position;
            back2 += m_position;
            DrawTriangleLines(front, back, back2, BLACK);

            // Thrust.
            if (m_isThrusting) {
                constexpr float RADIUS = 20.0f;
                Vector2 thrust = Vector2{-HEIGHT * 0.5f, 0.0f};
                thrust = Vector2Rotate(thrust, m_angle);
                thrust += m_position;
                DrawCircleLinesV(thrust, RADIUS, ORANGE);
            }
        }

        void ApplyForce(const Vector2& force)
        {
            m_acceleration += force;
        }

        void ApplyTorque(float torque)
        {
            m_angularAcceleration += torque;
        }

        Vector2 m_position = Vector2Zeros;
        Vector2 m_velocity = Vector2Zeros;
        Vector2 m_acceleration = Vector2Zeros;
        float m_angle = 0.0f;
        float m_angularVelocity = 0.0f;
        float m_angularAcceleration = 0.0f;

        bool m_isThrusting = false;
    };

    SpaceShip mover;
    Vector2 ballPosition;
} // namespace

namespace playground
{
    CPlayground7::CPlayground7(IGame& game) : m_game(game)
    {
    }

    void CPlayground7::Init()
    {
        m_game.AddKeyboardControlsInfo("Mouse - Ship direction")
            .AddKeyboardControlsInfo("Left Click - Thrust");

        const auto& center = m_game.GetWindowCenterPosition();
        mover = SpaceShip(center);
        ballPosition = center;
    }

    void CPlayground7::DeInit()
    {
    }

    void CPlayground7::Update()
    {
        const double time = m_game.GetTime();
        const auto& center = m_game.GetWindowCenterPosition();

        const Vector2 mouseDirection = GetMousePosition() - mover.m_position;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mover.ApplyForce(Vector2Normalize(mouseDirection) * 100.0f);
        }
        mover.m_angle = Vector2Angle(Vector2UnitX, mouseDirection);
        mover.Update();

        // Ball.
        constexpr float AMPLITUDE = 100.0f;
        constexpr float PERIOD = 3.0f;
        Vector2 newPos = {AMPLITUDE * std::cos(2.0f * PI * static_cast<float>(time) / PERIOD), 0.0f};
        newPos += center;
        ballPosition = newPos;
    }

    void CPlayground7::DrawWorld()
    {
        mover.Draw();

        // Ball.
        DrawCircleLinesV(ballPosition, 10.0f, RED);
    }

    void CPlayground7::DrawUi()
    {
    }

    std::string_view CPlayground7::GetName() const
    {
        return "Playground 7";
    }
} // namespace playground
