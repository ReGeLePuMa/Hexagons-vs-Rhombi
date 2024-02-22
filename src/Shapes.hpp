#pragma once

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include <cmath>

using std::string;
using std::vector;

namespace Shapes
{
    class Shape
    {
    public:
        Shape(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
        virtual ~Shape();

        virtual void CreateMesh() = 0;
        string GetMeshName();
        glm::vec3 GetCenter();
        glm::vec3 GetColor();
        Mesh *GetMesh();
        float GetRadius();
        bool GetFill();
        float GetAngle();
        double GetElapsedTime();
        bool HasCollided();
        float GetScaleFactor();
        void SetScaleFactor(float scaleFactor);
        void Collided(bool hasCollided);
        void SetElapsedTime(double elapsedTime);
        void SetAngle(float angle);
        void TranslateCenter(float x, float y);
        void ScaleRadius(float scale);

    protected:
        string name;
        glm::vec3 center;
        glm::vec3 color;
        float radius;
        float angle;
        double elapsedTime;
        Mesh *mesh;
        bool fill;
        bool hasCollided;
        float scaleFactor = 1;
    };

    class Square : public Shape
    {
    public:
        Square(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
        ~Square();
        void CreateMesh() override;
    };

    class Hexagon : public Shape
    {
    public:
        Hexagon(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
        ~Hexagon();
        void CreateMesh() override;
    };

    class Diamond : public Shape
    {
    public:
        Diamond(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
        ~Diamond();
        void CreateMesh() override;
    };

    class Star : public Shape
    {
    public:
        Star(const string &name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false);
        ~Star();
        void CreateMesh() override;
    };
}