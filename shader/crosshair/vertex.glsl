#version 330 core

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lNormal;
layout (location = 2) in vec3 lTexCoords;

uniform mat4 uModelMatrix;

out vec3 FragPosition;

void main() {
    gl_Position = uModelMatrix * vec4(lPosition, 1.0f);
    FragPosition = gl_Position.xyz;
}