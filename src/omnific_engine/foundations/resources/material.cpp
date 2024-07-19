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

#include "material.hpp"

void Omnific::Material::set_to_default()
{
	this->albedo = std::shared_ptr<Image>(new Image("Image::#CCCCCCCC"));
	this->metallicity = std::shared_ptr<Image>(new Image("Image::#000000FF"));
	this->specularity = std::shared_ptr<Image>(new Image("Image::#000000FF"));
	this->roughness = std::shared_ptr<Image>(new Image("Image::#FFFFFFFF"));
	this->anisotropy = std::shared_ptr<Image>(new Image("Image::#000000FF"));
	this->normal = std::shared_ptr<Image>(new Image("Image::#8080FFFF"));
	this->emission = std::shared_ptr<Image>(new Image("Image::#000000FF"));
	this->displacement = std::shared_ptr<Image>(new Image("Image::#000000FF"));
	this->occlusion = std::shared_ptr<Image>(new Image("Image::#000000FF"));
}