#include <raylib.h>
#include <raymath.h>
#include <array>
#include <numbers>
#include <cstdint>
#include <cmath>
#include <functional>
#include <vector>

#include "utility/Math.h"

#include "utility/PathFinder.h"

namespace
{
    class Mover
    {
    public:
        Mover(const Vector2 position)
            : Position(position)
        {
        }

        void Update()
        {

            if (IsStatic)
            {
                Velocity = Vector2Zeros;
                Acceleration = Vector2Zeros;
            }
            else
            {
                const float deltaTime = GetFrameTime();

                Velocity += Acceleration * deltaTime;
                Position += Velocity * deltaTime;

                Acceleration = Vector2{ 0.0f, 0.0f };
            }
        }

        void ApplyForce(const Vector2 force)
        {
            Acceleration += force;
        }

        void ApplyDamping(const float damping)
        {
            const float speed = Vector2Length(Velocity);
            if (speed > 0.0f)
            {
                const Vector2 direction = Vector2Normalize(Velocity);
                const float force = damping * speed;
                ApplyForce(direction * -force);
            }
        }

        Vector2 Position = Vector2Zeros;
        Vector2 Velocity = Vector2Zeros;
        Vector2 Acceleration = Vector2Zeros;
        bool IsStatic = false;
    };

    class AnchorSpring
    {
    public:
        AnchorSpring(const Vector2 anchorPos, const float restLength, const float k)
            : AnchorPos(anchorPos)
            , RestLength(restLength)
            , K(k)
        {
        }

        void Update()
        {
            if (m_pConnectedMover == nullptr)
            {
                return;
            }
            const Vector2& bobPos = m_pConnectedMover->Position;
            const Vector2 force = math::GetSpringForce(bobPos, AnchorPos, RestLength, K);
            m_pConnectedMover->ApplyForce(force);

            // Limit spring length.
            {
                const float maxLength = RestLength * 2.0f;
                const Vector2 spring = bobPos - AnchorPos;
                const float length = Vector2Length(spring);
                if (length > maxLength)
                {
                    const Vector2 direction = Vector2Normalize(spring);
                    const Vector2 newPos = AnchorPos + direction * maxLength;
                    m_pConnectedMover->Position = newPos;
                    m_pConnectedMover->Velocity = Vector2Zeros;
                }
            }
        }

        void Draw() const
        {
            if (m_pConnectedMover == nullptr)
            {
                return;
            }

            // Spring color:
            // BLACK: rest.
            // RED: stretched.
            // BLUE: compressed.
            const float distance = Vector2Distance(AnchorPos, m_pConnectedMover->Position);
            Color springColor =
                distance > RestLength ? RED :
                distance < RestLength ? BLUE :
                BLACK;

            // Anchor.
            DrawCircleLinesV(AnchorPos, 5.0f, springColor);

            // Spring.
            DrawLineV(AnchorPos, m_pConnectedMover->Position, springColor);
        }

        void ConnectedMover(Mover* pMover)
        {
            m_pConnectedMover = pMover;
        }

        Vector2 AnchorPos = Vector2Zeros;
        float RestLength = 0.0f;
        float K = 0.0f;

    private:
        Mover* m_pConnectedMover = nullptr;
    };

    class Spring
    {
    public:
        Spring(const float restLength, const float k)
            : RestLength(restLength)
            , K(k)
        {
        }

        void Update()
        {
            if (m_pConnectedMover1 == nullptr || m_pConnectedMover2 == nullptr)
            {
                return;
            }

            const Vector2 force = math::GetSpringForce(m_pConnectedMover2->Position, m_pConnectedMover1->Position, RestLength, K);
            m_pConnectedMover2->ApplyForce(force);
            m_pConnectedMover1->ApplyForce(force * -1.0f);
        }

        void Draw() const
        {
            if (m_pConnectedMover1 == nullptr || m_pConnectedMover2 == nullptr)
            {
                return;
            }

            // Spring color:
            // BLACK: rest.
            // RED: stretched.
            // BLUE: compressed.
            const float distance = Vector2Distance(m_pConnectedMover1->Position, m_pConnectedMover2->Position);
            Color springColor =
                distance > RestLength ? RED :
                distance < RestLength ? BLUE :
                BLACK;

            // Anchor.
            DrawCircleLinesV(AnchorPos, 5.0f, springColor);

            // Spring.
            DrawLineV(m_pConnectedMover1->Position, m_pConnectedMover2->Position, springColor);
        }

