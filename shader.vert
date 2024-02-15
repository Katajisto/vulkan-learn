#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout( push_constant ) uniform constants
{
	int frameNumber;
} FrameInfo;

layout(location = 0) out vec3 fragColor;

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, s, -s, c);
	return m * v;
}

void main() {
    gl_Position = vec4(rotate(inPosition, FrameInfo.frameNumber * 0.04), 0, 1.0);
    fragColor = inColor * (sin(FrameInfo.frameNumber * 0.01)+1)*0.5;
}