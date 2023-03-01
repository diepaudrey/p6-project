#pragma once
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 position;
    glm::vec2 direction;

public:
    Boid(){};
    Boid(const glm::vec2& pos, const glm::vec2& dir)
        : position(pos), direction(dir){};

    void update(p6::Context& ctx)
    {
        this->position += 0.01f * this->direction;
    };

    void draw(p6::Context& ctx);
    bool isOutWindow(p6::Context& ctx);
    void updateDirection(p6::Context& ctx);
};
