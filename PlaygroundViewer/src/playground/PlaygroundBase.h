#ifndef INCLUDE_PLAYGROUND_BASE_H
#define INCLUDE_PLAYGROUND_BASE_H

#include "IPlayground.h"
#include "IGame.h"

namespace playground
{
    // Base class providing shared utilities/state for playgrounds
    class CPlaygroundBase : public IPlayground
    {
    public:
        explicit CPlaygroundBase(IGame& game) : m_game(game) {}
        ~CPlaygroundBase() override = default;

        // Default no-op implementations (can be overridden by children)
        void DeInit() override {}
        void DrawUi() override {}

    protected:
         // Accessors for derived classes
        IGame& GetGame() noexcept { return m_game; }
        const IGame& GetGame() const noexcept { return m_game; }

    private:
        IGame& m_game;
    };
}

#endif // INCLUDE_PLAYGROUND_BASE_H
