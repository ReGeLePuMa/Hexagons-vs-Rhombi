#include "src/Tema1.hpp"
#include "src/ShapeFactory.hpp"
#include "src/Shapes.hpp"
#include "src/Transform2D.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace m1;

static constexpr float PI = 3.141592f;

enum Culoare
{
    PORTOCALIU = 0,
    ROZ,
    GALBEN,
    ALBASTRU,
    NONE
};

static string tip_planta;

static map<std::string, glm::vec3> culori = {
    {"planta1", glm::vec3(1, 0.5f, 0)},
    {"planta2", glm::vec3(1, 0, 1)},
    {"planta3", glm::vec3(1, 1, 0)},
    {"planta4", glm::vec3(0, 0, 0.95f)}};

static map<std::string, int> punctaj = {
    {"planta1", 1},
    {"planta2", 2},
    {"planta3", 2},
    {"planta4", 3}};

static map<std::string, double> timpi_asteptare = {
    {"planta1", 3},
    {"planta2", 2},
    {"planta3", 2},
    {"planta4", 1}};

static unordered_map<Culoare, bool> linie1_culori = {
    {PORTOCALIU, false},
    {ROZ, false},
    {GALBEN, false},
    {ALBASTRU, false}};

static unordered_map<Culoare, bool> linie2_culori = {
    {PORTOCALIU, false},
    {ROZ, false},
    {GALBEN, false},
    {ALBASTRU, false}};

static unordered_map<Culoare, bool> linie3_culori = {
    {PORTOCALIU, false},
    {ROZ, false},
    {GALBEN, false},
    {ALBASTRU, false}};

static int resourceCheck = 0;
static bool collision = false;
static int time_spawn_stars = 0;
static double time_start_star = 0;

static inline double distance(glm::vec3 a, glm::vec3 b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) +
                (b.y - a.y) * (b.y - a.y));
}

static inline Culoare getCuloare(glm::vec3 vec)
{
    if (vec == glm::vec3(1, 0.5f, 0))
    {
        return PORTOCALIU;
    }
    if (vec == glm::vec3(1, 0, 1))
    {
        return ROZ;
    }
    if (vec == glm::vec3(1, 1, 0))
    {
        return GALBEN;
    }
    if (vec == glm::vec3(0, 0, 0.95f))
    {
        return ALBASTRU;
    }
    return NONE;
}

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
    for (auto &shape : proiectile)
    {
        delete shape;
    }
    for (auto &shape : inamici)
    {
        delete shape;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (plante[i][j] != nullptr)
            {
                delete plante[i][j];
            }
        }
    }
}

