#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aSlope;

out float yModWater;
out float Slope;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
smooth out vec4 ViewMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float frame;

void main()
{	

//	if (aPos.y <= 0.0)
	//{
	//	yModWater = 0.0;
	//	FragPos = vec3(model * vec4(aPos.x, sin(aPos.x * aPos.z), aPos.z, 1.0));
	//} else {
	//	yModWater = 1.0;
	//	FragPos = vec3(model * vec4(aPos, 1.0));
	//}
	  
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
    TexCoord = aTexCoord;
	Slope = aSlope;
	ViewMat = view * model * vec4(aPos, 1.0f);

	if (aPos.y <= 0.0)
	{// water effects
		yModWater = 0.0;
		gl_Position = projection * view * model * vec4(aPos.x, 0.1 * sin(aPos.x + aPos.z + frame), aPos.z, 1.0);
	} else {//plane land
		yModWater = 1.0;
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
//	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

