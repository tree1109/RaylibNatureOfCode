#ifndef INCLUDE_PLAYGROUND_8_H
#define INCLUDE_PLAYGROUND_8_H

#include "PlaygroundBase.h"

namespace playground
{
    class CPlayground8 final : public CPlaygroundBase
    {
    public:
        explicit CPlayground8(IGame& game);

    public:
        void Init() override;
        void DeInit() override;
        void Update() override;
        void DrawWorld() override;
        void DrawUi() override;

        [[nodiscard]] std::string_view GetName() const override;
    };
} // namespace playground

#endif // INCLUDE_PLAYGROUND_8_H
