#version 440

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;
layout (location = 4) in vec3 StartPosition;

out vec3 Position;   
out vec3 Velocity;  
out float StartTime; 
out float Transp;    
out float colour;

uniform float Time;  
uniform float DT;     
uniform vec3 Accel; 
uniform float ParticleLifetime; 

uniform mat4 MVP;

subroutine (RenderPassType)
void update() {

    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;

    if( Time >= StartTime ) {

        float age = Time - StartTime;
        if( age > ParticleLifetime ) {
            // If the particle is past its lifetime reset it to its start position
            Position = StartPosition;
           Velocity = VertexInitialVelocity;
            StartTime = Time;
        } else {
            //Update using the Euler equation
            Position += Velocity * DT;
            Velocity += Accel * DT;
        }
    }
}

subroutine (RenderPassType)
void render() {
    float age = Time - VertexStartTime;
    Transp = ParticleLifetime - age;
	colour = 0.5 + age;// / 10;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

void main()
{
    //render or update
    RenderPass();
}
