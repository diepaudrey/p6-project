#include "Boids.hpp"
#include <vcruntime.h>
#include <iterator>

void Boids::drawBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    for (auto& boid : m_boids)
    {
        boid.draw(ctx, boidParam.protectedRadius);
        ctx.circle(boid.getPosition(), boidParam.protectedRadius);
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

void Boids::avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor, BoidsParameters& boidParam)
{
    if (boid.getPosition().x + boidParam.protectedRadius > ctx.aspect_ratio())
    {
        boid.addSpeedX(-turnfactor);
    }
    if (boid.getPosition().x - boidParam.protectedRadius < -ctx.aspect_ratio())
    {
        boid.addSpeedX(turnfactor);
    }

    if (boid.getPosition().y + boidParam.protectedRadius > 1)
    {
        boid.addSpeedY(-turnfactor);
    }
    if (boid.getPosition().y - boidParam.protectedRadius < -1)
    {
        boid.addSpeedY(turnfactor);
    }
}

bool Boids::isTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition()) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::fillNeighbors(const Boid& boid, p6::Context& ctx, BoidsParameters& boidParam)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, boidParam.protectedRadius))
        {
            neighbors.push_back(otherBoid);
            ctx.fill = {1.f, 0.f, 0.f, 0.3f};
        }
    }
    return neighbors;
}

/* 3 Rules of the game*/

glm::vec2 Boids::separation(const Boid& boid, BoidsParameters& boidParam) const
{
    glm::vec2   steeringForce(0.f, 0.f);
    const float separationRange   = 0.3f;
    int         numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, separationRange))

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

glm::vec2 Boids::alignment(const Boid& boid, BoidsParameters& boidParam) const
{
    glm::vec2 averageDirection(0.f, 0.f);
    float     alignmentRange    = 0.2f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, alignmentRange))
        {
            averageDirection += otherBoid.getSpeed();
            // std::cout << "Other Boid speed  " << otherBoid.m_speed.x << " " << otherBoid.m_speed.y << std::endl;
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

glm::vec2 Boids::cohesion(const Boid& boid, BoidsParameters& boidParam) const
{
    glm::vec2 averageLocation(0.f, 0.f);
    float     cohesionRange     = 0.5f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, cohesionRange))
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

void Boids::applySteeringForces(Boid& boid, BoidsParameters& boidParam)
{
    boid.applyForce(alignment(boid, boidParam));
    boid.applyForce(cohesion(boid, boidParam));
    boid.applyForce(separation(boid, boidParam));
    boid.limitSpeed(boidParam.maxSpeed);
}

void Boids::updateBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    float turnfactor = 0.3f;
    boidParam.updateBoidsParam();
    for (auto& boid : m_boids)
    {
        std::vector<Boid> neighbors = fillNeighbors(boid, ctx, boidParam);
        boid.updatePosition(ctx);
        applySteeringForces(boid, boidParam);
        avoidEdges(boid, ctx, turnfactor, boidParam);
        boid.draw(ctx, boidParam.protectedRadius);
        neighbors.clear();
    }
};
