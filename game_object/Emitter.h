#ifndef INCLUDE_EMITTER_H
#define INCLUDE_EMITTER_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cstdint>

#include "Particle.h"

class IForceField;

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

    CEmitter& ApplyForce(const Vector2& force);
    CEmitter& ApplyForceField(const IForceField& forceField);
    CEmitter& SetTint(const Color& tint);
    CEmitter& SetPosition(const Vector2& position);
    CEmitter& SetParticleInitialForce(const Vector2& force);
    CEmitter& SetParticleLifeTime(const float lifeTime);
    CEmitter& SetParticleSpawnCountPerFrame(const int32_t count);
    CEmitter& SetMaxParticleCount(const int32_t maxCount);
    CEmitter& SetEmitting(const bool isEmitting);
    CEmitter& SetRandomColor(const bool isRandomColor);

    CEmitter& SetDrawParticleWithTexture(const bool isDawParticleWithTexture);
    CEmitter& SetParticleTexture(const Texture2D& texture);

    [[nodiscard]] size_t GetParticleCount() const;
    [[nodiscard]] size_t GetCapacity() const;

private:
    Color m_tint{ WHITE };
    Vector2 m_position{ Vector2Zero() };
    Vector2 m_initialForce{Vector2Zero()};
    Vector2 m_force{ Vector2Zero() };
    float m_particleLifeTime{ 10.0f };
    size_t m_particleSpawnCountPerFrame{ 10 };
    size_t m_maxParticleCount = 16384;
    bool m_bEmitting{ true };
    bool m_bRandomColor{ false };
    std::vector<CParticle> m_particles;

    // TODO: Refactor this with model.
    bool m_bDrawParticleWithTexture{false};
    Texture2D m_particleTexture;
};

#endif //INCLUDE_EMITTER_H
