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

#include "camera_2d.hpp"

Lilliputian::Camera2D::Camera2D()
{
	this->setViewportWidth(300);
}

void Lilliputian::Camera2D::resetAspect()
{
	this->aspect = defaultAspect;
}

Lilliputian::Rectangle Lilliputian::Camera2D::getViewportDimensions()
{
	return this->viewport_px;
}

bool Lilliputian::Camera2D::getIsStreaming()
{
	return this->isStreaming;
}

void Lilliputian::Camera2D::setViewportWidth(uint32_t width_px)
{
	this->viewport_px.width = width_px;
	if (keepAspect)
		this->viewport_px.height = (uint32_t)((double)width_px / aspect);
}

void Lilliputian::Camera2D::setViewportHeight(uint32_t height_px)
{
	this->viewport_px.height = height_px;
	if (keepAspect)
		this->viewport_px.width = (uint32_t)((double)height_px * aspect);
}

void Lilliputian::Camera2D::setLimits(float left, float bottom, float top, float right)
{
	this->limits_px.min_px.x = left;
	this->limits_px.min_px.y = bottom;
	this->limits_px.max_px.x = right;
	this->limits_px.max_px.y = top;
}

void Lilliputian::Camera2D::setKeepAspect(bool value)
{
	this->keepAspect = value;
}

void Lilliputian::Camera2D::setIsStreaming(bool value)
{
	this->isStreaming = value;
}