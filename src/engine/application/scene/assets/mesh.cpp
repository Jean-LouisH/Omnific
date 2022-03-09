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

void Omnia::Mesh::setToCube()
{
    const std::vector<float> cubeVertices =
    {
        -0.5, -0.5,  0.5, //0
         0.5, -0.5,  0.5, //1
        -0.5,  0.5,  0.5, //2
         0.5,  0.5,  0.5, //3
        -0.5, -0.5, -0.5, //4
         0.5, -0.5, -0.5, //5
        -0.5,  0.5, -0.5, //6
         0.5,  0.5, -0.5  //7
    };

    const std::vector<uint32_t> cubeTextureCoords =
    {
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0
    };

    const std::vector<uint32_t> cubeIndices =
    {
        //Top
        2, 6, 7,
        2, 3, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Back
        4, 6, 7,
        4, 5, 7,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,
    };

    this->populateData(cubeVertices, cubeTextureCoords, cubeIndices);
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

    const std::vector<uint32_t> quadTextureCoords =
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

    const std::vector<uint32_t> planeTextureCoords =
    {

    };

    const std::vector<uint32_t> planeIndices =
    {

    };

    this->populateData(planeVertices, planeTextureCoords, planeIndices);
}

void Omnia::Mesh::populateData(
    std::vector<float> positions,
    std::vector<uint32_t> textureCoords,
    std::vector<uint32_t> indices)
{
    if ((positions.size() / 3) == (textureCoords.size() / 2))
    {
        size_t vertexCount = positions.size() / 3;
        float* positionsData = positions.data();
        uint32_t* textureCoordsData = textureCoords.data();

        for (size_t i = 0; i < vertexCount; i++)
        {
            Vertex vertex;
            vertex.position = { positionsData[i + 0], positionsData[i + 1], positionsData[i + 2] };
            vertex.uv = { textureCoordsData[i + 0], textureCoordsData[i + 1] };
            this->vertices.push_back(vertex);
        }

        this->indices = indices;
    }
}