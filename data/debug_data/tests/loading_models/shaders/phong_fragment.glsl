#version 330 core
in vec2 uv;
out vec4 colour;
uniform vec4 lightColour;
uniform float alpha;
uniform float ambientStrength;
uniform sampler2D albedoTextureSampler;

void main()
{
	//float ambientStrength = 0.5;
	colour = texture(albedoTextureSampler, uv) * ambientStrength * lightColour;
	colour.a *= alpha;
}
