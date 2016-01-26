#version 330

layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;  

// pipeline uniforms
uniform mat4 u_modelViewProjMat;    
uniform mat4 u_modelMat;                                                               
uniform vec3 u_eyePoint; 

// object uniforms
uniform vec3 u_centerPosition;   
uniform float u_billboardWidthScale;
uniform float u_billboardHeightScale;

out vec3 gf_position;
out vec3 gf_normal;
out vec2 gf_UV;


/*
float rand(vec2 n)
{
  return 0.5 + 0.5 * 
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}*/

void main()                                                                         
{                                
    vec3 pos = gl_in[0].gl_Position.xyz + u_centerPosition;                                            
    vec3 toCamera = normalize(u_eyePoint - pos);                                    
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

//    float randHeightScale = rand(gl_in[0].gl_Position.xz) * 3;
    float randHeightScale = 3;
    up = randHeightScale * up;
    right = u_billboardWidthScale * right;

    pos -= u_centerPosition;
    gf_normal = (u_modelMat * vec4(0.0, 1.0, 0.0, 0.0)).xyz;

    pos -= (right * 0.5);                                                           
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);    
    gf_position = (u_modelMat * vec4(pos, 1.0)).xyz;
    gf_UV = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_position = (u_modelMat * vec4(pos, 1.0)).xyz;
    gf_UV = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos -= up;                                                                   
    pos += right;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_position = (u_modelMat * vec4(pos, 1.0)).xyz;
    gf_UV = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_position = (u_modelMat * vec4(pos, 1.0)).xyz;
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
                                                                                  
