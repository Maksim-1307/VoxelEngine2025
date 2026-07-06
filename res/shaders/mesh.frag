#version 330

out vec4 outputColor;

in vec2 texCoord;
in vec4 lightColor;
in vec3 v_Position;

uniform sampler2D theTexture;
uniform vec3 fogColor;
uniform float fogStart = 50.0;
uniform float fogDistance;
uniform float fogDensity = 5.0;

void main()
{
    vec4 texColor = texture(theTexture, texCoord);
    if (texColor.a == 0.0) {
        discard;
    }
    vec3 light = clamp(lightColor.rgb, 0.0, 1.0);
    light = mix(light, pow(light, vec3(1.5)), 0.5);
    vec3 terrainColor = texColor.rgb *= light;

    float distance = length(v_Position);
    float t = (distance - fogStart) / (fogDistance - fogStart);
    t = clamp(t, 0.0, 1.0);
    float fogFactor = exp(-fogDensity * t);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    outputColor = vec4(mix(fogColor, terrainColor, fogFactor), texColor.a);
}