void Tema1::Init()
{
    srand((unsigned int)time(nullptr));
    linie1 = linie2 = linie3 = false;
    lives = 3;
    puncte = 5;
    time_start = 0;
    time_start_star = 0;
    timp_asteptare = 5;
    mousePressed = false;
    mousePosX = 0;
    mousePosY = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            plante[i][j] = nullptr;
        }
    }
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    auto center = glm::vec3(0, 0, 0);
    float radius = 100;

    Shape *rectangle = ShapeFactory::CreateShape(ShapeFactory::ShapeType::SQUARE, "rectangle", center, radius, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle->GetMesh());

    radius = 50;
    Shape *square = ShapeFactory::CreateShape(ShapeFactory::ShapeType::SQUARE, "square", center, radius, glm::vec3(0, 1, 0), true);
    AddMeshToList(square->GetMesh());

    Shape *chenar = ShapeFactory::CreateShape(ShapeFactory::ShapeType::SQUARE, "chenar", center, radius, glm::vec3(1, 1, 1));
    AddMeshToList(chenar->GetMesh());

    Shape *planta1 = ShapeFactory::CreateShape(ShapeFactory::ShapeType::DIAMOND, "planta1", center, radius, glm::vec3(1, 0.5f, 0), true);
    AddMeshToList(planta1->GetMesh());

    Shape *planta2 = ShapeFactory::CreateShape(ShapeFactory::ShapeType::DIAMOND, "planta2", center, radius, glm::vec3(1, 0, 1), true);
    AddMeshToList(planta2->GetMesh());

    Shape *planta3 = ShapeFactory::CreateShape(ShapeFactory::ShapeType::DIAMOND, "planta3", center, radius, glm::vec3(1, 1, 0), true);
    AddMeshToList(planta3->GetMesh());

    Shape *planta4 = ShapeFactory::CreateShape(ShapeFactory::ShapeType::DIAMOND, "planta4", center, radius, glm::vec3(0, 0, 0.95f), true);
    AddMeshToList(planta4->GetMesh());

    Shape *stea = ShapeFactory::CreateShape(ShapeFactory::ShapeType::STAR, "stea", center, radius, glm::vec3(1, 1, 1), true);
    AddMeshToList(stea->GetMesh());

    Shape *patrat = ShapeFactory::CreateShape(ShapeFactory::ShapeType::SQUARE, "patratel", center, 25, glm::vec3(1, 0, 0), true);
    AddMeshToList(patrat->GetMesh());
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::DrawScene()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= Transform2D::TranslateY(245);
    modelMatrix *= Transform2D::TranslateX(50);
    modelMatrix *= Transform2D::ScaleY(2.05f);
    modelMatrix *= Transform2D::ScaleX(0.33f);

    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

    float x = 150;
    float y = 400;
    for (int i = 1; i <= 9; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(x, y);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
        x += 150;
        if ((i + 1) % 3 == 1)
        {
            y -= 155;
            x = 150;
        }
    }
    x = 100;
    y = 500;
    for (int i = 1; i <= 4; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Scale(1.25f, 1.25f);
        modelMatrix *= Transform2D::Translate(x, y);
        RenderMesh2D(meshes["chenar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= Transform2D::TranslateX(-15);
        modelMatrix *= Transform2D::Scale(0.8f, 0.8f);
        RenderMesh2D(meshes["planta" + to_string(i)], shaders["VertexColor"], modelMatrix);
        if (i == 1)
        {
            modelMatrix *= Transform2D::TranslateX(-30);
            modelMatrix *= Transform2D::TranslateY(-100);
            modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }
        else if (i == 2 || i == 3)
        {
            modelMatrix *= Transform2D::TranslateX(-30);
            modelMatrix *= Transform2D::TranslateY(-100);
            modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= Transform2D::TranslateX(100);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }
        else
        {
            modelMatrix *= Transform2D::TranslateX(-30);
            modelMatrix *= Transform2D::TranslateY(-100);
            modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= Transform2D::TranslateX(100);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= Transform2D::TranslateX(100);
            RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        }
        x += 150;
    }
    x += 200;
    y = 625;
    for (int i = 1; i <= lives; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(x, y);
        modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
        RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);
        x += 150;
    }
    x = 875;
    y = 525;
    for (int i = 1; i <= puncte; i++)
    {
        if (i == 9)
        {
            x = 875;
            y = 475;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(x, y);
        modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
        RenderMesh2D(meshes["stea"], shaders["VertexColor"], modelMatrix);
        x += 50;
    }
}

void Tema1::DrawPlante(float deltaTimeSeconds)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (plante[i][j] != nullptr)
            {
                if (plante[i][j]->HasCollided())
                {
                    RenderMesh2D(plante[i][j]->GetMesh(), shaders["VertexColor"], glm::mat3(1) * Transform2D::Scale(plante[i][j]->GetScaleFactor(), plante[i][j]->GetScaleFactor()));
                    plante[i][j]->SetScaleFactor(plante[i][j]->GetScaleFactor() - 0.5f * deltaTimeSeconds);
                    if (plante[i][j]->GetScaleFactor() <= 0)
                    {
                        Shape *aux = plante[i][j];
                        plante[i][j] = nullptr;
                        delete aux;
                    }
                }
                else
                {
                    RenderMesh2D(plante[i][j]->GetMesh(), shaders["VertexColor"], glm::mat3(1));
                    if (i == 0 && linie1 && linie1_culori[getCuloare(plante[i][j]->GetColor())] && Engine::GetElapsedTime() - plante[i][j]->GetElapsedTime() >= timpi_asteptare[plante[i][j]->GetMeshName()])
                    {
                        plante[i][j]->SetElapsedTime(Engine::GetElapsedTime());
                        Shape *proiectil = ShapeFactory::CreateShape(ShapeFactory::ShapeType::STAR, "stea", glm::vec3(0, 0, 2), 25, culori[plante[i][j]->GetMeshName()], true);
                        proiectil->TranslateCenter(plante[i][j]->GetCenter().x + 50, plante[i][j]->GetCenter().y);
                        proiectile.push_back(proiectil);
                    }
                    else if (i == 1 && linie2 && linie2_culori[getCuloare(plante[i][j]->GetColor())] && Engine::GetElapsedTime() - plante[i][j]->GetElapsedTime() >= timpi_asteptare[plante[i][j]->GetMeshName()])
                    {
                        plante[i][j]->SetElapsedTime(Engine::GetElapsedTime());
                        Shape *proiectil = ShapeFactory::CreateShape(ShapeFactory::ShapeType::STAR, "stea", glm::vec3(0, 0, 2), 25, culori[plante[i][j]->GetMeshName()], true);
                        proiectil->TranslateCenter(plante[i][j]->GetCenter().x + 50, plante[i][j]->GetCenter().y);
                        proiectile.push_back(proiectil);
                    }
                    else if (i == 2 && linie3 && linie3_culori[getCuloare(plante[i][j]->GetColor())] && Engine::GetElapsedTime() - plante[i][j]->GetElapsedTime() >= timpi_asteptare[plante[i][j]->GetMeshName()])
                    {
                        plante[i][j]->SetElapsedTime(Engine::GetElapsedTime());
                        Shape *proiectil = ShapeFactory::CreateShape(ShapeFactory::ShapeType::STAR, "stea", glm::vec3(0, 0, 2), 25, culori[plante[i][j]->GetMeshName()], true);
                        proiectil->TranslateCenter(plante[i][j]->GetCenter().x + 50, plante[i][j]->GetCenter().y);
                        proiectile.push_back(proiectil);
                    }
                }
            }
        }
    }
}

