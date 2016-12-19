#version 330 core

layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 color;	// The color variable has attribute position 1

out vec3 ourColor; // Output a color to the fragment shader
out vec3 positionColor; // Output a color to the fragment shader

uniform float Offset; // Global variable

void main()
{
	gl_Position = vec4(position, 1.0);
	//gl_Position = vec4(position.x, position.y + Offset, position.z, 1.0); // offset 
	//gl_Position = vec4(position.x, -position.y, position.z, 1.0); // upside down

	ourColor = color;
	positionColor = position;
};