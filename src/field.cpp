#include "vox.h"
#include <cassert>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

void field_fill_sphere(Field &f, glm::vec3 pos, float radius) {

    int side = f.len;
    for (int x = 0; x < side; x++) {
        for (int y = 0; y < side; y++) {
            for (int z = 0; z < side; z++) {
                glm::vec3 vox_pos = glm::vec3(x, y, z);
                if (glm::length(vox_pos - pos) <= radius) {
                    int index = x * side * side + y * side + z;
                    assert(index < side * side * side);
                    f.data[index] = 1;
                }
            }
        }
    }
}

void field_setup(Context &ctx, int n, glm::vec3 pos) {

    Field &f = ctx.f;

    f.pos = pos;
    f.len = n;
    f.data.assign(n * n * n, 0);
    field_fill_sphere(f, glm::vec3(n / 2, n / 2, n / 2), (float)7);
}

int field_query(Field &f, int x, int y, int z) {
    int side = f.len;
    int index = x * side * side + y * side + z;
    assert(index < side * side * side);
    return f.data[index];
}
