#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragNormal;
layout(location = 4) in vec3 fragFromCamera;

layout(binding = 1) uniform sampler2D logoSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightPosition = vec3(3.0, -2.0, 3.0);
    float toLight = length(fragPos - lightPosition);
    vec3 lightDir = normalize(fragPos - lightPosition);

    float lambert = max(dot(lightDir, fragNormal), 0.0);
    float specular = 0.0;

    if(lambert > 0.1) {
        vec3 vDir = normalize(-fragFromCamera);
        vec3 halfDir = normalize(lightDir + vDir);
        float specAngle = max(dot(halfDir, fragNormal), 0.0);
        specular = pow(specAngle, 40.0);
    }

    vec3 ambient = vec3(0.05, 0.05, 0.05);
    vec3 diffColor = vec3(1.0, 0.9, 0.75) * 0.6;
    vec3 specColor = vec3(1.0, 1.0, 1.0);

    vec3 lightFactor = ambient + diffColor * lambert * 1/((toLight/7));

    // outColor = vec4(fragColor * texture(logoSampler, abs(fragTexCoord * 24.0)).rgb, 1.0);
    vec3 outC = vec3(texture(logoSampler, abs(fragTexCoord * 24.0)).rgb * lightFactor + specular * specColor);
    vec3 colorAfterGamma = pow(outC, vec3(1.0 / 2.2));
    outColor = vec4(colorAfterGamma, 1.0);
}