#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;

uniform mat4 projection;

out vec2 TexCoord;

void main(){
    TexCoord = texCoord;
    gl_Position = projection * vec4(position.x, position.y, position.z, 1.0f);
}