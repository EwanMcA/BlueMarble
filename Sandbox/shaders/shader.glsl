#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vWorldPosition;

void main()
{
    vPosition = aPosition;
    vNormal = aNormal;
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
    vWorldPosition = vec3(uTransform * vec4(aPosition, 1.0));
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vWorldPosition;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

    vec3 n = normalize(vNormal);
    vec3 lightDir = normalize(vec3(0.0f, 0.0f, 5.0f) - vWorldPosition);
    float diff = max(dot(n, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

    vec4 texCaps = {0.0f, 0.015f, 0.025f, 0.2f};
    vec4 texMix;
    float ratio;

    if (vPosition.z < texCaps.g) {
        ratio = (vPosition.z - texCaps.r) / (texCaps.g - texCaps.r);
        texMix = mix(texture( uTexture0, vTexCoord ), texture( uTexture1, vTexCoord ), ratio);
    } else if (vPosition.z < texCaps.b) {
        ratio = (vPosition.z - texCaps.g) / (texCaps.b - texCaps.g);
        texMix = mix(texture( uTexture1, vTexCoord ), texture( uTexture2, vTexCoord ), ratio);
    } else if (vPosition.z < texCaps.a) {
        ratio = (vPosition.z - 0.15f) / (texCaps.a - 0.15f);
        ratio = max(0.0f, ratio);
        texMix = mix(texture( uTexture2, vTexCoord ), texture( uTexture3, vTexCoord ), ratio);
    } else {
        texMix = texture( uTexture3, vTexCoord );
    }

    color = vec4(ambient + diffuse, 1.0f) * texMix;
    //color = vec4(abs(vNormal), 1.0f);
}
