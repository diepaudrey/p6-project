#include "boid.hpp"

void Boid::draw(p6::Context& ctx)
{
    // p6::Point2D p1(this->position.x, 0.2f);
    // p6::Point2D p2(this->position.x + 0.2f, this->position.y - 0.2f);
    // p6::Point2D p3(this->position.x - 0.2f, this->position.y - 0.2f);

    ctx.equilateral_triangle(
        p6::Center{this->position},
        p6::Radius{0.2f},
        p6::Rotation{this->direction}
    );
}

bool Boid::isOutWindow(p6::Context& ctx) const
{
    return this->position.x > ctx.aspect_ratio() || this->position.y > ctx.aspect_ratio() || this->position.x < -ctx.aspect_ratio() || this->position.y < -ctx.aspect_ratio();
}