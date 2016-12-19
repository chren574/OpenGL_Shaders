#version 330 core

in vec3 ourColor;

in vec3 positionColor; // Output a color to the fragment shader

out vec4 color;

void main()
{
	color = vec4(ourColor, 1.0f);
	//color = vec4(positionColor, 1.0f); // Test: Negative values is clamped to 0 => black
};