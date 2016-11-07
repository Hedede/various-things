#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;

uniform vec2 camera;
uniform float zNear;
uniform float zFar;
uniform float frustum_scale;

void main()
{
	vec4 cam_pos = position + vec4(camera.x, camera.y, 0, 0);
	vec4 clip_pos = cam_pos;

	clip_pos.xy = cam_pos.xy * frustum_scale;
	
	clip_pos.z  = cam_pos.z * (zNear + zFar) / (zNear - zFar);
	clip_pos.z += 2 * zNear * zFar / (zNear - zFar);

	clip_pos.w = -cam_pos.z;

	gl_Position = clip_pos;
	theColor = color;
}
