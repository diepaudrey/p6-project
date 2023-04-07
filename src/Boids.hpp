#pragma once
#include <cstdlib>
#include <vector>
#include "Boid.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boids {
private:
    /*Attributes*/
    std::vector<Boid> m_boids;
    int               m_numBoids;

    // glm::vec2 m_position;
    // glm::vec2 m_speed;

    /*ImGui variables*/
    // float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids)
        : m_boids(boids), m_numBoids(numBoids){};

    void fillBoids(p6::Context& ctx);

    /*Setters for ImGui*/
    void setProtectedRadius(const float& protecRad)
    {
        for (auto& boid : m_boids)
        {
            boid.protectedRadius = protecRad;
        }
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

    void setMaxSpeed(const float& maxSpeed)
    {
        for (auto& boid : m_boids)
        {
            boid.maxSpeed = maxSpeed;
        }
    }

    // draw the boid
    void drawBoids(p6::Context& ctx);

    // Help the boids to avoid edges
    void avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, p6::Context& ctx);

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    glm::vec2 separation(Boid& boid);
    glm::vec2 alignment(const Boid& boid) const;
    glm::vec2 cohesion(Boid& boid);

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForce(Boid& boid);
    // void updatePosition(p6::Context& ctx);

    void updateBoids(p6::Context& ctx)
    {
        float turnfactor = 0.3f;
        for (auto& boid : m_boids)
        {
            std::vector<Boid> neighbors = fillNeighbors(boid, ctx);
            boid.updatePosition(ctx);
            applySteeringForce(boid);
            avoidEdges(boid, ctx, turnfactor);
            boid.draw(ctx);
            neighbors.clear();
        }
    };
};
