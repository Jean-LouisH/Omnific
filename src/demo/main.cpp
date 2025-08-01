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


#include "engine.hpp"
#include "cpp_tests.hpp"
#include "xyz_scale_sine_wave.hpp"
#include "y_translation_sine_wave.hpp"
#include "z_rotation_sine_wave.hpp"

#undef main

/**Regular entry point*/

int main(int argc, char* argv[])
{
	Omnific::Engine* omnific_engine = new Omnific::Engine(argc, argv);

	omnific_engine->add_app_data_paths(
		{
			PROJECT_SOURCE_DIRECTORY, 
			PROJECT_APP_DATA_DIRECTORY,
			PROJECT_BUILD_DIRECTORY
		}
	);

	Omnific::ClassRegistry::add<OmnificDemo::CPPTests, Omnific::CPPScriptInstance>();
	Omnific::ClassRegistry::add<OmnificDemo::XYZScaleSineWave, Omnific::CPPScriptInstance>();
	Omnific::ClassRegistry::add<OmnificDemo::YTranslationSineWave, Omnific::CPPScriptInstance>();
	Omnific::ClassRegistry::add<OmnificDemo::ZRotationSineWave, Omnific::CPPScriptInstance>();

	omnific_engine->run();
	return 0;
}