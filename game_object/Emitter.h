#ifndef INCLUDE_EMITTER_H
#define INCLUDE_EMITTER_H

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Particle.h"

class CEmitter {
public:
    CEmitter() = default;
    explicit CEmitter(const Vector2& position);
    ~CEmitter() = default;

    CEmitter(const CEmitter& other) = delete;
    CEmitter& operator=(const CEmitter& other) = delete;
    CEmitter(CEmitter&& other) noexcept = default;
    CEmitter& operator=(CEmitter&& other) noexcept = default;

    void Update();
    void Draw() const;

    CEmitter& SetPosition(const Vector2& position);
    CEmitter& SetParticleInitialVelocity(const Vector2& velocity);
    CEmitter& SetForce(const Vector2& force);
    CEmitter& SetParticleLifeTime(const float lifeTime);
    CEmitter& SetParticleSpawnCountPerFrame(const int32_t count);
    CEmitter& SetEmitting(const bool isEmitting);

    size_t GetParticleCount() const;
    size_t GetCapacity() const;

private:
    Vector2 m_position{ Vector2Zero() };
    Vector2 m_initialForce{ Vector2Zero() };
    Vector2 m_force{ Vector2Zero() };
    float m_particleLifeTime{ 10.0f };
    int32_t m_particleSpawnCountPerFrame{ 10 };
    bool m_bEmitting{ false };
    std::vector<CParticle> m_particles;
};

#endif //INCLUDE_EMITTER_H
