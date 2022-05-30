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
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->populateData(positions,
        textureCoords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<float> normals)
{
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->populateData(positions,
        textureCoords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<uint32_t> indices)
{
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->populateData(positions,
        textureCoords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<float> normals,
    std::vector<uint32_t> indices)
{
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->populateData(positions,
        textureCoords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnia::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<float> normals,
    std::vector<float> tangents,
    std::vector<float> bitangents,
    std::vector<uint32_t> indices)
{
    this->populateData(positions,
        textureCoords,
        normals,
        tangents,
        bitangents,
        indices);
}

bool Omnia::Mesh::getIsIndexed()
{
    return this->isIndexed;
}

void Omnia::Mesh::setToCube()
{
    const std::vector<float> cubePositions = 
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

    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->populateData(cubePositions, cubeTextureCoords, normals, tangents, bitangents, indices);
}

void Omnia::Mesh::setToQuad()
{
    const std::vector<float> quadPositions =
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

    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->populateData(quadPositions, quadTextureCoords, normals, tangents, bitangents, quadIndices);
}

void Omnia::Mesh::setToPlane()
{
    const std::vector<float> planePositions =
    {

    };

    const std::vector<float> planeTextureCoords =
    {

    };

    const std::vector<uint32_t> planeIndices =
    {

    };

    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->populateData(planePositions, planeTextureCoords, normals, tangents, bitangents, planeIndices);
}

void Omnia::Mesh::populateData(
    std::vector<float> positions,
    std::vector<float> textureCoords,
    std::vector<float> normals,
    std::vector<float> tangents,
    std::vector<float> bitangents,
    std::vector<uint32_t> indices)
{
    uint8_t vec3Stride = 3;
    uint8_t vec2Stride = 2;

    if (positions.size() != 0)
    {
        if (textureCoords.size() == 0)
            for (size_t i = 0; i < positions.size(); i++)
                textureCoords.push_back(0.0);

        if (normals.size() == 0)
            for (size_t i = 0; i < positions.size(); i++)
                normals.push_back(0.0);

        if (tangents.size() == 0)
            for (size_t i = 0; i < positions.size(); i++)
                tangents.push_back(0.0);

        if (bitangents.size() == 0)
            for (size_t i = 0; i < positions.size(); i++)
                bitangents.push_back(0.0);

        if (indices.size() == 0)
            this->isIndexed = false;
        else
            this->indices = indices;

        size_t vertexCount = positions.size() / vec3Stride;
        float* positionsData = positions.data();
        float* textureCoordsData = textureCoords.data();
        float* normalsData = normals.data();

        for (size_t i = 0; i < vertexCount; i++)
        {
            Vertex vertex;
            vertex.position = {
                positionsData[i * vec3Stride + 0],
                positionsData[i * vec3Stride + 1],
                positionsData[i * vec3Stride + 2] };
            vertex.uv = {
                textureCoordsData[i * vec2Stride + 0],
                textureCoordsData[i * vec2Stride + 1] };
            vertex.normal = {
                normalsData[i * vec3Stride + 0],
                normalsData[i * vec3Stride + 1],
                normalsData[i * vec3Stride + 2] };
            this->vertices.push_back(vertex);
        }
    }
}