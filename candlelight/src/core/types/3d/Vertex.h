#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace candle::core {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        Vertex(glm::vec3 position = glm::vec3(0, 1, 0), glm::vec3 normal = glm::vec3(0, 1, 0), glm::vec2 uv = glm::vec2(0, 0)) {
            this->position = position;
            this->normal = normal;
            this->uv = uv;
        }
    };

}
