#version 330

out vec4 outputColor;

in vec2 texCoord;
in vec4 lightColor;

uniform sampler2D theTexture;

void main()
{
    vec4 texColor = texture(theTexture, texCoord);
    if (texColor.a == 0.0) {
        discard;
    }
    vec3 light = clamp(lightColor.rgb, 0.0, 1.0);
    light = mix(light, pow(light, vec3(1.5)), 0.5);
    outputColor = vec4(texColor.rgb *= light, 1);
}