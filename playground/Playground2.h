#ifndef INCLUDE_PLAYGROUND_2_H
#define INCLUDE_PLAYGROUND_2_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground2 final : public IPlayground
    {
    public:
        explicit CPlayground2(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_2_H
