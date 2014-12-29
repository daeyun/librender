#pragma once
#include <string>
#include "shader_source.h"

static ShaderSource kBlinnShader = {
"#version 330 core\n"
"layout(location = 0) in vec3 VertexPosition;"
"layout(location = 1) in vec3 VertexNormal;"
"layout(location = 2) in vec4 VertexColor;"
"layout(location = 3) in vec2 VertexTexCoord;"
"uniform mat4 MVPMatrix;"
"uniform mat4 MVMatrix;"
"uniform mat4 NormalMatrix;"
"out vec4 Color;"
"out vec3 Normal;"
"out vec4 Position;"
"void main() {"
"    Color = VertexColor;"
"    Normal = VertexNormal;"
"    Position = vec4(VertexPosition, 1);"
"    gl_Position = MVPMatrix * vec4(VertexPosition, 1);"
"}",
"#version 330 core\n"
"struct LightProperties {"
"    bool isEnabled;"
"    vec3 Ambient;"
"    vec3 LightColor;"
"    vec3 LightPosition;"
"    float ConstantAttenuation;"
"    float LinearAttenuation;"
"    float QuadraticAttenuation;"
"};"
"const int NumMaxLights = 20;"
"uniform int NumLights;"
"uniform vec3 EyeDirection;"
"uniform float Shininess;"
"uniform float Strength;"
"uniform LightProperties Lights[NumMaxLights];"
"in vec4 Color;"
"in vec3 Normal;"
"in vec4 Position;"
"out vec4 FragColor;"
"void main() {"
"    vec3 normal = normalize(Normal);"
"    vec4 col = vec4(0, 0, 0, 1);"
"    for (int lightIndex = 0; lightIndex < NumLights; lightIndex++) {"
"        if (!Lights[lightIndex].isEnabled) {"
"            continue;"
"        }"
"        vec3 lightDir = Lights[lightIndex].LightPosition - vec3(Position);"
"        float lightDistance = length(lightDir);"
"        lightDir = lightDir / lightDistance;"
"        float lambertian = max(dot(lightDir, normal), 0.0);"
"        float specular = 0.0;"
"        float attenuation = 1.0 /"
"            (Lights[lightIndex].ConstantAttenuation +"
"             Lights[lightIndex].LinearAttenuation * lightDistance +"
"             Lights[lightIndex].QuadraticAttenuation * lightDistance * lightDistance);"
"        if(lambertian > 0.0) {"
"            vec3 halfDir = normalize(lightDir + EyeDirection);"
"            float specAngle = max(dot(halfDir, normal), 0.0);"
"            specular = pow(specAngle, Shininess) * Strength;"
"        }"
"        col += vec4(mix(Lights[lightIndex].Ambient, vec3(Color), 0.35) * attenuation +"
"                lambertian * vec3(Color) * attenuation +"
"                specular * mix(Lights[lightIndex].LightColor, vec3(Color), 0.35) * attenuation, 1.0);"
"    }"
"    FragColor = col;"
"}"
};