#include "vox.h"
#include <math.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define MAX_STEPS 100

// Do not look at the code, it isn't worth your time
bool raycast(Field &f, glm::vec3 pos, glm::vec3 v, glm::vec3 &result) {
    glm::vec3 dir = glm::normalize(v);

    pos = pos - f.pos;

    for (int i = 0; i < MAX_STEPS; i++) {
        pos += v;
        // Causes weird behaviour, too dumb to find a better way, sorry...
        glm::vec3 curr = glm::floor(pos);
        if (curr.x < 0 || curr.y < 0 || curr.z < 0)
            continue;
        if (curr.x >= f.len || curr.y >= f.len || curr.z >= f.len)
            break;
        if (field_query(f, curr) > 0.0f) {
            result = pos + f.pos;
            return true;
        }
    }
    return false;
}
