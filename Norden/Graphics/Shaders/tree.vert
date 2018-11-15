#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offsets[123]; //	must be edited with Forest::forestMapSize;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal; 
	TexCoord = aTexCoord;
	vec3 offset = offsets[gl_InstanceID];
	gl_Position = projection * view * model * vec4(aPos + offset, 1.0);
}