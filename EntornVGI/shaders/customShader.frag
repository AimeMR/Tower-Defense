#version 460 core

struct Material
{	vec4 emission;		// Coeficient d'emissió (r,g,b,a) del material.
	vec4 ambient;		// Coeficient de reflectivitat ambient (r,g,b,a) del material.
        vec4 diffuse;		// Coeficient de reflectivitat difusa (r,g,b,a) del material.
	vec4 specular;		// Coeficient de reflectivitat especular (r,g,b,a) del material.
	float shininess;	// Exponent per al coeficient de reflectivitat especular del material (1,500).
};

uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform float ambientIntensity;

in vec4 vertColor;

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightDirection);

	float diffFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diffFactor;

	vec3 ambient = lightColor * ambientIntensity;
	

	vec3 result = (ambient + diffuse) * material.diffuse.rgb;
	FragColor = vec4(result, 1.0);
}