#include "Boids.hpp"
#include <vcruntime.h>
#include <iterator>

void Boids::draw(p6::Context& ctx)
{
    ctx.circle(m_position, this->protectedRadius);
    ctx.fill = {1.f, 1.f, 1.f, 0.5f};

    ctx.equilateral_triangle(
        p6::Center{m_position},
        p6::Radius{0.05f},
        p6::Rotation{m_speed}
    );

    ctx.use_stroke = false;
}

void Boids::avoidEdges(const p6::Context& ctx, const float& turnfactor)
{
    if (m_position.x + this->protectedRadius > ctx.aspect_ratio())
    {
        m_speed.x -= turnfactor;
    }
    if (m_position.x - this->protectedRadius < -ctx.aspect_ratio())
    {
        m_speed.x += turnfactor;
    }

    if (m_position.y + this->protectedRadius > ctx.inverse_aspect_ratio() * 2)
    {
        m_speed.y -= turnfactor;
    }
    if (m_position.y - this->protectedRadius < -ctx.inverse_aspect_ratio() * 2)
    {
        m_speed.y += turnfactor;
    }
}

bool Boids::isTooClose(const Boids& boid, const float& radius) const
{
    return glm::distance(boid.m_position, m_position) < radius && glm::distance(boid.m_position, m_position) != 0;
}

void Boids::displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const
{
    for (const auto& boid : neighbors)
        if (isTooClose(boid, protectedRadius))
        {
            ctx.fill = {1.f, 0.f, 0.f, 0.3f};
        }
}

/* 3 Rules of the game*/

glm::vec2 Boids::separation(const std::vector<Boids>& boids)
{
    glm::vec2   force(0.f, 0.f);
    const float separationRange   = 0.2f;
    int         numberOfNeighbors = 0;

    for (const auto& boid : boids)
    {
        if (isTooClose(boid, separationRange))

        {
            force += (m_position - boid.m_position) / glm::distance(m_position, boid.m_position);

            numberOfNeighbors++;
        }

        if (numberOfNeighbors != 0)
        {
            force /= numberOfNeighbors;
            force = normalize(force);
            force *= separationStrength;
        }
    }

    return force;
}

glm::vec2 Boids::alignment(const std::vector<Boids>& boids) const
{
    glm::vec2 averageDirection(0.f, 0.f);
    float     alignmentRange    = 0.2f;
    int       numberOfNeighbors = 0;

    for (const auto& boid : boids)
    {
        if (isTooClose(boid, alignmentRange))
        {
            averageDirection += boid.m_speed;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageDirection *= alignmentStrength;
        averageDirection /= numberOfNeighbors;
    }
    return averageDirection;
}

glm::vec2 Boids::cohesion(const std::vector<Boids>& boids)
{
    glm::vec2 averageLocation(0.f, 0.f);
    float     cohesionRange     = 0.5f;
    int       numberOfNeighbors = 0;

    for (const auto& boid : boids)
    {
        if (isTooClose(boid, cohesionRange))
        {
            averageLocation += boid.m_position;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = (averageLocation - m_position) * cohesionStrength;
    }

    return averageLocation;
}

void Boids::applySteeringForce(const std::vector<Boids>& boids)
{
    m_speed += alignment(boids);
    m_speed += cohesion(boids);
    m_speed += separation(boids);
    this->limitSpeed();
}

void Boids::updatePosition(p6::Context& ctx)
{
    m_position += ctx.delta_time() * m_speed;
}