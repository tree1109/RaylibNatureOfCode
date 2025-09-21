#ifndef INCLUDE_PLAYGROUND_7_H
#define INCLUDE_PLAYGROUND_7_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    class CPlayground7 final : public IPlayground
    {
    public:
        explicit CPlayground7(IGame& game);

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

#endif // INCLUDE_PLAYGROUND_7_H
