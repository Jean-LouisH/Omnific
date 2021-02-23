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