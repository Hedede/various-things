#version 330
out vec4 out_color;
smooth in vec2 frag_pos;
smooth in vec4 frag_color;
flat   in vec2 cpos;
uniform float radius;
uniform mat4 view_matrix;

void main()
{
        vec2 dist = frag_pos.xy - cpos;
        float len = dot(dist,dist);
        float r = radius;
        if (len > r*r)
                //discard;
                out_color = vec4(0,0,0,0);
        else if (len > r*r*0.85)
                out_color = vec4(0,0,0,1);
        else
                out_color = frag_color;
}
