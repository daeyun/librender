#version 330 core

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

uniform int NumLights;
uniform vec3 EyeDirection;
uniform float Shininess;
uniform float Strength;
uniform LightProperties Lights[NumMaxLights];

// Interpolated values
in vec4 Color;
in vec3 Normal;
in vec4 Position;
in vec3 LightPositions[NumMaxLights];

out vec4 FragColor;

void main() {
    vec3 normal = normalize(Normal);

    vec3 ambientSum = vec3(0, 0, 0);
    vec3 diffuseSum = vec3(0, 0, 0);
    vec3 specularSum = vec3(0, 0, 0);

    for (int lightIndex = 0; lightIndex < NumLights; lightIndex++) {
        if (!Lights[lightIndex].isEnabled) {
            continue;
        }

        vec3 lightDir = LightPositions[lightIndex] - vec3(Position);
        float lightDist = length(lightDir);

        lightDir = lightDir / lightDist;

        float lambertian = max(dot(lightDir, normal), 0.0);
        float specular = 0.0;

        float attenuation = 1.0 /
            (Lights[lightIndex].ConstantAttenuation +
             Lights[lightIndex].LinearAttenuation * lightDist +
             Lights[lightIndex].QuadraticAttenuation * lightDist * lightDist);

        if(lambertian > 0.0) {
            vec3 halfDir = normalize(lightDir + EyeDirection);
            float specAngle = max(dot(halfDir, normal), 0.0);
            specular = pow(specAngle, Shininess) * Strength;
        }

        ambientSum += Lights[lightIndex].Ambient;
        diffuseSum += lambertian * vec3(Color) * attenuation;
        specularSum += specular * Lights[lightIndex].LightColor * attenuation;
    }

    FragColor = vec4(ambientSum + diffuseSum + specularSum, 1.0);
}
