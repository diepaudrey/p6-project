#pragma once
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 position;
    glm::vec2 speed;

    float maxSpeed = 1.2f;

    std::vector<Boid> neighbors;

    float turnfactor = 0.15f;
    float protectedRadius;
    float visibleRange = 0.2f;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;

public:
    Boid(){};
    Boid(const glm::vec2& pos, const glm::vec2 vel)
        : position(pos), speed(vel){};

    /*Setters*/
    void setProtectedRadius(const float& protectedRadius)
    {
        this->protectedRadius = protectedRadius;
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

    void setSpeed(const float& velocity)
    {
        this->speed = glm::vec2(velocity, velocity);
    }

    static void setMaxSpeed(glm::vec2 vec, const float& maxSpeed)
    {
        if (glm::length(vec) > maxSpeed)
        {
            vec = glm::normalize(vec) * maxSpeed;
        }
    }

    void setMaxSpeed(const float& maxSpeed)
    {
        if (glm::length(this->speed) > maxSpeed)
        {
            this->speed = glm::normalize(this->speed) * maxSpeed;
        }
    }

    // draw the boid
    void draw(p6::Context& ctx);

    // check if the position of the boid is out of the window
    bool isOutWindow(const p6::Context& ctx) const;
    // update the direction if the boid is out of the window
    void updateDirectionBorders(const p6::Context& ctx);

    void avoidEdges(const p6::Context& ctx, const float& turnfactor);

    // check distance between this boid and the boid in argument
    bool isTooClose(const Boid& boid, const float& radius) const;
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const std::vector<Boid>& boids, p6::Context& ctx) const;
    // use to draw a red circle when boids are too close
    static void drawCollision(const std::vector<Boid>& neighbors, p6::Context& ctx);

    /*3 rules*/
    glm::vec2        calculateSeparationForce(const std::vector<Boid>& neighbors);
    static glm::vec2 calculateAlignmentForce(const std::vector<Boid>& neighbors);
    static glm::vec2 calculateCohesionForce(const std::vector<Boid>& neighbors);

    void      separation(const std::vector<Boid>& boid);
    glm::vec2 alignment();

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForce();

    void update(p6::Context& ctx, const std::vector<Boid>& boids)
    {
        this->neighbors = fillNeighbors(boids, ctx);

        this->position += ctx.delta_time() * this->speed;

        // separation(boids);
        applySteeringForce();

        // std::cout << this->speed.x << " " << this->speed.y << std::endl;
        // updateDirectionBorders(ctx);
        avoidEdges(ctx, this->turnfactor);
        draw(ctx);

        this->neighbors.clear();
    };
};
