#include "boid.hpp"

void Boid::draw(p6::Context& ctx)
{
    ctx.equilateral_triangle(
        p6::Center{this->position},
        p6::Radius{0.f},
        p6::Rotation{this->direction}
    );
}

bool Boid::isOutWindow(p6::Context& ctx)
{
    return this->position.x > ctx.aspect_ratio() || this->position.y > ctx.aspect_ratio() || this->position.x < -ctx.aspect_ratio() || this->position.y < -ctx.aspect_ratio();
}

void Boid::updateDirection(p6::Context& ctx)
{
    if (this->isOutWindow(ctx) == true)
    {
        this->direction = -(this->direction);
    }
    else
    {
        this->direction += 0.01f;
    }
}