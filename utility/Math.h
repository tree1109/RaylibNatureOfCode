#pragma once

#ifndef INCLUDE_MATH_H
#define INCLUDE_MATH_H

#include <raylib.h>
#include <random>

namespace math {
    float GetRandomGaussian(float mean, float stdDev);
    float GetRandomValue(float min, float max);
    float GetRandomValue();
    float AcceptReject();
    float MapValue(float value, float currentMin, float currentMax, float newMin, float newMax);
    float PeringNoise(float x);

    Vector2 GetRandomVector2();
}

#endif // !INCLUDE_MATH_H