#version 430

in vec3 myPos;
in vec3 myColor;
out vec4 color;
uniform vec3 center;

void main(void)
{
	float distance2=sqrt((gl_FragCoord.x-(center.x+1)*300)*(gl_FragCoord.x-(center.x+1)*300)+(gl_FragCoord.y-(center.y+1)*300)*(gl_FragCoord.y-(center.y+1)*300));
	color = vec4(myColor, 1.0);
}