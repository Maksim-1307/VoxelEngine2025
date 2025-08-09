#version 330

out vec4 outputColor;

in vec2 texCoord;
in vec4 lightColor;

uniform sampler2D theTexture;

void main()
{
    vec4 texColor = texture(theTexture, texCoord);
    outputColor = mix(texColor, lightColor, 0.8);
    // outputColor = lightColor;
}