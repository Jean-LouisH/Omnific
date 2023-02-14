#version 330 core
in vec2 uv;
out vec4 colour;
uniform float alpha;
uniform sampler2D albedoTextureSampler;

void main()
{    
	colour = texture(albedoTextureSampler, uv);
	colour.a *= alpha;
}  