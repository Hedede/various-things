#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;

uniform vec3 offset;
uniform vec2 camera;
uniform mat4 perspective;

void main()
{
	vec4 cam_pos  = position;
	cam_pos.xyz  += offset;
	vec4 clip_pos = perspective * cam_pos;
	clip_pos.xy  += camera;

	gl_Position = clip_pos;
	theColor = color;
}
