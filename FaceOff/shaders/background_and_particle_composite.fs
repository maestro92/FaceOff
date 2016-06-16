#version 330                                                                        

in vec2 vf_UV;                                                                                   
uniform sampler2D u_backgroundTexture;                                                        
uniform sampler2D u_particlesTexture;                                                        
                                                                                                                                   
out vec4 FragColor;                                                                 
      
                                                                                    
void main()                                                                         
{                                                                                                  
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

	vec4 dest = texture2D(u_backgroundTexture, tc);
	vec4 src = texture2D(u_particlesTexture, tc);

	float particle_alpha = 1 - src.a;

	FragColor.rgb = src.rgb * particle_alpha + 
					dest.rgb * (1.0 - particle_alpha);
	
	FragColor.a = 1.0;   
}


