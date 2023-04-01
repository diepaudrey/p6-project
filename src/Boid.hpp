#pragma once
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_speed;

    float maxSpeed;
    float protectedRadius;

public:
    friend class Boids;

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

    void draw(p6::Context& ctx);
    void updatePosition(p6::Context& ctx);
    void setMaxSpeed(const float& speed);

    // limit the speed
    void limitSpeed();

    friend bool operator==(const Boid& a, const Boid& b);
    friend bool operator!=(const Boid& a, const Boid& b);
};