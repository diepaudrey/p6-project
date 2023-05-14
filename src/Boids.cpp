#include "Boids.hpp"
#include <vcruntime.h>
#include <iterator>

void Boids::drawBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    for (auto& boid : m_boids)
    {
        boid.draw(ctx, boidParam.protectedRadius, boidParam.visualRange);
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

bool Boids::isTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition()) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::fillNeighbors(const Boid& boid, BoidsParameters& boidParam)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            neighbors.push_back(otherBoid);
        }
    }
    return neighbors;
}

/* 3 Rules of the game*/

glm::vec2 Boids::separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec2 steeringForce(0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.protectedRadius))

        {
            float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());
            steeringForce += (boid.getPosition() - otherBoid.getPosition()) / distance;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        steeringForce /= numberOfNeighbors;
        steeringForce = normalize(steeringForce) * boidParam.separationStrength;
    }

    return steeringForce;
}

glm::vec2 Boids::alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec2 averageDirection(0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            averageDirection += otherBoid.getSpeed();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageDirection /= numberOfNeighbors;
        averageDirection *= boidParam.alignmentStrength;
    }
    return averageDirection;
}

glm::vec2 Boids::cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec2 averageLocation(0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            averageLocation += otherBoid.getPosition();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = (averageLocation - boid.getPosition()) * boidParam.cohesionStrength;
    }

    return averageLocation;
}

void Boids::applySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    boid.applyForce(alignment(boid, boidParam, neighbors));
    boid.applyForce(cohesion(boid, boidParam, neighbors));
    boid.applyForce(separation(boid, boidParam, neighbors));
    boid.limitSpeed(boidParam.maxSpeed);
}

void Boids::updateBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    float turnfactor = 0.3f;
    boidParam.updateBoidsParam();
    for (auto& boid : m_boids)
    {
        std::vector<Boid> neighbors = fillNeighbors(boid, boidParam);
        boid.updatePosition(ctx);
        applySteeringForces(boid, boidParam, neighbors);
        boid.avoidEdges(boid, ctx, turnfactor, boidParam.protectedRadius);
        boid.draw(ctx, boidParam.protectedRadius, boidParam.visualRange);
        neighbors.clear();
    }
};
