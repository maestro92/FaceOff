#version 330                                                                        
                                                                                    
uniform sampler2D u_texture; 

in vec2 gf_UV;
                                                               
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                                  
//	FragColor = vec4(1.0,0.0,0.0,1.0);
	vec2 tc = gf_UV;
	tc.y = 1 - tc.y;
    FragColor = texture2D(u_texture, tc);   
    /*
    if (FragColor.r <= 0.5 && FragColor.g <= 0.5 && FragColor.b <= 0.5) 
    {
        discard;                                                                    
    } 
    */

/*
    if (FragColor.r >= 0.5 && FragColor.g >= 0.5 && FragColor.b >= 0.5) 
    {
        discard;                                                                    
    }   
*/

    if (FragColor.a < 0.5) 
    {
        discard;                                                                    
    }   



/*                                                                               
    if (FragColor.r >= 0.99 && FragColor.g >= 0.99 && FragColor.b >= 0.9) {
        discard;                                                                    
    }  
*/                                                                               
}
