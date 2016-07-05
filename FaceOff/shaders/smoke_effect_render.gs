#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;  

// pipeline uniforms
uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelViewMat;
uniform mat4 u_modelMat;
uniform vec3 u_eyePoint;

// object uniforms
uniform float u_billBoardSize;

out vec2 gf_UV;

uniform float u_angle;




mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

mat4 scaleMatrix(vec3 scale)
{
    return mat4(scale.x,    0.0,        0.0,        0.0,
                0.0,        scale.y,    0.0,        0.0,
                0.0,        0.0,        scale.z,    0.0,
                0.0,        0.0,        0.0,        1.0);

}

void main()
{
    vec3 pos = vec3(u_modelMat * vec4(gl_in[0].gl_Position.xyz, 1.0));
    vec3 toCamera = normalize(u_eyePoint - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * u_billBoardSize;

    up = cross(right, toCamera);
    up = normalize(up);

    pos = gl_in[0].gl_Position.xyz;


    toCamera = vec3(toCamera.x, 0.0, toCamera.z);
    mat4 rot = rotationMatrix(toCamera, u_angle);

    up = vec3(0.0, 1.0, 0.0);
    up = vec3(rot * vec4(up, 1.0));
    right = vec3(rot * vec4(right, 1.0));

    up = normalize(up);
    right = normalize(right);


    right = u_billBoardSize * right;
    up = u_billBoardSize * up;






    // left bottom (0.0, 0.0)
    pos -= right * 0.5; 
    pos -= up * 0.5;
    gf_UV = vec2(0.0, 0.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();


    // left top (0.0, 1.0)
    pos += up;
    gf_UV = vec2(0.0, 1.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();


    // right bottom (1.0, 0.0)
    pos -= up;
    pos += right;
    gf_UV = vec2(1.0, 0.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();


    // right top (1.0, 1.0)
    pos += up;
    gf_UV = vec2(1.0, 1.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();


    EndPrimitive();      
}



/*
void main()
{
    vec3 pos = vec3(u_modelMat * vec4(gl_in[0].gl_Position.xyz, 1.0));
    vec3 toCamera = normalize(u_eyePoint - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * u_billBoardSize;

    up = cross(right, toCamera);
    up = normalize(up);

    pos = gl_in[0].gl_Position.xyz;


    toCamera = vec3(toCamera.x, 0.0, toCamera.z);
    mat4 rot = rotationMatrix(toCamera, u_angle);

    up = vec3(0.0, 1.0, 0.0);
    up = vec3(rot * vec4(up, 1.0));
    right = vec3(rot * vec4(right, 1.0));

    up = normalize(up);
    right = normalize(right);


    right = u_billBoardSize * right;
    up = u_billBoardSize * up;

    pos -= right * 0.5;
    gf_UV = vec2(0.0, 0.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();

    pos += up;
    gf_UV = vec2(0.0, 1.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();

    pos -= up;
    pos += right;
    gf_UV = vec2(1.0, 0.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();

    pos += up;
    gf_UV = vec2(1.0, 1.0);
    gl_Position = u_modelViewProjMat * vec4(pos, 1.0);
    EmitVertex();


    EndPrimitive();      
}
*/

