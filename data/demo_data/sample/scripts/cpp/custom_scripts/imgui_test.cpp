// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "imgui_test.hpp"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <glad/glad.h>

void Sample::IMGUITest::onStart()
{
    std::shared_ptr<Omnia::Timer> timer = Omnia::EntityContext::getComponentByType<Omnia::Timer>();
    float countdownValue = 5.0f;
    timer->start(countdownValue);
}

void Sample::IMGUITest::onLogic()
{
    std::shared_ptr<Omnia::Timer> timer = Omnia::EntityContext::getComponentByType<Omnia::Timer>();
    Omnia::Logger& logger = Omnia::OS::getLogger();

    if (timer->isFinished())
    {
        timer->start(1.0f);
        timer->stop();
        logger.write("Time's up!");
    }
}

void Sample::IMGUITest::onOutput()
{
    Omnia::Window& window = Omnia::OS::getWindow();

    if (!this->isImGuiInitialized)
    {
        if (window.getSDLGLContext() != NULL)
        {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui_ImplSDL2_InitForOpenGL(window.getSDLWindow(), window.getSDLGLContext());
            ImGui_ImplOpenGL3_Init();
            this->isImGuiInitialized = true;
        }
    }
    else
    {
        char buf[32];
        float f;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.getSDLWindow());
        ImGui::NewFrame();

 /*       ImGui::Text("Hello, world %d", 123);
        if (ImGui::Button("Save"))
            Omnia::OS::getLogger().write("Saved.");
        ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);*/

        ImGui::Begin("MyWindow");
        ImGui::Button("OK");          // Label = "OK",     ID = hash of ("MyWindow", "OK")
        ImGui::Button("Cancel");      // Label = "Cancel", ID = hash of ("MyWindow", "Cancel")
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void Sample::IMGUITest::onFinish()
{
    if (this->isImGuiInitialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}