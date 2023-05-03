// #include <stdlib.h>
#include <cstdlib>
#include "p6/p6.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "Boids.hpp"
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
    std::vector<Boid> boids;
    int               nb_boids = 25;

    BoidsParameters boidsParam;
    boidsParam.protectedRadius    = 0.1f;
    boidsParam.separationStrength = 0.1f;
    boidsParam.alignmentStrength  = 0.1f;
    boidsParam.cohesionStrength   = 0.1f;
    boidsParam.maxSpeed           = 1.2f;
    boidsParam.visualRange        = 0.3f;

    Boids game(boids, nb_boids, boidsParam);
    game.fillBoids(ctx);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::DavySGrey);
        game.updateBoids(ctx, boidsParam);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}