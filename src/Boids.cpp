#include "Boids.hpp"
#include <vcruntime.h>
#include <iterator>

void Boids::drawBoids(p6::Context& ctx)
{
    for (auto& boid : m_boids)
    {
        boid.draw(ctx);
        ctx.circle(boid.m_position, boid.protectedRadius);
        ctx.fill = {1.f, 1.f, 1.f, 0.5f};
    }
}

void Boids::fillBoids(p6::Context& ctx)
{
    for (int i = 0; i < m_numBoids; i++)
    {
        glm::vec2 pos   = p6::random::point(ctx);
        glm::vec2 speed = pos + p6::random::point(ctx);
        Boid      boid(pos, speed);

        m_boids.push_back(boid);
    }
}

void Boids::avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor)
{
    if (boid.m_position.x + boid.protectedRadius > ctx.aspect_ratio())
    {
        boid.m_speed.x -= turnfactor;
    }
    if (boid.m_position.x - boid.protectedRadius < -ctx.aspect_ratio())
    {
        boid.m_speed.x += turnfactor;
    }

    if (boid.m_position.y + boid.protectedRadius > ctx.inverse_aspect_ratio() * 2)
    {
        boid.m_speed.y -= turnfactor;
    }
    if (boid.m_position.y - boid.protectedRadius < -ctx.inverse_aspect_ratio() * 2)
    {
        boid.m_speed.y += turnfactor;
    }
}

bool Boids::isTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.m_position, boid2.m_position) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::fillNeighbors(const Boid& boid, p6::Context& ctx)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, boid.protectedRadius))
        {
            neighbors.push_back(otherBoid);
            ctx.fill = {1.f, 0.f, 0.f, 0.3f};
        }
    }
    return neighbors;
}

// void Boids::displayCollision(const std::vector<Boid>& neighbors, p6::Context& ctx) const
// {
//     ctx.fill = {1.f, 0.f, 0.f, 0.3f};
// }

/* 3 Rules of the game*/

glm::vec2 Boids::separation(Boid& boid)
{
    glm::vec2   steeringForce(0.f, 0.f);
    const float separationRange   = 0.3f;
    int         numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, separationRange))

        {
            float distance = glm::distance(boid.m_position, otherBoid.m_position);
            steeringForce += (boid.m_position - otherBoid.m_position) / distance;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        steeringForce /= numberOfNeighbors;
        steeringForce = normalize(steeringForce) * separationStrength;
    }

    return steeringForce;
}

glm::vec2 Boids::alignment(const Boid& boid) const
{
    glm::vec2 averageDirection(0.f, 0.f);
    float     alignmentRange    = 0.2f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, alignmentRange))
        {
            averageDirection += otherBoid.m_speed;
            // std::cout << "Other Boid speed  " << otherBoid.m_speed.x << " " << otherBoid.m_speed.y << std::endl;
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

glm::vec2 Boids::cohesion(Boid& boid)
{
    glm::vec2 averageLocation(0.f, 0.f);
    float     cohesionRange     = 0.5f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, cohesionRange))
        {
            averageLocation += otherBoid.m_position;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = (averageLocation - boid.m_position) * cohesionStrength;
    }

    return averageLocation;
}

void Boids::applySteeringForce(Boid& boid)
{
    boid.m_speed += alignment(boid);
    boid.m_speed += cohesion(boid);
    boid.m_speed += separation(boid);
    boid.limitSpeed();
}
