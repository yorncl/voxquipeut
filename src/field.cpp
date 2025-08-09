#include "vox.h"
#include <cassert>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <math.h>
#include <string.h>

inline bool out_of_bounds(int field_side, int x, int y, int z) {
    return (x < 0 || y < 0 || z < 0) ||
           (x > field_side || y > field_side || z > field_side);
}

// Fill a sphere at given coordinates by iterating on its bounding box
void field_fill_sphere(Field &f, glm::vec3 pos, float radius) {

    std::vector<float> &grid = f.data;
    // TODO fix, not clear, I might have hit my head with all those floors and
    // ceilings
    int side = ceil(radius) * 2 + 3;
    // start of the bounding box
    glm::vec3 start = glm::floor(pos - glm::vec3(side / 2, side / 2, side / 2));
    glm::vec3 end = glm::ceil(start + glm::vec3(side, side, side));

    // int center = x * side * side + y * side + z;
    for (int x = start.x; x < end.x; x++) {
        for (int y = start.y; y < end.y; y++) {
            for (int z = start.z; z < end.z; z++) {
                if (out_of_bounds(f.len, x, y, z))
                    continue;
                float l = glm::length(glm::vec3(x, y, z) - pos);
                float dist_iso = radius - l;
                int index = x * f.len * f.len + y * f.len + z;
                // inside
                if (dist_iso > 0) {
                    // if the length is > 1.0, then we do not care about fine
                    // distance to the surface, just say the distance is one
                    grid[index] = dist_iso > 1.0 ? 1.0 : dist_iso;

                } else {
                    // outside
                    if (dist_iso > -1.0 && grid[index] == -1)
                        grid[index] = dist_iso;
                }
            }
        }
    }
}

void field_fill_cuboid(Field &f, glm::vec3 start, int l, int w, int h) {
    glm::vec3 end = start + glm::vec3(l, h, w);
    // int center = x * side * side + y * side + z;
    for (int x = start.x; x < end.x; x++) {
        for (int y = start.y; y < end.y; y++) {
            for (int z = start.z; z < end.z; z++) {
                int index = x * f.len * f.len + y * f.len + z;
                f.data[index] = 1.0;
            }
        }
    }
}

void field_fill_default(Field &f) {
    int l = 90;
    int w = 90;
    field_fill_cuboid(f, glm::vec3(f.len/2 - l/2, 30, f.len/2 - w/2), l, w, 10);
}

void field_setup(Field &f, int n, glm::vec3 pos) {
    f.pos = pos;
    f.len = n;
    f.data.assign(n * n * n, -1.0f);
}

void field_clear(Field &f) {
    memset(f.data.data(), 0, f.data.size() * sizeof(float));
}

float field_query(Field &f, glm::vec3 &coord) {
    int side = f.len;
    int index = coord.x * side * side + coord.y * side + coord.z;
    assert(index < side * side * side);
    return f.data[index];
}