void Tema1::DrawInamici(float deltaTimeSeconds)
{
    bool ok1 = false, ok2 = false, ok3 = false;
    bool cok1[4], cok2[4], cok3[4];
    for (int i = 0; i < 4; i++)
    {
        cok1[i] = cok2[i] = cok3[i] = false;
    }
    for (int i = 0; i < inamici.size(); i++)
    {
        if (inamici[i]->HasCollided())
        {
            RenderMesh2D(inamici[i]->GetMesh(), shaders["VertexColor"], glm::mat3(1) * Transform2D::Translate(inamici[i]->GetCenter().x, inamici[i]->GetCenter().y) * Transform2D::Scale(inamici[i]->GetScaleFactor(), inamici[i]->GetScaleFactor()));
            inamici[i]->SetScaleFactor(inamici[i]->GetScaleFactor() - 0.5f * deltaTimeSeconds);
            if (inamici[i]->GetScaleFactor() <= 0)
            {
                Shape *aux = inamici[i];
                inamici.erase(inamici.begin() + i);
                delete aux;
            }
        }
        else
        {
            RenderMesh2D(inamici[i]->GetMesh(), shaders["VertexColor"], glm::mat3(1) * Transform2D::Translate(inamici[i]->GetCenter().x, inamici[i]->GetCenter().y));
            inamici[i]->TranslateCenter(inamici[i]->GetCenter().x - 2.5f, inamici[i]->GetCenter().y);
            if (inamici[i]->GetCenter().y == window->GetResolution().y - 320)
            {
                ok1 = true;
                switch (getCuloare(inamici[i]->GetColor()))
                {
                case PORTOCALIU:
                    cok1[0] = true;
                    break;
                case ROZ:
                    cok1[1] = true;
                    break;
                case GALBEN:
                    cok1[2] = true;
                    break;
                case ALBASTRU:
                    cok1[3] = true;
                    break;
                }
            }
            else if (inamici[i]->GetCenter().y == window->GetResolution().y - 475)
            {
                ok2 = true;
                switch (getCuloare(inamici[i]->GetColor()))
                {
                case PORTOCALIU:
                    cok2[0] = true;
                    break;
                case ROZ:
                    cok2[1] = true;
                    break;
                case GALBEN:
                    cok2[2] = true;
                    break;
                case ALBASTRU:
                    cok2[3] = true;
                    break;
                }
            }
            else if (inamici[i]->GetCenter().y == window->GetResolution().y - 630)
            {
                ok3 = true;
                switch (getCuloare(inamici[i]->GetColor()))
                {
                case PORTOCALIU:
                    cok3[0] = true;
                    break;
                case ROZ:
                    cok3[1] = true;
                    break;
                case GALBEN:
                    cok3[2] = true;
                    break;
                case ALBASTRU:
                    cok3[3] = true;
                    break;
                }
            }
        }
    }
    linie1 = ok1;
    linie2 = ok2;
    linie3 = ok3;
    for (int i = 0; i < 4; i++)
    {
        linie1_culori[(Culoare)i] = cok1[i];
        linie2_culori[(Culoare)i] = cok2[i];
        linie3_culori[(Culoare)i] = cok3[i];
    }
}

