#include "boid.hpp"
#include <vcruntime.h>

void Boid::draw(p6::Context& ctx)
{
    ctx.circle(this->position, this->protectedRadius);
    ctx.fill = {0.f, 1.f, 0.f, 0.3f};
    ctx.equilateral_triangle(
        p6::Center{this->position},
        p6::Radius{0.1f},
        p6::Rotation{this->direction}
    );
}

bool Boid::isOutWindow(const p6::Context& ctx) const
{
    return this->position.x + this->protectedRadius > ctx.aspect_ratio() || this->position.y + this->protectedRadius > ctx.inverse_aspect_ratio() * 2 || this->position.x - this->protectedRadius < -ctx.aspect_ratio() || this->position.y - this->protectedRadius < -ctx.inverse_aspect_ratio() * 2;
}

void Boid::updateDirectionBorders(const p6::Context& ctx)
{
    if (this->isOutWindow(ctx))
    {
        this->direction = -(this->direction);
    }
}

bool Boid::isTooClose(const Boid& boid) const
{
    return glm::distance(boid.position, this->position) < this->protectedRadius && glm::distance(boid.position, this->position) != 0;
}

std::vector<Boid> Boid::fillNeighbors(const std::vector<Boid>& boids, p6::Context& ctx) const
{
    std::vector<Boid> neighbors;
    for (const auto& boid : boids)
    {
        if (this->isTooClose(boid))
        {
            neighbors.push_back(boid);
            drawCollision(neighbors, ctx);
        }
    }

    return neighbors;
}

void Boid::drawCollision(const std::vector<Boid>& neighbors, p6::Context& ctx)
{
    if (!neighbors.empty())
    {
        ctx.fill = {1.f, 0.f, 0.f, 0.3f};
    }
}

/* 3 Rules of the game*/

void Boid::separation(const std::vector<Boid>& boids)
{
    glm::vec2 force(0.f, 0.f);

    for (const auto& boid : boids)
    {
        if (isTooClose(boid))
        {
            force += (this->position - boid.position);
        }
    }

    this->direction += force * this->separationStrength;
    this->direction = normalize(this->direction);
}

void Boid::alignment(const std::vector<Boid>& boids)
{
    glm::vec2 averageVelocity(0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& boid : boids)
    {
        if (isTooClose(boid))
        {
            averageVelocity += boid.speed;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors > 0)
    {
        averageVelocity /= numberOfNeighbors;
    }

    this->speed += (averageVelocity - this->speed) * this->alignmentStrength;
}

glm::vec2 Boid::calculateSeparationForce(const std::vector<Boid>& neighbors)
{
    glm::vec2 totalForce;

    for (const auto& boid : neighbors)
    {
        totalForce += (this->position - boid.position) / glm ::distance(boid.position, this->position);
    }

    return totalForce;
}

glm::vec2 Boid::calculateAlignmentForce(const std::vector<Boid>& neighbors)
{
    glm::vec2 averageDirection;
    for (const auto& boid : neighbors)
    {
        averageDirection += boid.direction;
    }
    if (!neighbors.empty())
    {
        averageDirection /= neighbors.size();
        averageDirection = normalize(averageDirection);
    }
    // std::cout << "Average direction : " << averageDirection.x << " " << averageDirection.y << std::endl;
    return averageDirection;
}

glm::vec2 Boid::calculateCohesionForce(const std::vector<Boid>& neighbors)
{
    glm::vec2 averagePosition;
    for (const auto& boid : neighbors)
    {
        averagePosition += boid.position;
    }
    if (!neighbors.empty())
    {
        averagePosition /= neighbors.size();
    }
    averagePosition = normalize(averagePosition);
    return averagePosition;
}

void Boid::applySteeringForce()
{
    this->direction += this->alignmentStrength * calculateAlignmentForce(this->neighbors);
    // this->direction += this->separationStrength * calculateSeparationForce(this->neighbors);
    // this->direction += this->cohesionStrength * (calculateCohesionForce(this->neighbors) - this->position);
    this->direction = normalize(this->direction);
}

//  glm::vec2 totalForce;
//     for (auto& boid : neighbors)
//     {
//         glm::vec2 separationForce = normalize(this->position - boid.position) / glm::distance(boid.position, this->position);
//         totalForce += separationForce;
//     }
//     if (glm::length(totalForce) > 0)
//     {
//         totalForce = normalize(totalForce);
//     }
//     return totalForce;
