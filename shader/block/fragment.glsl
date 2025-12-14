#version 330 core

in vec3 FragPosition;

uniform int uBlockType;

out vec4 FragColour;

/*
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    BEDROCK = 4,
*/

vec4 colourFromBlockType() {
    // Grass
    if (uBlockType == 1) {
        return vec4(0.329f, 0.831f, 0.329f, 1.0f);
    }
    // Dirt
    else if (uBlockType == 2) {
        return vec4(0.561f, 0.294f, 0.0f, 1.0f);
    }
    // Stone
    else if (uBlockType == 3) {
        return vec4(0.541, 0.541, 0.541, 1.0f);
    }
    // Bedrock
    else if (uBlockType == 4) {
        return vec4(0.131, 0.131, 0.131, 1.0f);
    }
    // Air
    else {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void main()
{
    FragColour = colourFromBlockType();
}