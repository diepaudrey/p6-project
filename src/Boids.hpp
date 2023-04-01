#pragma once
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boids {
private:
    /*Attributes*/
    glm::vec2 m_position;
    glm::vec2 m_speed;

    /*ImGui variables*/
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float maxSpeed;

public:
    Boids() = default;
    Boids(const glm::vec2& pos, const glm::vec2 vel)
        : m_position(pos), m_speed(vel){};

    /*Setters for ImGui*/
    void setProtectedRadius(const float& protecRad)
    {
        this->protectedRadius = protecRad;
    }

    void setSeparationStrength(const float& separation)
    {
        this->separationStrength = separation;
    }

    void setAlignmentStrength(const float& alignment)
    {
        this->alignmentStrength = alignment;
    }

    void setCohesionStrength(const float& cohesion)
    {
        this->cohesionStrength = cohesion;
    }

    void setMaxSpeed(const float& speed)
    {
        this->maxSpeed = speed;
    }

    // limit the speed
    void limitSpeed()
    {
        if (glm::length(m_speed) > maxSpeed)
        {
            m_speed = glm::normalize(m_speed) * maxSpeed;
        }
    }

    // draw the boid
    void draw(p6::Context& ctx);

    // Help the boids to avoid edges
    void avoidEdges(const p6::Context& ctx, const float& turnfactor);

    // check distance between this boid and the boid in argument
    bool isTooClose(const Boids& boid, const float& radius) const;
    // fill a vector of the neighbor

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    glm::vec2 separation(const std::vector<Boids>& boid);
    glm::vec2 alignment(const std::vector<Boids>& boids) const;
    glm::vec2 cohesion(const std::vector<Boids>& boids);

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForce(const std::vector<Boids>& boids);
    void updatePosition(p6::Context& ctx);

    void updateBoids(p6::Context& ctx, const std::vector<Boids>& boids)
    {
        float turnfactor = 0.3f;
        updatePosition(ctx);
        applySteeringForce(boids);
        avoidEdges(ctx, turnfactor);
        displayCollision(boids, ctx);
        draw(ctx);
    };
};
