#ifndef INCLUDE_PLAYGROUND_13_H
#define INCLUDE_PLAYGROUND_13_H

#include "IPlayground.h"

class IGame;

namespace playground
{
    class CPlayground13 final : public IPlayground
    {
    public:
        explicit CPlayground13(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_13_H
