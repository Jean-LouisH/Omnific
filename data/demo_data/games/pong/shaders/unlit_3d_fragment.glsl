#version 330 core
in vec2 uv;
out vec4 colour;
uniform float alpha;
uniform sampler2D albedo_texture_sampler;

void main()
{    
	colour = texture(albedo_texture_sampler, uv);
	colour.a *= alpha;
}  