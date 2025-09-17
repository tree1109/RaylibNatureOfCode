#ifndef INCLUDE_I_MANAGER_H
#define INCLUDE_I_MANAGER_H

#include <string_view>

class IManager
{
public:
    IManager() = default;
    virtual ~IManager() = default;

public:
    virtual void Init() = 0;
    virtual void DeInit() = 0;

    [[nodiscard]] virtual std::string_view GetName() const = 0;
    [[nodiscard]] virtual size_t GetSize() const = 0;
    [[nodiscard]] virtual size_t GetCapacity() const = 0;
};

#endif // INCLUDE_I_MANAGER_H
