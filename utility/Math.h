#pragma once

#ifndef INCLUDE_MATH_H
#define INCLUDE_MATH_H

#include <raylib.h>
#include <random>

namespace math {
    constexpr Vector2 Gravity{ 0.0f, 9.81f };

    float GetRandomGaussian(float mean, float stdDev);
    float GetRandomValue(float min, float max);
    float GetRandomValue();
    float AcceptReject();
    float MapValue(float value, float currentMin, float currentMax, float newMin, float newMax);
    float PeringNoise(float x);

    Vector2 GetRandomVector2();
    float GetAngle(const Vector2& direction);

    Vector2 GetSpringForce(const Vector2& bobPos, const Vector2& anchorPos, const float restLength, const float k);

    Vector2 TranslateWorldSpaceToScreenSpace(Vector2 posInWorld, Camera);
}

#endif // !INCLUDE_MATH_H
