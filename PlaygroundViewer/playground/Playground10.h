#ifndef INCLUDE_PLAYGROUND_10_H
#define INCLUDE_PLAYGROUND_10_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground10 final : public IPlayground
    {
    public:
        explicit CPlayground10(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_10_H