void Tema1::DrawProiectile(float deltaTimeSeconds)
{
    for (int i = 0; i < proiectile.size(); i++)
    {
        if (proiectile[i]->HasCollided())
        {
            RenderMesh2D(proiectile[i]->GetMesh(), shaders["VertexColor"], glm::mat3(1) * Transform2D::Translate(proiectile[i]->GetCenter().x, proiectile[i]->GetCenter().y) * Transform2D::Rotate(proiectile[i]->GetAngle()) * Transform2D::Scale(proiectile[i]->GetScaleFactor(), proiectile[i]->GetScaleFactor()));
            proiectile[i]->SetScaleFactor(proiectile[i]->GetScaleFactor() - 0.5f * deltaTimeSeconds);
            if (proiectile[i]->GetScaleFactor() <= 0)
            {
                Shape *aux = proiectile[i];
                proiectile.erase(proiectile.begin() + i);
                delete aux;
            }
        }
        else
        {
            RenderMesh2D(proiectile[i]->GetMesh(), shaders["VertexColor"],
                         glm::mat3(1) * Transform2D::Translate(proiectile[i]->GetCenter().x, proiectile[i]->GetCenter().y) * Transform2D::Rotate(proiectile[i]->GetAngle()));
            proiectile[i]->TranslateCenter(proiectile[i]->GetCenter().x + 3.5f, proiectile[i]->GetCenter().y);
            proiectile[i]->SetAngle(proiectile[i]->GetAngle() - 0.1f);
        }
    }
}

void Tema1::DrawStele()
{
    for (int i = 0; i < stele.size(); i++)
    {
        RenderMesh2D(stele[i]->GetMesh(), shaders["VertexColor"], glm::mat3(1) * Transform2D::Translate(stele[i]->GetCenter().x, stele[i]->GetCenter().y));
    }
}

