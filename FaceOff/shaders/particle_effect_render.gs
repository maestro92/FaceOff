#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;  

// pipeline uniforms
uniform mat4 u_modelViewProjMat;
uniform vec3 u_eyePoint;

// object uniforms
uniform vec3 u_centerPosition;   
uniform float u_billBoardSize;

out vec2 gf_UV;


void main()
{                                
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(u_eyePoint - pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * u_billBoardSize;                              

    up = cross(right, toCamera);
    up = normalize(up);

    pos -= right * 0.5;
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(0.0, 0.0);
    EmitVertex();

    pos.y += u_billBoardSize;
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= u_billBoardSize;
    pos += right;
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += u_billBoardSize;
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);                                             
    gf_UV = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();      
}




