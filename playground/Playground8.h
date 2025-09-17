#ifndef INCLUDE_PLAYGROUND_8_H
#define INCLUDE_PLAYGROUND_8_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground8 final : public IPlayground
    {
    public:
        explicit CPlayground8(IGame& game);

    public:
        void Init() override;
        void DeInit() override;
        void Update() override;
        void DrawWorld() override;
        void DrawUi() override;

    private:
        IGame& m_game;

    };
} // namespace playground

#endif // INCLUDE_PLAYGROUND_8_H
