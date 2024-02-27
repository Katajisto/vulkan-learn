#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout( push_constant ) uniform constants
{
	int frameNumber;
} FrameInfo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, s, -s, c);
	return m * v;
}

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor * (sin(FrameInfo.frameNumber * 0.01)+1.5)*0.5;
    fragTexCoord = inTexCoord;
}