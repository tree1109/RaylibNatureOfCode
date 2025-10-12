#ifndef INCLUDE_PLAYGROUND_11_H
#define INCLUDE_PLAYGROUND_11_H

#include "PlaygroundBase.h"

namespace playground
{
    class CPlayground11 final : public CPlaygroundBase
    {
    public:
        explicit CPlayground11(IGame& game);

    public:
        void Init() override;
        void DeInit() override;
        void Update() override;
        void DrawWorld() override;
        void DrawUi() override;

        [[nodiscard]] std::string_view GetName() const override;
    };
} // namespace playground

#endif // INCLUDE_PLAYGROUND_11_H
