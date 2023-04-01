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

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);

    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 1.2f;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*Dear ImGui*/
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Protected Radius", &protectedRadius, 0.f, 2.f);
        ImGui::SliderFloat("Separation Strength", &separationStrength, 0.f, 1.f);
        ImGui::SliderFloat("Alignment Strength", &alignmentStrength, 0.f, 1.f);
        ImGui::SliderFloat("Cohesion Strength", &cohesionStrength, 0.f, 1.f);
        ImGui::SliderFloat("Max Speed", &maxSpeed, 0.f, 5.f);
        //  ImGui::SliderFloat("speed", &speed, 0.f, 10.f);
        ImGui::End();

        ctx.background(p6::NamedColor::DavySGrey);

        game.setProtectedRadius(protectedRadius);
        game.setAlignmentStrength(alignmentStrength);
        game.setCohesionStrength(cohesionStrength);
        game.setSeparationStrength(separationStrength);
        game.setMaxSpeed(maxSpeed);

        game.updateBoids(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}