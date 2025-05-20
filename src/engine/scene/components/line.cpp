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

#include "line.hpp"

void Omnific::Line::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "")
		{

		}
	}
}

void Omnific::Line::generate_from_points(glm::vec3 point_a, glm::vec3 point_b, std::shared_ptr<Colour> colour)
{
	std::vector<float> positions;
	positions.push_back(point_a.x);
	positions.push_back(point_a.y);
	positions.push_back(point_a.z);
	positions.push_back(point_b.x);
	positions.push_back(point_b.y);
	positions.push_back(point_b.z);
	this->mesh = std::shared_ptr<Mesh>(new Mesh(positions, Mesh::PrimitiveMode::LINE_STRIP));
	this->material = std::shared_ptr<Material>(new Material());
	this->highlight_colour = colour;
}

void Omnific::Line::generate_from_point_list(std::vector<glm::vec3> points, std::shared_ptr<Colour> colour)
{
	std::vector<float> positions;
	for (int i = 0; i < points.size(); ++i)
	{
		glm::vec3 point = points.at(i);
		positions.push_back(point.x);
		positions.push_back(point.y);
		positions.push_back(point.z);
	}
	this->mesh = std::shared_ptr<Mesh>(new Mesh(positions, Mesh::PrimitiveMode::LINE_STRIP));
	this->material = std::shared_ptr<Material>(new Material());
	this->highlight_colour = colour;
}