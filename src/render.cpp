#include "vox.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

// Scalar field implem

// This is not the line you're looking for
static RenderObject objects[10000];
static int stack_top = 0;

int render_new_object() {
    int handle = stack_top;
    stack_top++;
    // RenderObject& o = objects[handle];
    // o.vertices = std::vector<float>();
    // o.indices = std::vector<float>();
    // o.colors = std::vector<float>();
    return handle;
}

void render_setup() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Very ugly, but it's okay, we're all going to die anyway
void render_register_shaders(int handle, std::string path_v,
                             std::string path_f) {
    Shader sv, sf;
    RenderObject &obj = objects[handle];
    sv.path = path_v;
    sf.path = path_f;

    try {
        sv.source = get_shader_source(path_v);
        sf.source = get_shader_source(path_f);
        shader_compile(sv, GL_VERTEX_SHADER);
        shader_compile(sf, GL_FRAGMENT_SHADER);
    } catch (ShaderCreationException &e) {
        std::cerr << e.what() << std::endl;
        // one day, when I'm grown up, I will manage errors properly
        exit(1);
    }
    obj.sp.id = glCreateProgram();
    obj.sp.shaders.push_back(sv);
    obj.sp.shaders.push_back(sf);
    if (shader_program_link(obj.sp)) {
        exit(1);
    }
}
// code to update render target if needed
void render_update_objects(int handle, Object &obj) {
    RenderObject &robj = objects[handle];

    // copy the vbos data
    robj.vertices = obj.m.vertices;
    robj.indices = obj.m.indices;
    robj.colors = obj.m.colors;

    GLuint vertices;
    glGenBuffers(1, &vertices);

    unsigned int colors;
    glGenBuffers(1, &colors);

    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);

    glBufferData(GL_ARRAY_BUFFER, robj.vertices.size() * sizeof(float),
                 robj.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colors);
    glBufferData(GL_ARRAY_BUFFER, robj.colors.size() * sizeof(float),
                 robj.colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO);

    robj.vao = VAO;
    robj.vbo_v = vertices;
    robj.vbo_c = colors;
}

void render_clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_object(Object &obj, Context &ctx) {

    int handle = obj.handle;
    RenderObject &robj = objects[handle];

    glBindVertexArray(robj.vao);
    glUseProgram(robj.sp.id);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, obj.pos);
    model = glm::scale(model, obj.scale);
    int modelLoc = glGetUniformLocation(robj.sp.id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    modelLoc = glGetUniformLocation(robj.sp.id, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

    modelLoc = glGetUniformLocation(robj.sp.id, "view");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                       glm::value_ptr(ctx.camera.view()));
    int opacityLoc = glGetUniformLocation(robj.sp.id, "opacity");
    glUniform1f(opacityLoc, obj.opacity);

    glDrawArrays(GL_TRIANGLES, 0, robj.vertices.size() / 3);
}
