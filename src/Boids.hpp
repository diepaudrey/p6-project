#pragma once
#include <cstdlib>
#include <vector>
#include "Boid.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

struct BoidsParameters {
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float maxSpeed;

    void updateBoidsParam()
    {
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 3.f);
        ImGui::SliderFloat("Separation Strength", &this->separationStrength, 0.f, 1.f);
        ImGui::SliderFloat("Alignment Strength", &this->alignmentStrength, 0.f, 1.f);
        ImGui::SliderFloat("Cohesion Strength", &this->cohesionStrength, 0.f, 1.f);
        ImGui::SliderFloat("Max Speed", &this->maxSpeed, 0.1f, 10.f);
        ImGui::End();
    }
};

class Boids {
private:
    /*Attributes*/
    std::vector<Boid> m_boids;
    int               m_numBoids;

    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids, BoidsParameters& boidParam)
        : m_boids(boids), m_numBoids(numBoids){};

    void fillBoids(p6::Context& ctx);

    // draw the boid
    void drawBoids(p6::Context& ctx, BoidsParameters& boidParam);

    // Help the boids to avoid edges
    void avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor, BoidsParameters& boidParam);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, p6::Context& ctx, BoidsParameters& boidParam);

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    glm::vec2 separation(const Boid& boid, BoidsParameters& boidParam) const;
    glm::vec2 alignment(const Boid& boid, BoidsParameters& boidParam) const;
    glm::vec2 cohesion(const Boid& boid, BoidsParameters& boidParam) const;

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForces(Boid& boid, BoidsParameters& boidParam);

    // update boids' position, direction
    void updateBoids(p6::Context& ctx, BoidsParameters& boidParam);
};
