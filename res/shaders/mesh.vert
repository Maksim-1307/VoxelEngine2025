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
    // unpacking float to two bytes
    uint raw_bits = floatBitsToUint(aPackedData);
    uint byte1 = raw_bits & 0xFFFFu; 
    uint byte2 = (raw_bits >> 16) & 0xFFFFu;

    // unpacking light
    uint packed16 = byte1;
    float r = float((packed16 >> 12) & 0xFu) / 15.0; 
    float g = float((packed16 >> 8)  & 0xFu) / 15.0; 
    float b = float((packed16 >> 4)  & 0xFu) / 15.0; 
    float s = float(packed16         & 0xFu) / 15.0; 

    // unpacking face orientation
    uint face = uint(byte2);

    float faceDarkeing[6] = float[](0.7f, 0.2f, 0.8f, 0.0f, 0.5f, 0.3f);

    float darkeing = faceDarkeing[face];
    float factor = 0.05;
    lightColor = mix(vec4(r, g, b, 1), vec4(darkeing, darkeing, darkeing, 1), factor);
    texCoord = aTexCoord;
    gl_Position = projection * view * model  * vec4(aPosition, 1.0);
}