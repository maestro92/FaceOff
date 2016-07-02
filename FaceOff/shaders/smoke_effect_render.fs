#version 330


in vec4 gf_pos;
in vec2 gf_UV;
out vec4 FragColor;


uniform sampler2D u_texture;
uniform sampler2D u_depthTexture;

uniform vec2 u_inverseScreenSize;
uniform float u_zNear;
uniform float u_zFar;

uniform mat4 u_modelViewProjMat;

/*
float computeZ(float z_b)
{
    float numer = 2.0 * u_zFar * u_zNear;
    float denom = u_zFar + u_zNear - (u_zFar - u_zNear) * (2*z_b - 1);
    float z_e = numer / denom;
    return z_e;
}
*/


float computeZ(vec3 Depth_Vertex)
{
    vec4 clipDepthPosition = u_modelViewProjMat * vec4(Depth_Vertex,1.0);
    float depthValue = clipDepthPosition.z / clipDepthPosition.w; // Depth in NDC coordinates
    return (depthValue + 1.0) * 0.5;    // Depth value in range [0,1], same range as the depth buffer
}



void main()
{
    
    vec2 tc = gl_FragCoord.xy * u_inverseScreenSize;

    float sceneDepth = texture2D(u_depthTexture, tc).r;

 //   float particleDepth = computeZ(gf_pos.xyz);

   // if(sceneDepth < particleDepth)
    //    discard;


  //  float scene_z = computeZ(depth);
  //  float particle_z = computeZ(gl_FragCoord.z);

    if(sceneDepth < gl_FragCoord.z)
    {
        discard;
    }


    FragColor = vec4(1.0,1.0,0.0,1.0);

//    sceneDepth = pow(sceneDepth,32);
//    FragColor = vec4(sceneDepth, sceneDepth, sceneDepth, 1.0);
    
  //  FragColor = vec4(1.0,1.0,0.0,1.0);
}
