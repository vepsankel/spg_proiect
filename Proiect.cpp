#include "lab_m2/spg_proiect/Proiect.h"

using namespace std;
using namespace m2;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Proiect::Proiect()
{
    outputMode = 0;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(800, 800);

    v1 = WavePreset(0,11);
    v2 = WavePreset(1, 14);
    v3 = WavePreset(1, 14);

    std::vector<std::string> names;
    names.push_back("MainWave");
    names.push_back("NoiseLE");
    names.push_back("NoiseFW");
    
    

    for (int x = -5; x <= 5; x++) {
        for (int z = -5; z <= 5; z++) {
            surfaces[x + 5][z + 5] = std::make_unique<Surface>(names, (x + 6) * 4);
        }
    }

    shared_ptr<WaveDetails> wd1 = std::make_shared<WaveDetails>(v1->GetText(), SURFACE_DIR_UNITY, SURF_SPEED_DOWN(0.1));
    shared_ptr<WaveDetails> wd2 = std::make_shared<WaveDetails>(v2->GetText(), SURFACE_DIR_ROTATE(0), SURF_SPEED_DOWN(0.02));
    shared_ptr<WaveDetails> wd3 = std::make_shared<WaveDetails>(v2->GetText(), SURFACE_DIR_ROTATE(M_PI/2), SURF_SPEED_DOWN(0.03));
    
    std::cout << "wd2 text " << v2->GetText() << "\n";

    Surface::addWave("MainWave", wd1);
    Surface::addWave("NoiseFW", wd2);
    Surface::addWave("NoiseLE", wd3);
    
}


Proiect::~Proiect()
{
}


void Proiect::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 2, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    s_pos1 = 0;
    s_pos2 = 1;
    s_pos3 = 2;
    s_pos4 = 3;
    s_pos5 = 4;
}


void Proiect::FrameStart()
{
}


void Proiect::Update(float deltaTimeSeconds)
{
    // START

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,800,800);

    auto camera = GetSceneCamera();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 m = glm::scale(glm::mat4(1), glm::vec3(10,2,10));

    for (int x = -5; x <= 5; x++) {
        for (int z = -5; z <= 5; z++) {
            glm::mat4 p = glm::translate(m, glm::vec3(x, 0, z));
            surfaces[x+5][z+5]->FillRenderInfo(deltaTimeSeconds, p, camera->GetViewMatrix(), camera->GetProjectionMatrix());
        }
    }
    /*
    {
        glm::mat4 p1 = glm::translate(m, glm::vec3(0, 0, s_pos1));
        s1->FillRenderInfo(deltaTimeSeconds, p1, camera->GetViewMatrix(), camera->GetProjectionMatrix());

        glm::mat4 p2 = glm::translate(m, glm::vec3(0, 0, s_pos2));
        s1->FillRenderInfo(deltaTimeSeconds, p2, camera->GetViewMatrix(), camera->GetProjectionMatrix());

        glm::mat4 p3 = glm::translate(m, glm::vec3(0, 0, s_pos3));
        s1->FillRenderInfo(deltaTimeSeconds, p3, camera->GetViewMatrix(), camera->GetProjectionMatrix());

        glm::mat4 p4 = glm::translate(m, glm::vec3(0, 0, s_pos4));
        s1->FillRenderInfo(deltaTimeSeconds, p4, camera->GetViewMatrix(), camera->GetProjectionMatrix());

        glm::mat4 p5 = glm::translate(m, glm::vec3(0, 0, s_pos5));
        s1->FillRenderInfo(deltaTimeSeconds, p5, camera->GetViewMatrix(), camera->GetProjectionMatrix());

        //s_pos1 += deltaTimeSeconds; s_pos2 += deltaTimeSeconds; s_pos3 += deltaTimeSeconds; s_pos4 += deltaTimeSeconds; s_pos5 += deltaTimeSeconds;
        if (s_pos1 > 5) s_pos1 -= 5;
        if (s_pos2 > 5) s_pos2 -= 5;
        if (s_pos3 > 5) s_pos3 -= 5;
        if (s_pos4 > 5) s_pos4 -= 5;
        if (s_pos5 > 5) s_pos5 -= 5;
    }
    */
}


void Proiect::FrameEnd()
{
    DrawCoordinateSystem();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Proiect::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Proiect::OnKeyPress(int key, int mods)
{
}


void Proiect::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Proiect::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Proiect::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Proiect::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Proiect::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Proiect::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
