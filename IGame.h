#ifndef INCLUDE_I_GAME_H
#define INCLUDE_I_GAME_H

#include <raylib.h>
#include <string_view>

class IService;
class IManager;

class IGame
{
public:
    IGame() = default;
    virtual ~IGame() = default;

public:
    virtual IService& GetService(const std::string_view& serviceName) = 0;
    virtual IManager& GetManager(const std::string_view& managerName) = 0;

    virtual IGame& SetBackgroundColor(const Color& color) = 0;
    virtual IGame& SetClearBackgroundEachFrame(const bool isClear) = 0;

    [[nodiscard]] virtual int32_t GetWindowWidth() const = 0;
    [[nodiscard]] virtual int32_t GetWindowHeight() const = 0;
    [[nodiscard]] virtual Vector2 GetWindowCenterPosition() const = 0;
    [[nodiscard]] virtual Vector2 GetMouseWorldPosition() const = 0;

public:
    template <typename T>
    T& GetService()
    {
        static_assert(std::is_base_of_v<IService, T>);
        return dynamic_cast<T&>(GetService(T::StaticGetName()));
    }

    template <typename T>
    T& GetManager()
    {
        static_assert(std::is_base_of_v<IManager, T>);
        return dynamic_cast<T&>(GetManager(T::StaticGetName()));
    }
};

#endif // INCLUDE_I_GAME_H
