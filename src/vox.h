#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

//
//
//  WATCH OUT: the engine architecture is terrible, just the required minimum,
//  everything is bundled together. I've yet to go through the 600+ episodes of
//  Handmade Hero...
//
//

#define ERROR -1;
#define OK 0;

struct Shader {
    std::string source;
    GLuint id;
    std::string path;
    GLuint type;
};

class ShaderCreationException : public std::exception {
  public:
    char *what() { return (char*)"Shader creation failed"; }
};

struct ShaderProgram {
    std::vector<Shader> shaders;
    GLuint id;
};

struct Mesh {
    std::vector<float> vertices;
    std::vector<float> indices;
    std::vector<float> colors;
    bool dirty;
};

struct Object {
    Mesh m;
    glm::vec3 pos;
    int handle;
    // paths, pretend there is a pretty material abstraction
    std::string sv;
    std::string sf;
};

// rendering info structure
// referred to by a handle
struct RenderObject {
    int vao;
    int vbo_v;
    int vbo_c;
    int ebo;

    ShaderProgram sp;
    std::vector<float> vertices;
    std::vector<float> indices;
    std::vector<float> colors;
};

// Minimal camera class
class Camera {
  public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 left;

    Camera();
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 left);
    glm::mat4 view();
};

// Object that holds the voxel data
struct Field {
    // dimension of a side, len * len * len voxels
    int len;
    // Center of the cunk, center point of the cube if you will
    glm::vec3 pos;
    std::vector<int> data;
};

// Top level structure
struct Context {
    GLFWwindow *window;
    Camera camera;
    std::vector<Object> objs;
};

// object.cpp, code to build different objects
Object build_cube();

// shader.cpp
// code to create and compile shaderprograms
std::string get_shader_source(std::string path);
void shader_compile(Shader& s, GLuint type);
int shader_program_link(ShaderProgram &);

// render.cpp
// code to register shaders
// get a new render object handle
void render_setup();
int new_render_object();
void render_register_shaders(int handle, std::string path_v, std::string path_f);
// code to update render target if needed
void render_update_objects(int handle, Object &);
// code to render different objects
void render_object(int handle, Context&);
void render_clear();

// Input API
void setup_input(Context &ctx);
void process_input(Context &ctx);
