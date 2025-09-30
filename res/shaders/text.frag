#version 330 core

in vec2 TexCoord;
uniform sampler2D theTexture;

out vec4 color;

void main()
{
	color = texture(theTexture, TexCoord);
    if (color == vec4(0,0,0,1))
        color = vec4(255,255,255,1);
    else
        discard;
}