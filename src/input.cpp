#include "vox.h"
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>

bool input[500] = {false};
bool mouse_click = false;

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

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouse_click = true;
}

static bool firstMouse = true;
static double lastX = 0;
static double lastY = 0;
float pitch = 0;
float yaw = 0;

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

    if (mouse_click) {
        glm::vec3 hit;
        if (raycast(ctx.f, ctx.camera.pos, ctx.camera.front, hit))
        {
            printf("Raycast hit world coord: %f %f %f\n", hit.x, hit.y, hit.z);
            hit -= ctx.f.pos;
            printf("Raycast hit field coord: %f %f %f\n", hit.x, hit.y, hit.z);

            int nzeros = 0;
            for (int i = 0; i < ctx.f.data.size(); i++){
                if (ctx.f.data[i] == 0)
                    nzeros++;
            }
            std::cout << "nzeroes before " << nzeros << std::endl;
            field_fill_sphere(ctx.f, hit, 3);
            for (int i = 0; i < ctx.f.data.size(); i++){
                if (ctx.f.data[i] == 0)
                    nzeros++;
            }
            std::cout << "nzeroes after " << nzeros << std::endl;

            Object new_terrain = marching_mesh(ctx.f);

            // TODO very naughty to do this in the input code...
            for (auto it = ctx.objs.begin(); it != ctx.objs.end(); it++) {
                if (it->id == ctx.f.object_id) {
                    // TODO not clean, the new_terrain will be dropped here, taking its id with him
                    it->m = new_terrain.m;
                }
            }
            // new_terrain.handle = new_render_object();
            // new_terrain.m.dirty = true;
            // // new_terrain.pos -= glm::vec3(-10.0, 0.0, 0.0);
            // render_register_shaders(new_terrain.handle, new_terrain.sv, new_terrain.sf);
            // ctx.objs.push_back(new_terrain);
             
            // Object cube = build_cube(hit + ctx.f.pos, glm::vec3(1.0, 1.0, 1.0));
            // cube.handle = new_render_object();
            // render_register_shaders(cube.handle, cube.sv, cube.sf);
            // ctx.objs.push_back(cube);
        }
        mouse_click = 0;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    ctx.camera.front = glm::normalize(direction);

    ctx.camera.left = glm::cross(glm::vec3(0.0, 1.0, 0.0), ctx.camera.front);
    ctx.camera.up = glm::cross(ctx.camera.front, ctx.camera.left);
}

// code from https://learnopengl.com/Getting-started/Camera
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void setup_input(Context &ctx) {
    glfwSetKeyCallback(ctx.window, process_keys);
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(ctx.window, mouse_callback);
    glfwSetMouseButtonCallback(ctx.window, mouse_button_callback);
}
