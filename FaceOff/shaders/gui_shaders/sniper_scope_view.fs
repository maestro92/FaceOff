#version 330                                                                        

in vec2 vf_UV;                                                                                   
uniform sampler2D u_sniperScopeViewTexture;     
uniform sampler2D u_sceneTexture;                                                        
                                                   
out vec4 FragColor;                                                                 
      
                                                                                    
void main()                                                                         
{                                                                                                  
    vec2 tc = vf_UV;    tc.y = 1.0 - tc.y;

    vec4 sniperView = texture2D(u_sniperScopeViewTexture, tc);
    vec4 scene = texture2D(u_sceneTexture, tc);

    float factor = sniperView.x;

    FragColor.rgb = scene.rgb * factor;
	// FragColor.rgb = scene.rgb;
 
	// FragColor.rgb = sniperView.rgb;
            
}

