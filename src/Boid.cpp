#include "Boid.hpp"

void Boid::draw(p6::Context& ctx, const float& protectedRadius, const float& visualRange)
{
    ctx.fill = {1.f, 1.f, 1.f, 0.1f};
    ctx.circle(m_position, visualRange);

    ctx.fill = {1.f, 1.f, 1.f, 0.3f};
    ctx.circle(m_position, protectedRadius);

    ctx.fill = {0.f, 0.f, 0.f, 1.f};
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

void Boid::applyForce(const glm::vec2 force)
{
    this->m_speed += force;
}

void Boid::limitSpeed(const float& maxSpeed)
{
    if (glm::length(m_speed) > maxSpeed)
    {
        m_speed = glm::normalize(m_speed) * maxSpeed;
    }
}

void Boid::avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor, const float& protectedRadius)
{
    if (boid.m_position.x + protectedRadius > ctx.aspect_ratio())
    {
        boid.m_speed.x -= turnfactor;
    }
    if (boid.m_position.x - protectedRadius < -ctx.aspect_ratio())
    {
        boid.m_speed.x += turnfactor;
    }

    if (boid.m_position.y + protectedRadius > 1)
    {
        boid.m_speed.y -= turnfactor;
    }
    if (boid.m_position.y - protectedRadius < -1)
    {
        boid.m_speed.y += turnfactor;
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