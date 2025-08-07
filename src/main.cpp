#include "vox.h"
#include <GLFW/glfw3.h>
#include <complex>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void fatal() {
    std::cerr << "Fatal error" << std::endl;
    exit(1);
}

Context populate_scene(Context &ctx) {

    ctx.camera = Camera(glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 0.0, 1.0),
                        glm::vec3(0.0, 1.0, 0.0));

    // Adding objects that will appear in scene
    // ctx.objs.push_back(
    //     build_cube(glm::vec3(0.0, 0.0, 5.0), glm::vec3(1.0, 1.0, 1.0)));

    int dim = 15;

    std::vector<float> chunk(0, 15 * 15 * 15);

    int n = 10;
    field_setup(ctx, n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            for (int z = 0; z < n; z++) {
                glm::vec3 pos = ctx.f.pos + glm::vec3(x, y, z);
                float val = field_query(ctx.f, x, y, z);
                glm::vec3 color = glm::vec3(val, val, val);
                ctx.objs.push_back(
                    build_unicolor_cube(pos, glm::vec3(0.1, 0.1, 0.1), color));
            }
        }
    }
    return ctx;
}

void game_loop(Context &ctx) {

    // Setup all the rendering
    render_setup();
    for (auto it = ctx.objs.begin(); it != ctx.objs.end(); it++) {
        it->handle = new_render_object();
        render_register_shaders(it->handle, it->sv, it->sf);
    }

    while (!glfwWindowShouldClose(ctx.window)) {
        // Update state based on input
        process_input(ctx);

        // Update render objects if needed
        for (auto it = ctx.objs.begin(); it != ctx.objs.end(); it++) {
            if (it->m.dirty) {
                render_update_objects(it->handle, *it);
                it->m.dirty = false;
            }
        }
        // clean up the screen
        render_clear();

        // Render objects
        for (auto it = ctx.objs.begin(); it != ctx.objs.end(); it++) {
            render_object(*it, ctx);
        }

        glfwSwapBuffers(ctx.window);
        glfwPollEvents();
    }

    //     Context ctx;
}

int main() {
    // Initialise window
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "voxel opengl", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Context ctx;
    populate_scene(ctx);
    ctx.window = window;
    setup_input(ctx);

    game_loop(ctx);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
