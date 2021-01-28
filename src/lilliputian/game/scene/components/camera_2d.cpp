#include "camera_2d.hpp"

void Lilliputian::Camera2D::setViewportWidth(uint32_t width_px)
{
	this->viewport_px.width = width_px;
}

void Lilliputian::Camera2D::setViewportHeight(uint32_t height_px)
{
	this->viewport_px.height = height_px;
}