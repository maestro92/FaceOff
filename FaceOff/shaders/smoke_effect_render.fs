#version 330                                                                        
                                                                                    
uniform sampler2D u_texture;                                                        
                                                                                    
in vec2 gf_UV;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{   
  FragColor = vec4(1.0,1.0,0.0,1.0);
}
