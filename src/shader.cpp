#include "vox.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

std::string get_shader_source(std::string path) {
    FILE *f = fopen(path.c_str(), "r");
    if (!f) {
        std::cerr << "Failed to open shader file: \"" << path << "\""
                  << std::endl;
        throw ShaderCreationException();
    }

    struct stat stats;
    if (stat(path.c_str(), &stats)) {

        std::cerr << "Cannot access file information: \"" << path << "\""
                  << std::endl;
        perror("stat()");
        throw ShaderCreationException();
    }

    char *buff = (char *)calloc(1, stats.st_size + 1);

    if (fread(buff, stats.st_size, 1, f) == -1) {
        std::cerr << "Cannot read shader file" << path << "\"" << std::endl;
        throw ShaderCreationException();
    }

    std::string source = std::string(buff);
    free(buff);
    return source;
}

void shader_compile(Shader& s, GLuint type) {
    int success;
    char infoLog[512];
    int id = glCreateShader(type);
    char *ptr = s.source.data();

    glShaderSource(id, 1, &ptr, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cerr << "Shader Compilation failed: " << s.path << std::endl;
        std::cerr << infoLog << std::endl;
        throw ShaderCreationException();
    }
    s.id = id;
}

int shader_program_link(ShaderProgram &sp) {
    for (auto it = sp.shaders.begin(); it != sp.shaders.end(); it++) {
        glAttachShader(sp.id, it->id);
    }
    glLinkProgram(sp.id);

    int success;
    char infoLog[512];
    glGetProgramiv(sp.id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(sp.id, 512, NULL, infoLog);
        std::cout << "Shader program linking failed" << std::endl
                  << infoLog << std::endl;
        return ERROR;
    }
    return OK;
}
