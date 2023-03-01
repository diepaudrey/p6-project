// #include <stdlib.h>
#include <cstdlib>
#include "p6/p6.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "boid.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "p6-project"}};
    ctx.maximize_window();

    // Variable declaration
    // p6::Angle rotation = 0.011_turn;

    // Initialize a boid
    glm::vec2 p(0, 0);
    glm::vec2 v(1, 1);
    Boid      b1(p, v);

    std::vector<Boid> boids;
    int               nb_boids = 10;

    for (int i = 0; i < nb_boids; i++)
    {
        glm::vec2 pos = p6::random::point(ctx);
        glm::vec2 dir = p6::random::point(ctx);
        Boid      boid(pos, dir);
        boids.push_back(boid);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::PastelBlue);

        for (size_t i = 0; i < boids.size(); i++)
        {
            boids[i].draw(ctx);
            boids[i].update(ctx);
            boids[i].updateDirection(ctx);
        }

        // for (unsigned i = 0; i < 10; i++)
        // {
        //     ctx.square(
        //         p6::TopLeftCorner{p6::random::point(ctx)},
        //         p6::Radius{0.1f},
        //         p6::Rotation{rotation}
        //     );

        //     ctx.rectangle(
        //         p6::TopLeftCorner{p6::random::point(ctx)},
        //         p6::Radii{glm::vec2(0.1f, 0.2f)},
        //         p6::Rotation{rotation}
        //     );
        // }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}