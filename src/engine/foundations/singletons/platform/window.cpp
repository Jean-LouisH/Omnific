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

#include "window.hpp"
#include <stdint.h>
#include "foundations/singletons/platform/platform.hpp"

void Omnia::Window::initialize(std::string title, uint16_t width, uint16_t height, bool is_fullscreen, std::string rendering_context)
{
	uint64_t rendering_context_flag = 0x0;

	if (rendering_context == "opengl")
		rendering_context_flag = SDL_WINDOW_OPENGL;

	this->sdl_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN_DESKTOP & is_fullscreen | rendering_context_flag), SDL_DestroyWindow);

	SDL_DisableScreenSaver();
	SDL_GetCurrentDisplayMode(0, this->sdl_display_mode.get());
	this->is_fullscreen = is_fullscreen;
}

void Omnia::Window::initialize_window_context(std::string rendering_context)
{
	if (rendering_context == "opengl")
		this->sdl_gl_context = SDL_GL_CreateContext(this->sdl_window.get());
}

void Omnia::Window::set_to_windowed(uint16_t width_px, uint16_t height_px)
{
	if (this->sdl_window != nullptr)
	{
		SDL_SetWindowFullscreen(this->sdl_window.get(), 0);
		SDL_SetWindowSize(this->sdl_window.get(), width_px, height_px);
		Platform::get_logger().write("Set Window to windowed.");
	}
}

void Omnia::Window::set_to_fullscreen()
{
	if (this->sdl_window != nullptr)
	{
		SDL_SetWindowDisplayMode(this->sdl_window.get(), this->sdl_display_mode.get());
		SDL_SetWindowFullscreen(this->sdl_window.get(), SDL_WINDOW_FULLSCREEN);
		Platform::get_logger().write("Set Window to fullscreen.");
	}
}

void Omnia::Window::toggle_windowed_fullscreen()
{
	if (this->sdl_window != nullptr)
	{
		this->is_fullscreen = !is_fullscreen;

		if (this->is_fullscreen)
		{
			SDL_SetWindowFullscreen(this->sdl_window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
			Platform::get_logger().write("Set Window to windowed fullscreen.");
		}
		else
		{
			SDL_SetWindowFullscreen(this->sdl_window.get(), 0);
			SDL_SetWindowPosition(
				this->sdl_window.get(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED);
			Platform::get_logger().write("Set Window to windowed.");
		}

		SDL_ShowCursor(!this->is_fullscreen);
	}
}

void Omnia::Window::resize(uint16_t width_px, uint16_t height_px)
{
	if (this->sdl_window != nullptr)
	{
		SDL_SetWindowSize(this->sdl_window.get(), width_px, height_px);
		SDL_SetWindowPosition(this->sdl_window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		Platform::get_logger().write("Resized Window to: " + std::to_string(width_px) + "x" + std::to_string(height_px));
	}
}

void Omnia::Window::change_title(const char* title)
{
	if (this->sdl_window != nullptr)
	{
		SDL_SetWindowTitle(this->sdl_window.get(), title);
		Platform::get_logger().write("Changed Window title to: \"" + (std::string)title + "\"");
	}
}

void Omnia::Window::change_icon(void* data, uint32_t width, uint32_t height, uint32_t depth, uint32_t pitch)
{
	if (this->sdl_window != nullptr)
	{
		SDL_Surface* sdl_surface = SDL_CreateRGBSurfaceFrom(
			data,
			width,
			height,
			depth,
			pitch,
			0,
			0,
			0,
			0);

		if (sdl_surface != nullptr)
		{
			SDL_SetWindowIcon(this->sdl_window.get(), sdl_surface);
			SDL_FreeSurface(sdl_surface);
			Platform::get_logger().write("Changed Window icon.");
		}
	}
}

void Omnia::Window::maximize()
{
	if (this->sdl_window != nullptr)
	{
		SDL_MaximizeWindow(this->sdl_window.get());
		Platform::get_logger().write("Maximized Window.");
	}
}

void Omnia::Window::minimize()
{
	if (this->sdl_window != nullptr)
	{
		SDL_MinimizeWindow(this->sdl_window.get());
		Platform::get_logger().write("Minimized Window.");
	}
}

void Omnia::Window::raise()
{
	if (this->sdl_window != nullptr)
	{
		SDL_RaiseWindow(this->sdl_window.get());
		Platform::get_logger().write("Raised Window.");
	}
}

void Omnia::Window::restore()
{
	if (this->sdl_window != nullptr)
	{
		SDL_RestoreWindow(this->sdl_window.get());
		Platform::get_logger().write("Restored Window.");
	}
}

void Omnia::Window::hide()
{
	if (this->sdl_window != nullptr)
	{
		SDL_HideWindow(this->sdl_window.get());
		Platform::get_logger().write("Hid Window.");
	}
}

void Omnia::Window::show()
{
	if (this->sdl_window != nullptr)
	{
		SDL_ShowWindow(this->sdl_window.get());
		Platform::get_logger().write("Showed Window.");
	}
}

glm::vec2 Omnia::Window::get_window_size()
{
	glm::vec2 window_size;

	if (this->sdl_window != nullptr)
	{
		int width = 0;
		int height = 0;

		SDL_GetWindowSize(this->sdl_window.get(), &width, &height);
		window_size.x = width;
		window_size.y = height;
	}

	return window_size;
}

glm::vec2 Omnia::Window::get_window_position()
{
	glm::vec2 window_position;

	if (this->sdl_window != nullptr)
	{
		int x = 0;
		int y = 0;

		SDL_GetWindowPosition(this->sdl_window.get(), &x, &y);
		window_position.x = x;
		window_position.y = y;
	}
	
	return window_position;
}

SDL_Window* Omnia::Window::get_sdlwindow()
{
	return this->sdl_window.get();
}

SDL_GLContext Omnia::Window::get_sdlglcontext()
{
	return this->sdl_gl_context;
}