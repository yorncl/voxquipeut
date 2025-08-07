#include "vox.h"
#include <GLFW/glfw3.h>

bool input[500] = {false};

void process_keys(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }

        if (key == GLFW_KEY_W)
            input[GLFW_KEY_W] = true;
        if (key == GLFW_KEY_S)
            input[GLFW_KEY_S] = true;
        if (key == GLFW_KEY_A)
            input[GLFW_KEY_A] = true;
        if (key == GLFW_KEY_D)
            input[GLFW_KEY_D] = true;
        if (key == GLFW_KEY_SPACE)
            input[GLFW_KEY_SPACE] = true;
        if (key == GLFW_KEY_LEFT_CONTROL)
            input[GLFW_KEY_LEFT_CONTROL] = true;
    }
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W)
            input[GLFW_KEY_W] = false;
        if (key == GLFW_KEY_S)
            input[GLFW_KEY_S] = false;
        if (key == GLFW_KEY_A)
            input[GLFW_KEY_A] = false;
        if (key == GLFW_KEY_D)
            input[GLFW_KEY_D] = false;
        if (key == GLFW_KEY_SPACE)
            input[GLFW_KEY_SPACE] = false;
        if (key == GLFW_KEY_LEFT_CONTROL)
            input[GLFW_KEY_LEFT_CONTROL] = false;
    }
}

void process_input(Context &ctx) {
    float speed = 0.3f;
    if (input[GLFW_KEY_W])
        ctx.camera.pos += ctx.camera.front * speed;
    if (input[GLFW_KEY_S])
        ctx.camera.pos -= ctx.camera.front * speed;
    if (input[GLFW_KEY_A])
        ctx.camera.pos += ctx.camera.left * speed;
    if (input[GLFW_KEY_D])
        ctx.camera.pos -= ctx.camera.left * speed;
    if (input[GLFW_KEY_SPACE])
        ctx.camera.pos += ctx.camera.up * speed;
    if (input[GLFW_KEY_LEFT_CONTROL])
        ctx.camera.pos -= ctx.camera.up * speed;
}

void setup_input(Context& ctx) {
    glfwSetKeyCallback(ctx.window, process_keys);
}
