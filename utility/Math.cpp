#include "Math.h"

#include <random>
#include <numbers>

#include "raymath.h"

#include "external/stb_perlin.h"

float math::GetRandomGaussian(float mean, float stdDev)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stdDev);

    return dist(gen);
}

float math::GetRandomValue(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

float math::GetRandomValue()
{
    return GetRandomValue(0.0f, 1.0f);
}

float math::AcceptReject()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    while (true)
    {
        float r1 = dist(gen);
        const float& probability = r1 * r1;

        float r2 = dist(gen);
        if (r2 < probability)
        {
            return r1;
        }
    }
}

float math::MapValue(float value, float currentMin, float currentMax, float newMin, float newMax)
{
    float scale = (newMax - newMin) / (currentMax - currentMin);
    float mappedValue = newMin + (value - currentMin) * scale;
    return mappedValue;
}

float math::PeringNoise(float x)
{
    float noise = stb_perlin_noise3(x, x + 1234.1234f, x - 321.312f, 0, 0, 0);
    return MapValue(noise, -1.0f, 1.0f, 0.0f, 1.0f);
}

Vector2 math::GetRandomDirection()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, std::numbers::pi_v<float> * 2.0f);
    float rad = dist(gen);
    return {std::cosf(rad), std::sinf(rad)};
}

Vector2 math::GetRandomVector2(const float scale)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, scale);
    return {dist(gen), dist(gen)};
}

Vector2 math::GetRandomGaussianVector2(const float stdDev)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, stdDev);

    return {dist(gen), dist(gen)};
}

Vector2 math::GetSpringForce(const Vector2& bobPos, const Vector2& anchorPos, const float restLength, const float k)
{
    const Vector2 spring = bobPos - anchorPos;
    const float distance = Vector2Length(spring);
    const float stretch = distance - restLength;
    const Vector2 force = Vector2Normalize(spring) * -k * stretch;
    return force;
}
