/**
 * @file blinn.f.glsl
 * @brief Shader based on the Blinn-Phong model.
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software under the BSD 2-Clause license.
 */
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

out vec4 FragColor;

void main() {
    vec3 normal = normalize(Normal);
    vec4 col = vec4(0, 0, 0, 1);

    for (int lightIndex = 0; lightIndex < NumLights; lightIndex++) {
        if (!Lights[lightIndex].isEnabled) {
            continue;
        }

        vec3 lightDir = Lights[lightIndex].LightPosition - vec3(Position);
        float lightDistance = length(lightDir);

        lightDir = lightDir / lightDistance;

        float lambertian = max(dot(lightDir, normal), 0.0);
        float specular = 0.0;

        float attenuation = 1.0 /
            (Lights[lightIndex].ConstantAttenuation +
             Lights[lightIndex].LinearAttenuation * lightDistance +
             Lights[lightIndex].QuadraticAttenuation * lightDistance * lightDistance);

        if(lambertian > 0.0) {
            vec3 halfDir = normalize(lightDir + EyeDirection);
            float specAngle = max(dot(halfDir, normal), 0.0);
            specular = pow(specAngle, Shininess) * Strength;
        }

        col += vec4(mix(Lights[lightIndex].Ambient, vec3(Color), 0.35) * attenuation +
                lambertian * vec3(Color) * attenuation +
                specular * mix(Lights[lightIndex].LightColor, vec3(Color), 0.35) * attenuation, 1.0);
    }
    FragColor = col;
}
