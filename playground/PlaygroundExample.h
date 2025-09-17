#ifndef INCLUDE_PLAYGROUND_EXAMPLE_H
#define INCLUDE_PLAYGROUND_EXAMPLE_H

#include "IGame.h"
#include "IPlayground.h"

namespace playground
{
    class CPlaygroundExample final : public IPlayground
    {
    public:
        CPlaygroundExample(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_EXAMPLE_H
