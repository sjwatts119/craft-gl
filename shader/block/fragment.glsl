#version 330 core

struct LightStruct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoords;
flat in int TexIndex;
in LightStruct Sun;
flat in int IsHighlighted;

uniform sampler2DArray uBlockTextures;

out vec4 FragColour;

/*
    ERROR = -2,
    AIR = -1,

    GRASS_BLOCK_TOP = 0,
    GRASS_BLOCK_SIDE = 1,
    DIRT = 2,
    STONE = 3,
    BEDROCK = 4,
    DIAMOND_BLOCK = 5,
*/

vec4 darken(vec4 colour) {
    float darkenFactor = 0.6f;

    float r = colour.r - (colour.r * darkenFactor);
    float g = colour.g - (colour.g * darkenFactor);
    float b = colour.b - (colour.b * darkenFactor);

    return colour - vec4(r, g, b, colour.a);
}

vec4 colourFromBlockType() {
    // Error
    if (TexIndex == -2) {
        return vec4(1.0f, 0.0f, 1.0f, 1.0f);
    }
    // Air
    else if (TexIndex == -1) {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else {
        return texture(uBlockTextures, vec3(TexCoords, TexIndex));
    };
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
//    float shininess = 5.0f;
//
//    // Get the direction from this fragment to the camera
//    vec3 cameraDirection = normalize(cameraPosition - FragPosition);
//    // Get the reflected direction from the fragment surface
//    vec3 reflectionDirection = reflect(-lightDirection, normal);
//    // How in-line this is with the current camera direction is the amount of specular strength.
//    float specularAmount = pow(max(dot(cameraDirection, reflectionDirection), 0.0f), shininess);
//    vec4 specularLight = specularAmount * baseColour;

    vec4 litColour = ambientLight + diffuseLight;

    /** HIGHLIGHT MODIFIER **/
    if (IsHighlighted == 0) {
        FragColour = litColour;
        return;
    }

    FragColour = darken(litColour);
}