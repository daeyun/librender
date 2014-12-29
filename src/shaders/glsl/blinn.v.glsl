#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec4 VertexColor;
layout(location = 3) in vec2 VertexTexCoord;

struct LightProperties {
    bool isEnabled;
    vec3 Ambient;
    vec3 LightColor;
    vec3 LightPosition;

    // Attenuation coefficients
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
};

const int NumMaxLights = 20;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;
uniform LightProperties Lights[NumMaxLights];
uniform int NumLights;

out vec4 Color;
out vec3 Normal;
out vec4 Position;
out vec3 LightPositions[NumMaxLights];

void main() {
    Color = VertexColor;
    Normal = normalize(NormalMatrix * vec4(VertexNormal, 1)).xyz;
    Position = MVMatrix * vec4(VertexPosition, 1);
    gl_Position = MVPMatrix * vec4(VertexPosition, 1);

    for (int lightIndex = 0; lightIndex < NumLights; lightIndex++) {
        LightPositions[lightIndex] = (MVMatrix * vec4(Lights[lightIndex].LightPosition, 1)).xyz;
    }
}