void Tema1::DrawGameOverScreen()
{
    // G
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(150, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(100, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(100, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(100, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(150, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 550));

    // A
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(450, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(425, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(400, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(375, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(350, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(475, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(500, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(525, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(425, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(475, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(550, 450));

    // M
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(700, 625));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(750, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(800, 625));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 650));

    // E
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 600));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 500));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1000, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 650));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1000, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 550));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1000, 450));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 450));

    // O
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(200, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(300, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(350, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(400, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(400, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(400, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(400, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(250, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(300, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(350, 300));

    // V
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(500, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(525, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(550, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(575, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(600, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(625, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(650, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(675, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(700, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(725, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(750, 300));

    // E
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(850, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(900, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(900, 175));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 175));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(900, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(950, 50));

    // R
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1050, 50));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1100, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1150, 300));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1200, 250));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1200, 200));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1100, 150));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1150, 100));
    RenderMesh2D(meshes["patratel"], shaders["VertexColor"], Transform2D::Translate(1200, 50));
}

void Tema1::Update(float deltaTimeSeconds)
{
    glClearColor(0.10f, 0.10f, 0.10f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (lives > -1)
    {
        double time_now = Engine::GetElapsedTime();
        if (time_now - time_start_star >= time_spawn_stars && puncte < 16)
        {
            time_start_star = time_now;
            time_spawn_stars = 6 + (rand() % (9 - 6) + 1);
            int x = rand() % 4 + 1;
            for (int i = 0; i < x; i++)
            {
                Shape *stea = ShapeFactory::CreateShape(ShapeFactory::ShapeType::STAR, "stea", glm::vec3(0, 0, 3), 25, glm::vec3(1, 1, 1), true);
                stea->TranslateCenter((float)(600 + (rand() % (window->GetResolution().x - 600))), (float)(rand() % 450));
                stele.push_back(stea);
            }
        }
        if (time_now - time_start >= timp_asteptare)
        {
            time_start = time_now;
            timp_asteptare = 3 + rand() % (6 - 3) + 1;
            int line = rand() % 3 + 1;
            glm::vec3 culoare = culori["planta" + to_string(rand() % 4 + 1)];
            int x = window->GetResolution().x;
            int y = 400;
            switch (line)
            {
            case 1:
                y = window->GetResolution().y - 320;
                linie1 = true;
                linie1_culori[getCuloare(culoare)] = true;
                break;
            case 2:
                y = window->GetResolution().y - 475;
                linie2 = true;
                linie2_culori[getCuloare(culoare)] = true;
                break;
            case 3:
                y = window->GetResolution().y - 630;
                linie3 = true;
                linie3_culori[getCuloare(culoare)] = true;
                break;
            }
            Shape *inamic = ShapeFactory::CreateShape(ShapeFactory::ShapeType::HEXAGON, "inamic", glm::vec3(0, 0, 0), 50, culoare, true);
            inamic->TranslateCenter((float)x, (float)y);
            inamici.push_back(inamic);
        }

        DrawScene();
        if (mousePressed)
        {
            RenderMesh2D(meshes[tip_planta], shaders["VertexColor"], glm::mat3(1) * Transform2D::Translate(1.0f * mousePosX, 1.0f * (window->GetResolution().y - mousePosY)));
        }
        DrawPlante(deltaTimeSeconds);
        DrawInamici(deltaTimeSeconds);
        DrawProiectile(deltaTimeSeconds);
        DrawStele();
        collision = false;
        for (int i = 0; i < inamici.size(); i++)
        {
            if (inamici[i]->GetCenter().x <= 0)
            {
                Shape *aux = inamici[i];
                inamici.erase(inamici.begin() + i);
                lives--;
                delete aux;
            }
        }
        for (int i = 0; i < proiectile.size(); i++)
        {
            if (proiectile[i]->GetCenter().x >= window->GetResolution().x)
            {
                Shape *aux = proiectile[i];
                proiectile.erase(proiectile.begin() + i);
                delete aux;
            }
        }

        for (int i = 0; i < inamici.size(); i++)
        {
            for (int j = 0; j < proiectile.size(); j++)
            {
                if (distance(inamici[i]->GetCenter(), proiectile[j]->GetCenter()) <= inamici[i]->GetRadius() + proiectile[j]->GetRadius() && !inamici[i]->HasCollided() && !proiectile[j]->HasCollided() && inamici[i]->GetColor() == proiectile[j]->GetColor())
                {
                    inamici[i]->Collided(true);
                    proiectile[j]->Collided(true);
                    resourceCheck++;
                    if (resourceCheck == 5)
                    {
                        resourceCheck = 0;
                        if (puncte < 16)
                        {
                            puncte++;
                        }
                    }
                    collision = true;
                    break;
                }
            }
            if (collision)
            {
                collision = true;
                continue;
            }
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    if (plante[j][k] != nullptr &&
                        distance(inamici[i]->GetCenter(), plante[j][k]->GetCenter()) <= plante[j][k]->GetRadius() + inamici[i]->GetRadius() && !plante[j][k]->HasCollided())
                    {
                        plante[j][k]->Collided(true);
                        continue;
                    }
                }
            }
        }
    }
    else
    {
        DrawGameOverScreen();
    }
}

void Tema1::FrameEnd()
{
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (lives == -1 && (key == GLFW_KEY_R))
    {
        linie1 = linie2 = linie3 = false;
        lives = 3;
        puncte = 5;
        time_start = Engine::GetElapsedTime();
        time_start_star = Engine::GetElapsedTime();
        timp_asteptare = 3;
        time_spawn_stars = 0;
        mousePressed = false;
        mousePosX = 0;
        mousePosY = 0;
        linie1_culori = {
            {PORTOCALIU, false},
            {ROZ, false},
            {GALBEN, false},
            {ALBASTRU, false}};

        linie2_culori = {
            {PORTOCALIU, false},
            {ROZ, false},
            {GALBEN, false},
            {ALBASTRU, false}};

        linie3_culori = {
            {PORTOCALIU, false},
            {ROZ, false},
            {GALBEN, false},
            {ALBASTRU, false}};
        for (int i = 0; i < inamici.size(); i++)
        {
            Shape *aux = inamici[i];
            inamici.erase(inamici.begin() + i);
            delete aux;
        }
        for (int i = 0; i < proiectile.size(); i++)
        {
            Shape *aux = proiectile[i];
            proiectile.erase(proiectile.begin() + i);
            delete aux;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Shape *aux = plante[i][j];
                plante[i][j] = nullptr;
                delete aux;
            }
        }
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mousePosX = mouseX;
    mousePosY = mouseY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        mousePressed = true;
        mousePosX = mouseX;
        mousePosY = mouseY;
        if (mousePosX >= 80 && mousePosX <= 165 && mousePosY >= 45 && mousePosY <= 145)
        {
            tip_planta = "planta1";
        }
        else if (mousePosX >= 270 && mousePosX <= 355 && mousePosY >= 45 && mousePosY <= 145)
        {
            tip_planta = "planta2";
        }
        else if (mousePosX >= 455 && mousePosX <= 545 && mousePosY >= 45 && mousePosY <= 145)
        {
            tip_planta = "planta3";
        }
        else if (mousePosX >= 645 && mousePosX <= 730 && mousePosY >= 45 && mousePosY <= 145)
        {
            tip_planta = "planta4";
        }
        else
        {
            mousePressed = false;
            mousePosX = mousePosY = 0;
            return;
        }
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        mousePressed = false;
        mousePosX = 0;
        mousePosY = 0;

        if ((mouseX >= 100 && mouseX <= 200) || (mouseX >= 250 && mouseX <= 350) || (mouseX >= 400 && mouseX <= 500) ||
            (mouseY >= 270 && mouseY <= 370) || (mouseY >= 425 && mouseY <= 525) || (mouseY >= 580 && mouseY <= 680))
        {
            int x = 0, y = 0;
            int index1 = -1, index2 = -1;
            if (mouseX >= 100 && mouseX <= 200)
            {
                x = 150;
                index2 = 0;
            }
            else if (mouseX >= 250 && mouseX <= 350)
            {
                x = 300;
                index2 = 1;
            }
            else if (mouseX >= 400 && mouseX <= 500)
            {
                x = 450;
                index2 = 2;
            }
            if (mouseY >= 270 && mouseY <= 370)
            {
                y = window->GetResolution().y - 320;
                index1 = 0;
            }
            else if (mouseY >= 425 && mouseY <= 525)
            {
                y = window->GetResolution().y - 475;
                index1 = 1;
            }
            else if (mouseY >= 580 && mouseY <= 680)
            {
                y = window->GetResolution().y - 630;
                index1 = 2;
            }
            if (x != 0 && y != 0)
            {
                Shape *planta = ShapeFactory::CreateShape(ShapeFactory::ShapeType::DIAMOND, tip_planta, glm::vec3(x - 15, y, 2), 50, culori[tip_planta], true);
                if (index1 != -1 && index2 != -1 && plante[index1][index2] == nullptr && puncte >= punctaj[tip_planta])
                {
                    puncte -= punctaj[tip_planta];
                    plante[index1][index2] = planta;
                }
                else
                {
                    delete planta;
                }
            }
        }
        for (int i = 0; i < stele.size(); i++)
        {
            if (distance(stele[i]->GetCenter(), glm::vec3(mouseX, window->GetResolution().y - mouseY, 0)) <= stele[i]->GetRadius())
            {
                Shape *aux = stele[i];

                stele.erase(stele.begin() + i);
                delete aux;
                puncte = min(puncte + 1, 16);
                break;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_3)
    {
        if ((mouseX >= 100 && mouseX <= 200) || (mouseX >= 250 && mouseX <= 350) || (mouseX >= 400 && mouseX <= 500) ||
            (mouseY >= 270 && mouseY <= 370) || (mouseY >= 425 && mouseY <= 525) || (mouseY >= 580 && mouseY <= 680))
        {
            int index1 = -1, index2 = -1;
            if (mouseX >= 100 && mouseX <= 200)
            {
                index2 = 0;
            }
            else if (mouseX >= 250 && mouseX <= 350)
            {
                index2 = 1;
            }
            else if (mouseX >= 400 && mouseX <= 500)
            {
                index2 = 2;
            }
            if (mouseY >= 270 && mouseY <= 370)
            {
                index1 = 0;
            }
            else if (mouseY >= 425 && mouseY <= 525)
            {
                index1 = 1;
            }
            else if (mouseY >= 580 && mouseY <= 680)
            {
                index1 = 2;
            }
            if (index1 != -1 && index2 != -1 && plante[index1][index2] != nullptr)
            {
                Shape *aux = plante[index1][index2];
                plante[index1][index2] = nullptr;
                puncte = min(puncte + punctaj[aux->GetMeshName()], 16);
                delete aux;
            }
        }
    }
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
