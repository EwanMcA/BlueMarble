#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPosition;
out vec2 vTexCoord;

void main()
{
    vPosition = aPosition;
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec2 vTexCoord;

uniform sampler2D uTexture1;

void main()
{
    color = texture( uTexture1, vTexCoord ) * vec4(vPosition * 0.05 + 0.5, 1.0f);
}
