#version 330 core
in vec3 translation;
in vec2 uv;
in vec3 normal;
in vec3 fragmentTranslation;
out vec4 colour;
uniform vec4 lightColour;
uniform vec3 lightTranslation;
uniform vec3 cameraTranslation;
uniform float alpha;
uniform float ambientStrength;
uniform sampler2D albedoTextureSampler;

void main()
{
	float specularIntensity = 0.5;
	vec3 normalDirection = normalize(normal);
	vec3 lightDirection = normalize(lightTranslation - fragmentTranslation);
	vec3 viewDirection = normalize(cameraTranslation - fragmentTranslation);
	vec3 reflectionDirection = reflect(-lightDirection, normalDirection);
	vec4 ambient = ambientStrength * lightColour;
	vec4 diffuse = max(dot(normalDirection, lightDirection), 0.0) * lightColour;
	vec4 specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32) * specularIntensity * lightColour;
	colour = (ambient + diffuse + specular) * texture(albedoTextureSampler, uv);
	colour.a *= alpha;
}
