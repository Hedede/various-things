#version 330
smooth in vec4 theColor;

out vec4 outputColor;

void main()
{
	vec4 color1 = mix(vec4(1,0,0,1), vec4(0,1,1,1), gl_FragCoord.x / 800);
	vec4 color2 = mix(vec4(0,0,1,1), vec4(1,1,0,1), gl_FragCoord.y / 800);
	vec4 color3 = mix(color1, color2, gl_FragCoord / 800);

	outputColor = mix(theColor, color3, 0.5);
}
