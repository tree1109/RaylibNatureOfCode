#ifndef INCLUDE_PLAYGROUND_5_H
#define INCLUDE_PLAYGROUND_5_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground5 final : public IPlayground
    {
    public:
        explicit CPlayground5(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_5_H
