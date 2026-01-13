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
uniform LightStruct uLight;

out vec3 FragPosition;
out LightStruct Light;
out vec3 Normal;
out vec2 TexCoords;
flat out int TexIndex;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(lPosition, 1.0f);

    FragPosition = vec3(uModelMatrix * vec4(lPosition, 1.0f));

    Light = uLight;
    Light.position = (vec4(Light.position, 1.0f)).xyz;

    Normal = mat3(uModelMatrix) * lNormal;
    TexIndex = lTexIndex;
    TexCoords = lTexCoords;
}