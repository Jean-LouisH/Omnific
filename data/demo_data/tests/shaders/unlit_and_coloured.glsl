#version 330 core

in vec2 uv;
out vec4 colour;
uniform float red;
uniform float green;
uniform float blue;
uniform float alpha;
uniform sampler2D albedoTextureSampler;

void main()
{
	vec4 overridingColour = vec4(red, green, blue, 1.0);
	//colour = mix(overridingColour, texture(albedoTextureSampler, uv));
	colour = overridingColour * texture(albedoTextureSampler, uv);
	//colour = texture(albedoTextureSampler, uv);
	//colour = overridingColour;
	colour.a *= alpha;
}  