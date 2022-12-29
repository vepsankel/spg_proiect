#pragma once

#include "utils/Fourier.h"
#include "wave/Wave.h"
#include "surface/Surface.h"

namespace m2
{
    class Proiect : public gfxc::SimpleScene
    {
    public:
        Proiect();
        ~Proiect();

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

    private:
        Texture2D* originalImage;
        Texture2D* processedImage;

        std::shared_ptr<Wave> v1;
        std::shared_ptr<Surface> s1, s2, s3, s4, s5;
        float s_pos1, s_pos2, s_pos3, s_pos4, s_pos5;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;
    };
}   // namespace m2
