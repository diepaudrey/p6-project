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

    std::vector<Boid> boids;
    int               nb_boids = 25;

    float protectedRadius    = 0.1f;
    float separationStrength = 2.f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;

    for (int i = 0; i < nb_boids; i++)
    {
        glm::vec2 pos   = p6::random::point(ctx);
        glm::vec2 speed = pos + p6::random::point(ctx);
        Boid      boid(pos, speed);

        boids.push_back(boid);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*Dear ImGui*/
        ImGui::Begin("Settings");
        ImGui::SliderFloat("ProtectedRadius", &protectedRadius, 0.f, 2.f);
        ImGui::SliderFloat("separationStrength", &separationStrength, 0.f, 15.f);
        ImGui::SliderFloat("alignmentStrength", &alignmentStrength, 0.f, 1.f);
        ImGui::SliderFloat("cohesionStrength", &cohesionStrength, 0.f, 1.f);
        // ImGui::SliderFloat("speed", &speed, 0.f, 10.f);
        ImGui::End();

        ctx.background(p6::NamedColor::DavySGrey);

        // for (size_t i = 0; i < boids.size(); i++)
        // {
        //     boids[i].update(ctx, boids);
        // }

        for (auto& boid : boids)
        {
            boid.setProtectedRadius(protectedRadius);
            boid.setAlignmentStrength(alignmentStrength);
            boid.setCohesionStrength(cohesionStrength);
            boid.setSeparationStrength(separationStrength);
            // boid.setSpeed(speed);

            boid.update(ctx, boids);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}