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

void Omnific::PlotCanvas::set_plot_points(std::vector<float> plot_points, std::shared_ptr<Omnific::Colour> plot_colour)
{
	if (!this->is_hidden() && plot_points.size() > 0)
	{
		this->dimensions = glm::vec3(Platform::get_window().get_window_size(), 0.0);
		float y_centre = this->dimensions.y / 2;
		float x_centre = this->dimensions.x / 2;
		size_t plot_points_size = plot_points.size();

		std::vector<float> positions;

		for (int i = 0; i < plot_points_size; ++i)
		{
			float plot_point = plot_points.at(i);
			this->maximum_plot_point = std::max(std::abs(plot_point), this->maximum_plot_point);
			float x = (float)i / (plot_points_size - 1) * this->dimensions.x - x_centre;
			float y = plot_point / this->maximum_plot_point * this->dimensions.y / 2.0f * this->y_scale;
			float z = 0.0;

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);
		}
		
		this->mesh = std::shared_ptr<Mesh>(new Mesh(positions, Mesh::PrimitiveMode::LINE_STRIP));
		this->material = std::shared_ptr<Material>(new Material());
		this->material->albedo_map = std::shared_ptr<Image>(new Image(plot_colour));
	}
}

void Omnific::PlotCanvas::update_image()
{

}