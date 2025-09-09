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

Omnific::Mesh::Mesh(std::string filepath)
{
    this->type = TYPE_STRING;
	if (filepath == "Mesh::cube")
		this->set_to_cube();
    else if (filepath == "Mesh::quad")
        this->set_to_quad();
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    PrimitiveMode primitive_mode)
{
    std::vector<float> texture_coords;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->type = TYPE_STRING;
    this->primitive_mode = primitive_mode;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> texture_coords)
{
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->type = TYPE_STRING;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> texture_coords,
    std::vector<float> normals)
{
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<uint32_t> indices;

    this->type = TYPE_STRING;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> texture_coords,
    std::vector<uint32_t> indices)
{
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->type = TYPE_STRING;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> texture_coords,
    std::vector<float> normals,
    std::vector<uint32_t> indices)
{
    std::vector<float> tangents;
    std::vector<float> bitangents;

    this->type = TYPE_STRING;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

Omnific::Mesh::Mesh(std::vector<float> positions,
    std::vector<float> texture_coords,
    std::vector<float> normals,
    std::vector<float> tangents,
    std::vector<float> bitangents,
    std::vector<uint32_t> indices)
{
    this->type = TYPE_STRING;
    this->populate_data(positions,
        texture_coords,
        normals,
        tangents,
        bitangents,
        indices);
}

bool Omnific::Mesh::get_is_indexed()
{
    return this->is_indexed;
}

Omnific::Mesh::PrimitiveMode Omnific::Mesh::get_primitive_mode()
{
    return this->primitive_mode;
}

void Omnific::Mesh::set_to_cube()
{
    std::vector<float> cube_positions = {
        // Front face
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Back face
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,

        // Left face
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right face
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

        // Top face
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
    };

    std::vector<float> cube_texture_coords = {
        // Front
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    std::vector<float> cube_normals = {
        // Front
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,

        // Back
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,

        // Left
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

        // Right
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,

        // Top
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,

        // Bottom
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
    };

    std::vector<float> cube_tangents = {
        // Front
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Back
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Left
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Right
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Top
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Bottom
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    std::vector<float> cube_bitangents = {
        // Front
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Back
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Left
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Right
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Top
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Bottom
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
    };

    std::vector<uint32_t> cube_indices = {
        0,  1,  2,  0,  2,  3,  // Front
        4,  5,  6,  4,  6,  7,  // Back
        8,  9, 10,  8, 10, 11,  // Left
        12, 13, 14, 12, 14, 15,  // Right
        16, 17, 18, 16, 18, 19,  // Top
        20, 21, 22, 20, 22, 23   // Bottom
    };

    this->populate_data(cube_positions, cube_texture_coords, cube_normals, cube_tangents, cube_bitangents, cube_indices);
}

void Omnific::Mesh::set_to_quad()
{
    const std::vector<float> quad_positions =
    {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
    };

    const std::vector<float> quad_texture_coords =
    {
        1, 1,
        1, 0,
        0, 0,
        0, 1
    };

    const std::vector<uint32_t> quad_indices =
    {
        0, 1, 3,
        1, 2, 3
    };

    std::vector<float> quad_normals = {
        0.0f, 0.0f, 1.0f,  // +Z
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    std::vector<float> quad_tangents = {
        1.0f, 0.0f, 0.0f,  // +X
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    std::vector<float> quad_bitangents = {
        0.0f, 1.0f, 0.0f,  // +Y
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    this->populate_data(quad_positions, quad_texture_coords, quad_normals, quad_tangents, quad_bitangents, quad_indices);
}

void Omnific::Mesh::populate_data(
    std::vector<float> positions,
    std::vector<float> texture_coords,
    std::vector<float> normals,
    std::vector<float> tangents,
    std::vector<float> bitangents,
    std::vector<uint32_t> indices)
{
    uint8_t vec3_stride = 3;
    uint8_t vec2_stride = 2;

    if (positions.size() != 0)
    {
        if (texture_coords.size() == 0)
            for (size_t i = 0; i < positions.size(); ++i)
                texture_coords.push_back(0.0);

        if (normals.size() == 0)
            for (size_t i = 0; i < positions.size(); ++i)
                normals.push_back(0.0);

        if (tangents.size() == 0)
            for (size_t i = 0; i < positions.size(); ++i)
                tangents.push_back(0.0);

        if (bitangents.size() == 0)
            for (size_t i = 0; i < positions.size(); ++i)
                bitangents.push_back(0.0);

        if (indices.size() == 0)
            this->is_indexed = false;
        else
            this->indices = indices;

        size_t vertex_count = positions.size() / vec3_stride;
        float* positions_data = positions.data();
        float* texture_coords_data = texture_coords.data();
        float* normals_data = normals.data();

        for (size_t i = 0; i < vertex_count; ++i)
        {
            Vertex vertex;
            vertex.position = {
                positions_data[i * vec3_stride + 0],
                positions_data[i * vec3_stride + 1],
                positions_data[i * vec3_stride + 2] };
            vertex.uv = {
                texture_coords_data[i * vec2_stride + 0],
                texture_coords_data[i * vec2_stride + 1] };
            vertex.normal = {
                normals_data[i * vec3_stride + 0],
                normals_data[i * vec3_stride + 1],
                normals_data[i * vec3_stride + 2] };
            this->vertices.push_back(vertex);
        }
    }
}