#ifndef INCLUDE_PLAYGROUND_2_H
#define INCLUDE_PLAYGROUND_2_H

#include "PlaygroundBase.h"

namespace playground
{
    class CPlayground2 final : public CPlaygroundBase
    {
    public:
        explicit CPlayground2(IGame& game);

    public:
        void Init() override;
        void DeInit() override;
        void Update() override;
        void DrawWorld() override;
        void DrawUi() override;

        [[nodiscard]] std::string_view GetName() const override;
    };
} // namespace playground

#endif // INCLUDE_PLAYGROUND_2_H
