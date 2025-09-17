#include "Playground1.h"

#include "utility/Math.h"

namespace
{
    class Walker
    {
    public:
        Walker() = default;

        explicit Walker(Vector2 position) : m_position(position)
        {
        }

        void Update()
        {
            m_prevPosition = m_position;

            constexpr float maxStep = 2.0f;
            const float stepX = math::MapValue(math::PeringNoise(GetTime()), 0.0f, 1.0f, -maxStep, maxStep);
            const float stepY = math::MapValue(math::PeringNoise(GetTime() + 1233.0f), 0.0f, 1.0f, -maxStep, maxStep);
            m_position.x += stepX;
            m_position.y += stepY;
        }

        void Draw() const
        {
            DrawLineV(m_prevPosition, m_position, BLACK);
        }

    private:
        Vector2 m_position = {0.0f, 0.0f};
        Vector2 m_prevPosition = {0.0f, 0.0f};
    };

    Walker walker;
} // namespace

namespace playground
{
    CPlayground1::CPlayground1(IGame& game) : m_game(game)
    {
    }

    void CPlayground1::Init()
    {
        m_game.SetClearBackgroundEachFrame(false);
        walker = Walker(m_game.GetWindowCenterPosition());
    }

    void CPlayground1::DeInit()
    {
    }

    void CPlayground1::Update()
    {
        walker.Update();
    }

    void CPlayground1::DrawWorld()
    {
        walker.Draw();
    }

    void CPlayground1::DrawUi()
    {
    }
} // namespace playground
