#include "vox.h"
#include <cassert>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

static void fill_sphere(Field &f, glm::vec3 pos, float radius) {

    int side = f.len;
    for (int x = 0; x < side; x++) {
        for (int y = 0; y < side; y++) {
            for (int z = 0; z < side; z++) {
                glm::vec3 vpos =
                    glm::vec3(-side / 2 + x, -side / 2 + y, -side / 2 + z);
                f.data[x * side * side + y * side + z] =
                    glm::length(vpos - pos) <= radius ? 1.0 : 0.0;
            }
        }
    }
}

void field_setup(Context &ctx, int n, glm::vec3 pos) {

    Field &f = ctx.f;

    f.pos = pos;
    f.len = n;
    f.data.assign(n * n * n, 0);
    fill_sphere(f, glm::vec3(0.0, 0.0, 0.0), (float)n / 3);
}

int field_query(Field &f, int x, int y, int z) {
    int side = f.len;
    int index = x * side * side + y * side + z;
    assert(index < side * side * side);
    return f.data[index];
}
