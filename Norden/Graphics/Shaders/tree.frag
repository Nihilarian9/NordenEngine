#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D treeTexture;

uniform vec3 sunPos;
vec4 treeColor;

void main()
{
	// with normals
	vec3 norm = normalize(Normal);

	if (texture(treeTexture, TexCoord).a != 1.0)
	{
		discard;
	} else {
		treeColor = texture(treeTexture, TexCoord);
	}

	


	//////////////
	//	lighting
	// ambient
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); // light colour
  	
	// diffuse 
	vec3 lightDir = normalize(sunPos - FragPos);
	float diff = max(dot(norm, -lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0); // light colour

	vec3 result = vec3(ambient);// + diffuse);

	FragColor = vec4(result, 1.0) * treeColor;
    //FragColor = texture(treeTexture, TexCoord);
}