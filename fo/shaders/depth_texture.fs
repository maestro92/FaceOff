#version 330
uniform sampler2D u_texture;

in vec2 vf_UV;
out vec4 FragColor;

void main()
{
//	vec2 pos = gl_FragCoord.xy * gPixelSize;	

// if it's a depth texture
#if 0	
	vec4 value = texture2D(gTexture,pos);
	value.x = pow(value.x,32);
	value.y = pow(value.y,32);
	value.z = pow(value.z,32);
	FragColor = value;
#else
	vec2 tc = vf_UV;	 // tc.y = 1.0 - tc.y;
	// pos = tc;

	
	FragColor = texture(u_texture, tc);
/*
	if(gl_FragCoord.y > 200)
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);	
*/

	// FragColor = vec4(0.0, 1.0, 0.0, 1.0);

//	FragColor = vec4(1.0,0.0,0.0,1.0);
//	FragColor = vec4(outUV.x, outUV.x, outUV.x, 1.0);
//	FragColor = vec4(outUV.y, outUV.y, outUV.y, 1.0);
#endif
}


