#version 330                     
           
layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;
           
in float type0[];
in vec3 position0[];
in vec3 velocity0[];
in float age0[]; 
           
out float type1; 
out vec3 position1;
out vec3 velocity1;
out float age1;
           
uniform float u_deltaTimeMillis;
uniform float u_time;
uniform sampler1D u_randomTexture;
uniform float u_launcherLifeTime; 
uniform float u_shellLifeTime;
uniform float u_secondaryShellLifeTime;
           

#define PARTICLE_GRAVITY_VEL_CONSTANT 0.05f
#define PARTICLE_EXPLODE_VEL_CONSTANT 10.0f
#define PARTICLE_FLOAT_VEL_CONSTANT 0.0001f

#define PARTICLE_TYPE_LAUNCHER 0.0f   
#define PARTICLE_TYPE_EXPLODE 1.0f 
#define PARTICLE_TYPE_FLOAT 2.0f  

#define PARTICLE_LAUNCHER_LIFE_TIME 300.0f
#define PARTICLE_EXPLODE_LIFE_TIME 340.0f
#define PARTICLE_FLOAT_LIFE_TIME 10000000.0f


const float seedRadius = 0.25;

const float TwoPi = 6.28318530718;
const float UINT_MAX = 4294967295.0;

float dt = 0.01f;

uint randhash(uint seed)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return i;
}


float randhashf(uint seed, float b)
{
    return float(b * randhash(seed)) / UINT_MAX;
}


vec3 GetRandomDir(float TexCoord)
{          
     vec3 dir = texture(u_randomTexture, TexCoord).xyz;
     dir -= vec3(0.5, 0.5, 0.5); 
     return dir; 
}          

           
float rand(vec2 n)
{
  return 0.5 + 0.5 * 
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{          
    float age = age0[0] + u_deltaTimeMillis; 
    // float age = age0[0] + dt;


    // launcher, these particles don't actually move
    // we start initialize them with a random speed when we hit the EXPLODE_START_TIME
    // and that becomes a shell

    // explode mode
    if (type0[0] == PARTICLE_TYPE_LAUNCHER)
    {
        // time to initalize
        if(age >= PARTICLE_LAUNCHER_LIFE_TIME)
        {
            type1 = PARTICLE_TYPE_EXPLODE;
            position1 = position0[0];
            velocity1 = velocity0[0];
            age1 = 0.0;
            EmitVertex();
            EndPrimitive();
            age = 0.0; 
        }
        else
        {
            // we put it back to the buffer and wait till we get to the start time
            type1 = PARTICLE_TYPE_LAUNCHER; 
            position1 = position0[0];
            velocity1 = velocity0[0];
            age1 = age;
            EmitVertex();
            EndPrimitive();
        }
    }

    // PARTICLE_TYPE_EXPLODE or PARTICLE_TYPE_FLOAT
    else
    { 
        // gravity
        vec3 dx = dt * velocity0[0];
        vec3 dv = vec3(dt) * (0.0, -9.81, 0.0) * PARTICLE_GRAVITY_VEL_CONSTANT;

        if (type0[0] == PARTICLE_TYPE_EXPLODE)
        {
            // if still a explode particle
            if (age < PARTICLE_EXPLODE_LIFE_TIME) 
            { 
                type1 = PARTICLE_TYPE_EXPLODE;
                position1 = position0[0] + dx;
                velocity1 = velocity0[0] + dv;
                age1 = age;
                EmitVertex();
                EndPrimitive();
            }

            // once it reaches the explode life time
            // change it to float particle
            else 
            {
                type1 = PARTICLE_TYPE_FLOAT; 
                position1 = position0[0];

              //  float r = randhashf(seed++, seedRadius);
              //  vec3 dir = GetRandomDir(r);

                vec3 dir = GetRandomDir((u_time)/1000.0); 
                velocity1 = normalize(dir) * PARTICLE_FLOAT_VEL_CONSTANT;
                age1 = 0.0f;
                EmitVertex();
                EndPrimitive();
            }
        }

        
        else // PARTICLE_TYPE_FLOAT
        {
            // if still alive, we leave in the system
            if (age < PARTICLE_FLOAT_LIFE_TIME) 
            {
                type1 = PARTICLE_TYPE_FLOAT;
                position1 = position0[0] + dx;
                velocity1 = velocity0[0] + dv;
                age1 = age;
                EmitVertex();
                EndPrimitive();
            }
            // else, it just dies
        }  
    } 


    // else we let it die
}          
