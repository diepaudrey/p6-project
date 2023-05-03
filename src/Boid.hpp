#pragma once
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_speed;

public:
    Boid() = default;
    Boid(const glm::vec2& pos, const glm::vec2 vel)
        : m_position(pos), m_speed(vel){};

    glm::vec2 getPosition() const
    {
        return m_position;
    }
    glm::vec2 getSpeed() const
    {
        return m_speed;
    }

    void draw(p6::Context& ctx, const float& protectedRadius, const float& visualRange);
    void updatePosition(p6::Context& ctx);
    void applyForce(const glm::vec2 force);
    void avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor, const float& protectedRadius);

    // limit the speed
    void limitSpeed(const float& maxSpeed);

    friend bool operator==(const Boid& a, const Boid& b);
    friend bool operator!=(const Boid& a, const Boid& b);
};