#version 330

uniform float u_x1;
uniform float u_x2;
uniform float u_y1;
uniform float u_y2;

out vec4 FragColor;

void main()
{
	if(gl_FragCoord.x < u_x1 || u_x2 < gl_FragCoord.x || gl_FragCoord.y < u_y1 || u_y2 < gl_FragCoord.y)   
		FragColor = vec4(1.0, 1.0, 0.0, 1.0);

	else
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}