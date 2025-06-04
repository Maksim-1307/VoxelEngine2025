#version 330

out vec4 outputColor;

in vec2 texCoord;

uniform sampler2D theTexture;

void main()
{
    vec4 color = texture(theTexture, texCoord);
    outputColor = color;
}