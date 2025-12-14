#version 330 core

layout (location = 0) in vec3 lPosition;
layout (location = 1) in int lBlockType;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 FragPosition;
flat out int BlockType;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(lPosition, 1.0f);

    FragPosition = vec3(gl_Position);
    BlockType = lBlockType;
}