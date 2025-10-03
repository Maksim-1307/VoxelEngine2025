#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aPackedData;

out vec2 texCoord;
out vec4 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    // make uniform
    float skyBrightness = 1.0;
    vec3 skyColor = vec3(1, 1, 1);

    // unpacking float to two bytes
    uint raw_bits = floatBitsToUint(aPackedData);
    uint byte1 = raw_bits & 0xFFFFu; 
    uint byte2 = (raw_bits >> 16) & 0xFFFFu;

    // unpacking light
    uint packed16 = byte1;
    float r = float(uint((packed16 >> 12) & 0xFu) + uint(2)) / 17.0; 
    float g = float(uint((packed16 >> 8)  & 0xFu) + uint(2)) / 17.0; 
    float b = float(uint((packed16 >> 4)  & 0xFu) + uint(2)) / 17.0; 
    float s = float(uint(packed16         & 0xFu) + uint(2)) / 17.0; 

    // unpacking face orientation
    uint face = uint(byte2);

    vec3 totalLight =  skyColor * skyBrightness * s;

    float faceDarkeing[6] = float[](0.2f, 0.7f, 0.0f, 0.8f, 0.3f, 0.5f);
    float faceFactor = 0.5;
    float darkeing = faceDarkeing[face] * faceFactor;

    lightColor = vec4(mix(totalLight, vec3(0), darkeing), 1);
    texCoord = aTexCoord;
    gl_Position = projection * view * model  * vec4(aPosition, 1.0);
}