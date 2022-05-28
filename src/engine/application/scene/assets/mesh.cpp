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

#include "mesh.hpp"
#include <stdint.h>

Omnia::Mesh::Mesh(std::string filepath)
{
	if (filepath == "Mesh::cube")
		this->setToCube();
    else if (filepath == "Mesh::quad")
        this->setToQuad();
    else if (filepath == "Mesh::plane")
        this->setToPlane();
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords)
{
    this->isIndexed = false;
    this->populateData(positions, textureCoords);
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<uint32_t> indices)
{
    this->isIndexed = true;
    this->populateData(positions, textureCoords, indices);
}

bool Omnia::Mesh::getIsIndexed()
{
    return this->isIndexed;
}

void Omnia::Mesh::setToCube()
{
    this->isIndexed = false;

    const std::vector<float> cubeVertices = 
    {
      -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5,
      -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5,

      0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5,
      0.5, -0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5,

      0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5,
      0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5,

      -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5,
      -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5,

      -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5,
      -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5,

      -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5,
      -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5
    };

    const std::vector<float> cubeTextureCoords =
    {
      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1,

      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1,

      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1,

      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1,

      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1,

      0, 0, 1, 0, 1, 1,
      0, 0, 1, 1, 0, 1
    };

    this->populateData(cubeVertices, cubeTextureCoords);
}

void Omnia::Mesh::setToQuad()
{
    const std::vector<float> quadVertices =
    {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
    };

    const std::vector<float> quadTextureCoords =
    {
        1, 1,
        1, 0,
        0, 0,
        0, 1
    };

    const std::vector<uint32_t> quadIndices =
    {
        0, 1, 3,
        1, 2, 3
    };

    this->populateData(quadVertices, quadTextureCoords, quadIndices);
}

void Omnia::Mesh::setToPlane()
{
    const std::vector<float> planeVertices =
    {

    };

    const std::vector<float> planeTextureCoords =
    {

    };

    const std::vector<uint32_t> planeIndices =
    {

    };

    this->populateData(planeVertices, planeTextureCoords, planeIndices);
}

void Omnia::Mesh::populateData(
    std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<uint32_t> indices)
{
    this->populateData(positions, textureCoords);
    this->indices = indices;
}

void Omnia::Mesh::populateData(
    std::vector<float> positions,
    std::vector<float> textureCoords)
{
    unsigned int positionsStride = 3;
    unsigned int textureCoordsStride = 2;

    if ((positions.size() / positionsStride) == (textureCoords.size() / textureCoordsStride))
    {
        size_t vertexCount = positions.size() / positionsStride;
        float* positionsData = positions.data();
        float* textureCoordsData = textureCoords.data();

        for (size_t i = 0; i < vertexCount; i++)
        {
            Vertex vertex;
            vertex.position = {
                positionsData[i * positionsStride + 0],
                positionsData[i * positionsStride + 1],
                positionsData[i * positionsStride + 2] };
            vertex.uv = {
                textureCoordsData[i * textureCoordsStride + 0],
                textureCoordsData[i * textureCoordsStride + 1] };
            this->vertices.push_back(vertex);
        }

    }
}