#include "ShapeFactory.hpp"

using namespace Shapes;

Shape *ShapeFactory::CreateShape(ShapeType type, const std::string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
{
    switch (type)
    {
    case ShapeType::SQUARE:
        return new Square(name, center, radius, color, fill);
    case ShapeType::HEXAGON:
        return new Hexagon(name, center, radius, color, fill);
    case ShapeType::DIAMOND:
        return new Diamond(name, center, radius, color, fill);
    case ShapeType::STAR:
        return new Star(name, center, radius, color, fill);
    default:
        return nullptr;
    }
}
