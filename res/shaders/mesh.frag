#version 330

out vec4 outputColor;

in vec2 texCoord;
in vec4 lightColor;

uniform sampler2D theTexture;

void main()
{
    vec4 texColor = texture(theTexture, texCoord);
    outputColor = vec4(texColor.rgb *= min(vec3(1.0), lightColor.rgb), 1);
    //outputColor = lightColor;
}