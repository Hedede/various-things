#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
smooth out vec2 frag_pos;
smooth out vec4 frag_color;

uniform vec2 center;
uniform float radius;
uniform mat4 view_matrix;
uniform mat4 transform;

flat out vec2 cpos;

void main()
{
        gl_Position = view_matrix * transform * position;
        frag_pos = (transform * position).xy;
        frag_color = color;
        cpos = vec2(transform * vec4(center,0,1));
        //cpos = center;
}
