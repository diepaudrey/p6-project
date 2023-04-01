#include "Boid.hpp"

void Boid::draw(p6::Context& ctx)
{
    ctx.circle(m_position, protectedRadius);
    ctx.fill = {1.f, 1.f, 1.f, 0.5f};

    ctx.equilateral_triangle(
        p6::Center{m_position},
        p6::Radius{0.05f},
        p6::Rotation{m_speed}
    );

    ctx.use_stroke = false;
}

void Boid::updatePosition(p6::Context& ctx)
{
    m_position += ctx.delta_time() * m_speed;
}

void Boid::setMaxSpeed(const float& speed)
{
    this->maxSpeed = speed;
}

void Boid::limitSpeed()
{
    if (glm::length(m_speed) > maxSpeed)
    {
        m_speed = glm::normalize(m_speed) * maxSpeed;
    }
}

bool operator==(const Boid& a, const Boid& b)
{
    return (a.getPosition() == b.getPosition()) && (a.getSpeed() == b.getSpeed());
}

bool operator!=(const Boid& a, const Boid& b)
{
    return !(a == b);
}