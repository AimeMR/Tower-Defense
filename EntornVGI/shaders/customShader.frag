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

in vec3 fragPos;
in vec3 normal;
in vec4 vertColor;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

    // Si está fuera del frustum de la luz (projCoords.z > 1.0), está iluminado
    if (projCoords.z > 1.0)
        return 0.0; // 0.0 = iluminado (no en sombra)

    float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normalize(normal), normalize(lightDirection))), 0.0005);
    
    float shadow = 0.0;
    
    // Obtener el tamaño de un texel (la unidad de muestreo)
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 

    // Bucle de muestreo (PCF 3x3)
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // Muestrear en el punto actual + offset
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            
            // Si la profundidad actual (con bias) es mayor que la almacenada, está en sombra (1.0)
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // Promediar los 9 muestreos (3x3 = 9)

	return shadow; // Un valor entre 0.0 (iluminado) y 1.0 (sombra total)
}

void main()
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightDirection);

	float diffFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diffFactor;

	vec3 ambient = lightColor * ambientIntensity;
	
	float shadow = ShadowCalculation(FragPosLightSpace);

	vec3 result = (ambient + (1.0- shadow) * diffuse) * material.diffuse.rgb;
	FragColor = vec4(result, 1.0);
}