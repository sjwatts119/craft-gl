#version 330 core

struct LightStruct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lNormal;
layout (location = 2) in vec2 lTexCoords;
layout (location = 3) in int lTexIndex;
layout (location = 4) in int lIsHighlighted;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform LightStruct uSun;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoords;

flat out int TexIndex;
flat out int IsHighlighted;

out LightStruct Sun;

void main() {
    // Transform frag position into camera space
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(lPosition, 1.0f);

    // Transform sun position into camera space
    Sun = uSun;
    Sun.position = (uViewMatrix * vec4(Sun.position, 1.0f)).xyz;

    FragPosition = vec3(uViewMatrix * uModelMatrix * vec4(lPosition, 1.0f));
    Normal = mat3(uViewMatrix * uModelMatrix) * lNormal;
    TexIndex = lTexIndex;
    IsHighlighted = lIsHighlighted;
    TexCoords = lTexCoords;
}