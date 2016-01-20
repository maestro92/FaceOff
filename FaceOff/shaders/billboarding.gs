#version 330

layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;  

uniform mat4 u_modelViewProjMat;                                                                   
uniform vec3 u_cameraPosition;    
uniform float u_billboardWidthScale;
uniform float u_billboardHeightScale;
out vec2 gf_UV;

void main()                                                                         
{                                
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(u_cameraPosition - pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up);                                               
    right = normalize(right);

    up = cross(right, toCamera);
    up = normalize(up);

    // we're growing a quad around a point
    
    // the initial point is considered to be
    // at the center of the bottom of the quad                                                             
    
    // generating two front facing triangles from it
    // subtracting half of right 

    up = u_billboardHeightScale * up;
    right = u_billboardWidthScale * right;

    pos -= (right * 0.5);                                                           
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos -= up;                                                                   
    pos += right;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();        
}










/*                                                      
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(gCameraPosition - pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
 //   vec3 right = cross(toCamera, up);                                               
    vec3 right = cross(gCameraViewDir, up);
                     
    // we're growing a quad around a point
    
    // the initial point is considered to be
    // at the center of the bottom of the quad                                                             
    
	// generating two front facing triangles from it
	// subtracting half of right 
    pos -= (right * 0.5);                                                           
    gl_Position = gModelViewProjectionMatrix * vec4(pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y += 1.0;                                                                   
    gl_Position = gModelViewProjectionMatrix * vec4(pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y -= 1.0;                                                                   
    pos += right;                                                                   
    gl_Position = gModelViewProjectionMatrix * vec4(pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y += 1.0;                                                                   
    gl_Position = gModelViewProjectionMatrix * vec4(pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();    
    */                                                             
                                                                                  
