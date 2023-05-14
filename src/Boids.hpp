#pragma once
#include <cstdlib>
#include <utility>
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
    float visualRange;

    void updateBoidsParam()
    {
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 3.f);
        ImGui::SliderFloat("Visual Range", &this->visualRange, 0.f, 3.f);
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
    int               m_numBoids{};

    /*Methods*/
    // draw the boid
    void drawBoids(p6::Context& ctx, BoidsParameters& boidParam);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, BoidsParameters& boidParam);

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    static glm::vec2 separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec2 alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec2 cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

    // apply the 3 rules(separation, alignment, cohesion)
    static void applySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

public:
    Boids() = default;

    // constructor
    Boids(std::vector<Boid> boids, const int& numBoids)
        : m_boids(std::move(boids)), m_numBoids(numBoids){};

    // initialize m_boids
    void fillBoids(p6::Context& ctx);

    // update boids' position, direction
    void updateBoids(p6::Context& ctx, BoidsParameters& boidParam);
};
