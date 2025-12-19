#version 330 core

layout (location = 0) in vec3 lPosition;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 FragPosition;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * vec4(lPosition, 1.0f);
    FragPosition = gl_Position.xyz;
}