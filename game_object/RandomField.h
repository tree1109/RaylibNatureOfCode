#ifndef INCLUDE_RANDOM_FIELD_H
#define INCLUDE_RANDOM_FIELD_H

#include "IForceField.h"

class CRandomField : public IForceField
{
public:
    CRandomField(const float power);
    ~CRandomField() override = default;

    void Draw() const override;

    CRandomField& SetPower(const float power);

    Vector2 GetForce(CParticle& particle) const override;

private:
    float m_power = 1.0f;
};

#endif //INCLUDE_RANDOM_FIELD_H
