#ifndef INCLUDE_I_PLAYGROUND_H
#define INCLUDE_I_PLAYGROUND_H
#include <string_view>

namespace playground
{
    class IPlayground
    {
    public:
        IPlayground() = default;
        virtual ~IPlayground() = default;

    public:
        virtual void Init() = 0;
        virtual void DeInit() = 0;
        virtual void Update() = 0;
        virtual void DrawWorld() = 0;
        virtual void DrawUi() = 0;

        virtual std::string_view GetName() const = 0;
    };
} // namespace playground

#endif // INCLUDE_I_PLAYGROUND_H
