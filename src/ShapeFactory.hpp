#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "Shapes.hpp"

using namespace Shapes;

class ShapeFactory
{
public:
    enum ShapeType
    {
        SQUARE,
        HEXAGON,
        DIAMOND,
        STAR
    };
    static Shape *CreateShape(ShapeType type, const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
};
