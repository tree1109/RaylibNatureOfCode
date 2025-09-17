#ifndef INCLUDE_I_SERVICE_H
#define INCLUDE_I_SERVICE_H

#include <string_view>

class IService
{
public:
    IService() = default;
    virtual ~IService() = default;

public:
    [[nodiscard]] virtual std::string_view GetName() const = 0;
};

#endif // INCLUDE_I_SERVICE_H
