#pragma once

#include "components/simple_scene.h"
#include "src/Shapes.hpp"
#include <vector>

using std::vector;
using namespace Shapes;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void DrawScene();
        void DrawPlante(float deltaTimeSeconds);
        void DrawInamici(float deltaTimeSeconds);
        void DrawProiectile(float deltaTimeSeconds);
        void DrawGameOverScreen();
        void DrawStele();

    protected:
        int lives;
        int puncte;
        glm::mat3 modelMatrix;
        vector<Shape *> inamici, proiectile, stele;
        Shape *plante[3][3];
        bool mousePressed;
        int mousePosX, mousePosY;
        double time_start;
        int timp_asteptare;
        bool linie1, linie2, linie3;
    };
}