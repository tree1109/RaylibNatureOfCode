#ifndef INCLUDE_PLAYGROUND_6_H
#define INCLUDE_PLAYGROUND_6_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground6 final : public IPlayground
    {
    public:
        explicit CPlayground6(IGame& game);

    public:
        void Init() override;
        void DeInit() override;
        void Update() override;
        void DrawWorld() override;
        void DrawUi() override;

        [[nodiscard]] std::string_view GetName() const override;

    private:
        IGame& m_game;

    };
} // namespace playground

#endif // INCLUDE_PLAYGROUND_6_H
