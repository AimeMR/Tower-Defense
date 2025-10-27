#version 460 core

struct Material
{	vec4 emission;		// Coeficient d'emissió (r,g,b,a) del material.
	vec4 ambient;		// Coeficient de reflectivitat ambient (r,g,b,a) del material.
        vec4 diffuse;		// Coeficient de reflectivitat difusa (r,g,b,a) del material.
	vec4 specular;		// Coeficient de reflectivitat especular (r,g,b,a) del material.
	float shininess;	// Exponent per al coeficient de reflectivitat especular del material (1,500).
};

uniform Material material;

uniform vec3 lightColor;   // color de la luz
uniform vec3 objectColor;  // color del objeto

in vec4 vertColor;

out vec4 FragColor;

void main()
{
    vec3 result = material.diffuse.rgb;
    FragColor = vec4(result, 1.0);
}