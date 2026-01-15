#version 330 core

struct LightStruct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPosition;
in LightStruct Light;
in vec3 Normal;
in vec2 TexCoords;
flat in int TexIndex;

uniform sampler2DArray uBlockTextures;
uniform LightStruct uLight;

out vec4 FragColour;

vec4 colourFromBlockType() {
    return texture(uBlockTextures, vec3(TexCoords, TexIndex));
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 cameraPosition = vec3(0.0f);

    /** BASE **/
    vec4 baseColour = colourFromBlockType();

    /** AMBIENT **/
    vec4 ambientLight = baseColour * vec4(Light.ambient, 1.0f);

    /** DIFFUSE **/
    vec3 lightDirection = normalize(Light.position - FragPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
    vec4 diffuseLight = baseColour * (vec4(Light.diffuse, 1.0f) * diffuseStrength);

    vec4 litColour = ambientLight + diffuseLight;

    if (litColour.a == 0.0f) {
        discard;
    }

    FragColour = litColour;
}