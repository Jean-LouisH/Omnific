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

#include "plot_canvas.hpp"
#include <foundations/singletons/platform/platform.hpp>

void Omnific::PlotCanvas::deserialize(YAML::Node yaml_node)
{
	GUIElement::deserialize(yaml_node);

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "")
		{

		}
	}
}

void Omnific::PlotCanvas::set_plot_points(std::vector<float> plot_points, std::shared_ptr<Omnific::Colour> background_colour, std::shared_ptr<Omnific::Colour> plot_colour)
{
	this->plot_points = plot_points;
	this->target_plot_colour = plot_colour;
	this->target_plot_background_colour = background_colour;
	this->dimensions = glm::vec3(Platform::get_window().get_window_size(), 0.0);
	this->update_image();
}

void Omnific::PlotCanvas::update_image()
{
	if (!this->is_hidden() && this->plot_points.size() > 0)
	{
		this->set_to_image(std::shared_ptr<Image>(new Image(this->plot_points, this->dimensions.x, this->dimensions.y, this->target_plot_background_colour, this->target_plot_colour)));
	}
}