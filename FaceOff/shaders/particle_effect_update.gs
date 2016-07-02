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
                                                                                    
#define PARTICLE_TYPE_LAUNCHER 0.0f                                                 
#define PARTICLE_TYPE_SHELL 1.0f                                                    
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f                                          
                                                                                    
vec3 GetRandomDir(float TexCoord)                                                   
{                                                                                   
     vec3 Dir = texture(u_randomTexture, TexCoord).xyz;                              
     Dir -= vec3(0.5, 0.5, 0.5);                                                    
     return Dir;                                                                    
}                                                                                   
                                                                                    
void main()                                                                         
{                                                                                   
    float age = age0[0] + u_deltaTimeMillis;                                         
                                                                                    
    if (type0[0] == PARTICLE_TYPE_LAUNCHER) 
    {                                       
        if (age >= u_launcherLifeTime) 
        {                                             
            type1 = PARTICLE_TYPE_SHELL;                                            
            position1 = position0[0];                                               
            vec3 Dir = GetRandomDir(u_time/1000.0);                                  
            Dir.y = max(Dir.y, 0.5);                                                
            velocity1 = normalize(Dir) / 20.0;                                      
            age1 = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        type1 = PARTICLE_TYPE_LAUNCHER;                                             
        position1 = position0[0];                                                   
        velocity1 = velocity0[0];                                                   
        age1 = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    }                                                                               
    else 
    {                                                                          
        float DeltaTimeSecs = u_deltaTimeMillis / 1000.0f;                           
        float t1 = age0[0] / 1000.0;                                                
        float t2 = age / 1000.0;                                                    
        vec3 DeltaP = DeltaTimeSecs * velocity0[0];                                 
        vec3 DeltaV = vec3(DeltaTimeSecs) * (0.0, -9.81, 0.0);                      
                                                                                    
        if (type0[0] == PARTICLE_TYPE_SHELL)  
        {                                     
            if (age < u_shellLifeTime) 
            {                                             
                type1 = PARTICLE_TYPE_SHELL;                                        
                position1 = position0[0] + DeltaP;                                  
                velocity1 = velocity0[0] + DeltaV;                                  
                age1 = age;                                                         
                EmitVertex();                                                       
                EndPrimitive();                                                     
            }                                                                       
            else 
            {    
                                              
                for (int i = 0 ; i < 10 ; i++) 
                {                                    
                     type1 = PARTICLE_TYPE_SECONDARY_SHELL;                         
                     position1 = position0[0];                                      
                     vec3 Dir = GetRandomDir((u_time + i)/1000.0);                   
                     velocity1 = normalize(Dir) / 20.0;                             
                     age1 = 0.0f;                                                   
                     EmitVertex();                                                  
                     EndPrimitive();                                                
                } 
                                                                                 
            }                                                                       
        }                                                                           
        else 
        {                                                                      
            if (age < u_secondaryShellLifeTime) 
            {                                    
                type1 = PARTICLE_TYPE_SECONDARY_SHELL;                              
                position1 = position0[0] + DeltaP;                                  
                velocity1 = velocity0[0] + DeltaV;                                  
                age1 = age;                                                         
                EmitVertex();                                                       
                EndPrimitive();                                                     
            }                                                                       
        }                                                                           
    }                                                                               
}                                                                                   
