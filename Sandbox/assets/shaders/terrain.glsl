#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aStats;
layout(location = 4) in float aOverlay;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vWorldPosition;
out vec3 vStats;
out float vOverlay;

void main()
{
    vPosition = aPosition;
    vNormal = aNormal;
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
    vWorldPosition = vec3(uTransform * vec4(aPosition, 1.0));
	vStats = aStats;
    vOverlay = aOverlay;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vWorldPosition;
in vec3 vStats;
in float vOverlay;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform vec4 uTextureCutoffs;
uniform bool uDisplayOverlay;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

    vec3 n = normalize(vNormal);
    vec3 lightDir = normalize(vec3(0.0f, 0.0f, 15.0f) - vWorldPosition);
    float diff = max(dot(n, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

    vec4 texMix;
    float ratio;

    // TODO: Biomes
    /*                Zone 0    Zone 1    Zone 2          Zone 3
        Default:      Water     Beach     Grassland       Snow
        LowM, LowH:                       Rocky desert    Rock
        LowM, HighH:                      Sandy desert    Rock
        HighM, LowH:                      Tundra/snow     Snow
        HighM, HighH:                     Rainforest      Rock
    */
    if (vPosition.z < uTextureCutoffs.g) {
        ratio = (vPosition.z - uTextureCutoffs.r) / (uTextureCutoffs.g - uTextureCutoffs.r);
        texMix = mix(texture( uTexture0, vTexCoord ), texture( uTexture1, vTexCoord ), ratio);
    } else if (vPosition.z < uTextureCutoffs.b) {
        ratio = (vPosition.z - uTextureCutoffs.g) / (uTextureCutoffs.b - uTextureCutoffs.g);
        texMix = mix(texture( uTexture1, vTexCoord ), texture( uTexture2, vTexCoord ), ratio);
    } else if (vPosition.z < uTextureCutoffs.a) {
        ratio = (vPosition.z - uTextureCutoffs.b) / (uTextureCutoffs.a - uTextureCutoffs.b);
        ratio = ratio * ratio;

        if (vStats.x >= 1.0)     // lakes/rivers
            texMix = texture( uTexture0, vTexCoord );
        else if (vStats.x < 0.5) // Arid land
            texMix = mix(vec4(0.85, 0.4, 0.1, 1.0), texture(uTexture2, vTexCoord), 0.5 + vStats.x / 2);
        else                     // Grass land
            texMix = texture(uTexture2, vTexCoord);

        texMix = mix(texMix, texture( uTexture3, vTexCoord ), ratio);
	} else {
        texMix = texture( uTexture3, vTexCoord );
    }

    if (uDisplayOverlay) {
        texMix = mix(texMix, vec4(1.0f - vOverlay, vOverlay, 0.0f, 1.0f), 0.5);
    }
    if (vStats.z > 0) {
        texMix = mix(texMix, vec4(1.0f, 1.0, 1.0, 1.0f), vStats.z / 2);
    }
    color = vec4(ambient + diffuse, 1.0f) * texMix;
}
