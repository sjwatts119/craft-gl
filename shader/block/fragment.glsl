#version 330 core

in vec3 FragPosition;
flat in int BlockType;
flat in int IsHighlighted;

out vec4 FragColour;

/*
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    BEDROCK = 4,
    ERROR = 5,
*/

vec4 darken(vec4 colour) {
    return colour - vec4(0.2f, 0.2f, 0.2f, 1.0f);
}

vec4 colourFromBlockType() {
    // Grass
    if (BlockType == 1) {
        return vec4(0.329f, 0.831f, 0.329f, 1.0f);
    }
    // Dirt
    else if (BlockType == 2) {
        return vec4(0.561f, 0.294f, 0.0f, 1.0f);
    }
    // Stone
    else if (BlockType == 3) {
        return vec4(0.541, 0.541, 0.541, 1.0f);
    }
    // Bedrock
    else if (BlockType == 4) {
        return vec4(0.131, 0.131, 0.131, 1.0f);
    }
    // Error
    else if (BlockType == 5) {
        return vec4(1.0f, 0.0f, 1.0f, 1.0f);
    }
    // Air
    else {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void main()
{
    vec4 baseColour = colourFromBlockType();

    if (IsHighlighted == 0) {
        FragColour = baseColour;
        return;
    }

    FragColour = darken(baseColour);
}