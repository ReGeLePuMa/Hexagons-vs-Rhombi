#include "Shapes.hpp"

namespace Shapes
{
    Shape::Shape(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
    {
        this->name = name;
        this->center = center;
        this->radius = radius;
        this->color = color;
        this->fill = fill;
        this->angle = 0;
        this->elapsedTime = 0;
        this->hasCollided = false;
    }

    Shape::~Shape()
    {
        delete this->mesh;
    }

    string Shape::GetMeshName()
    {
        return this->name;
    }

    glm::vec3 Shape::GetCenter()
    {
        return this->center;
    }

    glm::vec3 Shape::GetColor()
    {
        return this->color;
    }

    Mesh *Shape::GetMesh()
    {
        return this->mesh;
    }

    float Shape::GetRadius()
    {
        return this->radius;
    }

    float Shape::GetAngle()
    {
        return this->angle;
    }

    double Shape::GetElapsedTime()
    {
        return this->elapsedTime;
    }

    void Shape::SetElapsedTime(double elapsedTime)
    {
        this->elapsedTime = elapsedTime;
    }

    void Shape::SetAngle(float angle)
    {
        this->angle = angle;
    }

    bool Shape::GetFill()
    {
        return this->fill;
    }

    bool Shape::HasCollided()
    {
        return this->hasCollided;
    }

    void Shape::Collided(bool hasCollided)
    {
        this->hasCollided = hasCollided;
    }

    float Shape::GetScaleFactor()
    {
        return this->scaleFactor;
    }

    void Shape::SetScaleFactor(float scaleFactor)
    {
        this->scaleFactor = scaleFactor;
    }

    void Shape::TranslateCenter(float x, float y)
    {
        this->center = glm::vec3(x, y, 0);
    }

    void Shape::ScaleRadius(float scale)
    {
        this->radius *= scale;
    }

    Square::Square(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill) : Shape(name, center, radius, color, fill)
    {
        this->CreateMesh();
    }

    Square::~Square()
    {
    }

    void Square::CreateMesh()
    {
        vector<VertexFormat> vertices =
            {
                VertexFormat(this->center + glm::vec3(-this->radius, -this->radius, 0), this->color),
                VertexFormat(this->center + glm::vec3(this->radius, -this->radius, 0), this->color),
                VertexFormat(this->center + glm::vec3(this->radius, this->radius, 0), this->color),
                VertexFormat(this->center + glm::vec3(-this->radius, this->radius, 0), this->color)};

        vector<unsigned int> indices = {0, 1, 2, 3};
        this->mesh = new Mesh(this->name);
        if (!this->fill)
        {
            this->mesh->SetDrawMode(GL_LINE_LOOP);
        }
        else
        {
            indices.push_back(0);
            indices.push_back(2);
        }
        this->mesh->InitFromData(vertices, indices);
    }

    Hexagon::Hexagon(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill) : Shape(name, center, radius, color, fill)
    {
        this->CreateMesh();
    }

    Hexagon::~Hexagon()
    {
    }

    void Hexagon::CreateMesh()
    {
        static constexpr float PI = 3.141592f;
        auto inv_color = glm::vec3(1, 1, 1) - this->color;
        vector<VertexFormat> vertices =
            {
                VertexFormat(this->center + glm::vec3(0, 0, 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(PI / 3 + PI / 6), (this->center.y + radius) * cos(PI / 3 + PI / 6), 1), this->color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(2 * PI / 3 + PI / 6), (this->center.y + radius) * cos(2 * PI / 3 + PI / 6), 1), this->color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(3 * PI / 3 + PI / 6), (this->center.y + radius) * cos(3 * PI / 3 + PI / 6), 1), this->color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(4 * PI / 3 + PI / 6), (this->center.y + radius) * cos(4 * PI / 3 + PI / 6), 1), this->color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(5 * PI / 3 + PI / 6), (this->center.y + radius) * cos(5 * PI / 3 + PI / 6), 1), this->color),
                VertexFormat(glm::vec3((this->center.x + radius) * sin(6 * PI / 3 + PI / 6), (this->center.y + radius) * cos(6 * PI / 3 + PI / 6), 2), this->color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(PI / 3 + PI / 6), 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(2 * PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(2 * PI / 3 + PI / 6), 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(3 * PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(3 * PI / 3 + PI / 6), 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(4 * PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(4 * PI / 3 + PI / 6), 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(5 * PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(5 * PI / 3 + PI / 6), 2), inv_color),
                VertexFormat(glm::vec3((this->center.x + radius * 0.6f) * sin(6 * PI / 3 + PI / 6), (this->center.y + radius * 0.6f) * cos(6 * PI / 3 + PI / 6), 2), inv_color)};

        vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1,
            0, 7, 8,
            0, 8, 9,
            0, 9, 10,
            0, 10, 11,
            0, 11, 12,
            0, 12, 7};
        this->mesh = new Mesh(this->name);
        if (!this->fill)
        {
            this->mesh->SetDrawMode(GL_LINE_LOOP);
        }
        this->mesh->InitFromData(vertices, indices);
    }

    Diamond::Diamond(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill) : Shape(name, center, radius, color, fill)
    {
        this->CreateMesh();
    }

    Diamond::~Diamond()
    {
    }

    void Diamond::CreateMesh()
    {
        vector<VertexFormat> vertices =
            {
                VertexFormat(this->center + glm::vec3(0, 0, 2), this->color),
                VertexFormat(this->center + glm::vec3(0, this->radius, 2), this->color),
                VertexFormat(this->center + glm::vec3(0, -this->radius, 2), this->color),
                VertexFormat(this->center + glm::vec3(-this->radius * 0.5, 0, 2), this->color),
                VertexFormat(this->center + glm::vec3(this->radius * 0.5, 10, 2), this->color),
                VertexFormat(this->center + glm::vec3(this->radius * 0.5, -10, 2), this->color),
                VertexFormat(this->center + glm::vec3(this->radius * 0.5 + 35, 10, 2), this->color),
                VertexFormat(this->center + glm::vec3(this->radius * 0.5 + 35, -10, 2), this->color)};
        vector<unsigned int> indices = {
            0, 1, 3,
            0, 3, 2,
            0, 2, 5,
            0, 1, 4,
            0, 4, 5,
            5, 6, 4,
            5, 7, 6};
        this->mesh = new Mesh(this->name);
        if (!this->fill)
        {
            this->mesh->SetDrawMode(GL_LINE_LOOP);
        }
        this->mesh->InitFromData(vertices, indices);
    }

    Star::Star(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill) : Shape(name, center, radius, color, fill)
    {
        this->CreateMesh();
    }

    Star::~Star()
    {
    }

    void Star::CreateMesh()
    {
        static constexpr float PI = 3.141592f;
        vector<VertexFormat> vertices = {
            VertexFormat(this->center, this->color),
            VertexFormat(glm::vec3((this->center.x + radius) * sin(2 * PI / 5), (this->center.y + radius) * cos(2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius) * sin(2 * 2 * PI / 5), (this->center.y + radius) * cos(2 * 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius) * sin(3 * 2 * PI / 5), (this->center.y + radius) * cos(3 * 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius) * sin(4 * 2 * PI / 5), (this->center.y + radius) * cos(4 * 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3(0.0f, (this->center.x + radius), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius * 0.5f) * sin(2 * PI / 5 + PI - 2 * PI / 5), (this->center.y + radius * 0.5f) * cos(2 * PI / 5 + PI - 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius * 0.5f) * sin(2 * 2 * PI / 5 + PI - 2 * PI / 5), (this->center.y + radius * 0.5f) * cos(2 * 2 * PI / 5 + PI - 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius * 0.5f) * sin(3 * 2 * PI / 5 + PI - 2 * PI / 5), (this->center.y + radius * 0.5f) * cos(3 * 2 * PI / 5 + PI - 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius * 0.5f) * sin(4 * 2 * PI / 5 + PI - 2 * PI / 5), (this->center.y + radius * 0.5f) * cos(4 * 2 * PI / 5 + PI - 2 * PI / 5), 2), this->color),
            VertexFormat(glm::vec3((this->center.x + radius * 0.5f) * sin(5 * 2 * PI / 5 + PI - 2 * PI / 5), (this->center.x + radius * 0.5f) * cos(5 * 2 * PI / 5 + PI - 2 * PI / 5), 2), this->color)};
        vector<unsigned int> indices = {
            5, 3, 10,
            4, 1, 6,
            2, 7, 5};
        this->mesh = new Mesh(this->name);
        if (!this->fill)
        {
            this->mesh->SetDrawMode(GL_LINE_LOOP);
        }
        this->mesh->InitFromData(vertices, indices);
    };
}