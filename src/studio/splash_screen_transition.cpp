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

#include "splash_screen_transition.hpp"

void OmnificEditor::SplashScreenTransition::on_start()
{
    std::shared_ptr<Omnific::Timer> timer = Omnific::CPPEntityContext::get_component_by_type<Omnific::Timer>();
    float countdown_value = 5.0f;
    timer->start(countdown_value);
}

void OmnificEditor::SplashScreenTransition::on_logic()
{
    if (Omnific::CPPEntityContext::get_component_by_type<Omnific::Timer>()->is_finished())
    {
        std::string scene_path = "assets/scenes/debug.yml";
        Omnific::Platform::get_logger().write("Timer ran out!");
        //Omnific::SceneStorage::change_to_scene(std::shared_ptr<Omnific::Scene>(new Omnific::Scene(scene_path)));
    }
}