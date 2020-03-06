#version 330

layout (location = 0) in vec3 Position;
out vec4 Color;

void main()
{
    Color = vec4(0.5, 0.5, clamp(Position.z,0.4,1.0), 1.0 );
    gl_Position = vec4( Position.x, Position.y, Position.z, 1.0);
}