        void ConnectedMover(Mover* mover1, Mover* mover2)
        {
            if (mover1 == nullptr || mover2 == nullptr || mover1 == mover2)
            {
                return;
            }
            m_pConnectedMover1 = mover1;
            m_pConnectedMover2 = mover2;
        }

        Vector2 AnchorPos = Vector2Zeros;
        float RestLength = 0.0f;
        float K = 0.0f;

    private:
        Mover* m_pConnectedMover1 = nullptr;
        Mover* m_pConnectedMover2 = nullptr;
    };
}

int32_t main()
{
    // Window initialization.
    constexpr int32_t windowWidth = 800;
    constexpr int32_t windowHeight = 600;
    constexpr const char* pWindowTitle = "Raylib - Nature of Code";

    InitWindow(windowWidth, windowHeight, pWindowTitle);
    SetTargetFPS(60);

    auto canvas = LoadRenderTexture(windowWidth, windowHeight);

    const Vector2 center = { windowWidth * 0.5f , windowHeight * 0.5f };

    constexpr Vector2 gravity = { 0.0f, 100.0f };

    // Spring const.
    const Vector2 anchorPos = center;
    constexpr float restLength = 100.0f;
    constexpr float k = 50.0f;
    const Vector2 bobPos = center + Vector2{0.0f, 50.0f};

    // Anchor spring.
    Mover bob = { bobPos };
    AnchorSpring anchorSpring = { anchorPos, restLength, k };
    anchorSpring.ConnectedMover(&bob);

    // Spring chain.
    std::vector<Mover> movers;
    std::vector<Spring> springs;
    const Vector2 chainStarPos = center + Vector2{ -100.0f, -100.0f };
    {
        constexpr int32_t chainLength = 10;
        constexpr float chainRestLength = 30.0f;

        movers.reserve(chainLength);
        springs.reserve(chainLength - 1);

        for (int32_t i = 0; i < chainLength; ++i)
        {
            movers.emplace_back(chainStarPos + Vector2{ chainRestLength * i, 0.0f });
        }
        movers.front().IsStatic = true;

        for (int32_t i = 0; i < chainLength - 1; ++i)
        {
            constexpr float chainK = 500.0f;
            springs.emplace_back(chainRestLength, chainK);
            springs.back().ConnectedMover(&movers[i], &movers[i + 1]);
        }
    }

    while (!WindowShouldClose())
    {
        const float time = static_cast<float>(GetTime());
        const float deltaTime = GetFrameTime();

        // Update.
        {
            // Anchor spring.
            {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                    bob.Position = GetMousePosition();
                    bob.Velocity = Vector2Zeros;
                }

                bob.ApplyForce(gravity);
                bob.ApplyDamping(0.1f);

                anchorSpring.Update();
                bob.Update();
            }

            // Spring chain.
            {
                if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                {
                    //movers.back().Position = GetMousePosition();
                    //movers.back().Velocity = Vector2Zeros;
                    const Vector2 dragForce = (GetMousePosition() - movers.back().Position) * 100.0f;
                    movers.back().ApplyForce(dragForce);
                }

                for (auto& mover : movers)
                {
                    mover.ApplyForce(gravity * 10.0f);
                    mover.ApplyDamping(0.8f);
                }

                for (auto& spring : springs)
                {
                    spring.Update();
                }
                for (auto& mover : movers)
                {
                    mover.Update();
                }
            }
        }

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        // Draw.
        {
            // Anchor spring.
            {
                anchorSpring.Draw();
                DrawCircleLinesV(bob.Position, 10.0f, BLACK);
            }

            // Spring chain.
            {
                for (const auto& mover : movers)
                {
                    DrawCircleLinesV(mover.Position, 10.0f, BLUE);
                }
                for (const auto& spring : springs)
                {
                    spring.Draw();
                }
            }
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw UI.
        {

        }
        constexpr auto sourceRect = Rectangle{ 0.0f, 0.0f, windowWidth, -windowHeight };
        constexpr auto destRect = Rectangle{ 0.0f, 0.0f, windowWidth, windowHeight };
        DrawTexturePro(canvas.texture, sourceRect, destRect, { 0.0f, 0.0f }, 0.0f, WHITE);
        DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
