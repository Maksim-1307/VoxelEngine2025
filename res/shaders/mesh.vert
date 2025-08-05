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
    // from float to uint16
    uint raw_bits = floatBitsToUint(aPackedData);
    uint packedData = raw_bits & 0xFFFFu; 
    // unpacking 
    uint packed16 = packedData;
    float r = float((packed16 >> 12) & 0xFu) / 15.0; 
    float g = float((packed16 >> 8)  & 0xFu) / 15.0; 
    float b = float((packed16 >> 4)  & 0xFu) / 15.0; 
    float s = float(packed16         & 0xFu) / 15.0; 
    
    lightColor = vec4(r, g, b, 1);
    texCoord = aTexCoord;
    gl_Position = projection * view * model  * vec4(aPosition, 1.0);
}