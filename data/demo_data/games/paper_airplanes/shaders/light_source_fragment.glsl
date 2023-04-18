#version 330 core

out vec4 colour;
uniform vec4 lightColour;
uniform float lightIntensity;

void main()
{
	colour = lightColour * lightIntensity;
}
