#version 330 core

struct LightStruct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPosition;
in vec3 Normal;
in LightStruct Sun;
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
    float darkenFactor = 0.6f;

    float r = colour.r - (colour.r * darkenFactor);
    float g = colour.g - (colour.g * darkenFactor);
    float b = colour.b - (colour.b * darkenFactor);

    return colour - vec4(r, g, b, colour.a);
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
    vec3 normal = normalize(Normal);
    vec3 cameraPosition = vec3(0.0f);

    /** BASE **/
    vec4 baseColour = colourFromBlockType();

    /** AMBIENT **/
    vec4 ambientLight = baseColour * vec4(Sun.ambient, 1.0f);

    /** DIFFUSE **/
    vec3 lightDirection = normalize(Sun.position - FragPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
    vec4 diffuseLight = baseColour * (vec4(Sun.diffuse, 1.0f) * diffuseStrength);

    /** SPECULAR **/
    float shininess = 5.0f;

    // Get the direction from this fragment to the camera
    vec3 cameraDirection = normalize(cameraPosition - FragPosition);
    // Get the reflected direction from the fragment surface
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    // How in-line this is with the current camera direction is the amount of specular strength.
    float specularAmount = pow(max(dot(cameraDirection, reflectionDirection), 0.0f), shininess);
    vec4 specularLight = specularAmount * baseColour;

    vec4 litColour = ambientLight + diffuseLight;

    /** HIGHLIGHT MODIFIER **/
    if (IsHighlighted == 0) {
        FragColour = litColour;
        return;
    }

    FragColour = darken(litColour);
}