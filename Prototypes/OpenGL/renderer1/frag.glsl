#version 330
smooth in vec4 theColor;

out vec4 outputColor;
uniform vec2 screen;

void main()
{
	vec4 color1 = mix(vec4(1,0,0,1), vec4(0,1,1,1), gl_FragCoord.x / screen.x);
	vec4 color2 = mix(vec4(0,0,1,1), vec4(1,1,0,1), gl_FragCoord.y / screen.y);
	vec4 color3 = mix(color1, color2, gl_FragCoord / sqrt(screen.x*screen.y));

	outputColor = mix(theColor, color3, 0.5);
}
