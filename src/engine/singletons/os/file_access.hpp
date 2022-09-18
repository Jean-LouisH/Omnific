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

#pragma once

#include <string>
#include <omnia_engine_api.hpp>
#include <scene/scene.hpp>
#include <asset.hpp>
#include <memory>

namespace Omnia
{
	class OMNIA_ENGINE_API FileAccess
	{
	public:
		FileAccess(std::string executableFilepath);
		void setDataDirectory(std::string assetDirectory);
		std::shared_ptr<Scene> loadScene(std::string filepath);
		std::string getExecutableFilePath();
		std::string getExecutableName();
		std::string getExecutableDirectoryPath();
		std::string getDataDirectoryPath();
		std::string getFileNameWithoutExtension(std::string filepath);
		std::string getPathBeforeFile(std::string filepath);
		bool exists(std::string filepath);

		template <class DerivedAsset>
		std::shared_ptr<DerivedAsset> loadAssetByType(std::string filepath, bool applyDataDirectory = true)
		{
			std::shared_ptr<DerivedAsset> derivedAsset;
			std::string fullFilepath;

			if (applyDataDirectory)
				fullFilepath = this->getDataDirectoryPath() + filepath;
			else
				fullFilepath = filepath;

			if (this->assets.count(filepath) != 0)
			{
				derivedAsset = std::dynamic_pointer_cast<DerivedAsset>(this->assets.at(filepath));
			}
			else
			{
				derivedAsset = std::shared_ptr<DerivedAsset>(new DerivedAsset(fullFilepath));
				std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(derivedAsset);
				if (!this->assets.count(asset->getName()))
					this->assets.emplace(asset->getName(), asset);
			}

			return derivedAsset;
		}
	private:
		std::string executableFilepath;
		std::string dataDirectory;
		std::unordered_map<std::string, std::shared_ptr<Omnia::Asset>> assets;
	};
}