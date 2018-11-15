#version 330 core
out vec4 FragColor;
  
in float yModWater;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in float Slope;
smooth in vec4 ViewMat;

uniform float frame;
uniform vec3 sunPos;
uniform vec3 sunColor;
uniform sampler2D grassTexture;
uniform sampler2D stoneTexture;
uniform sampler2D waterTexture;
uniform bool fogBool;
uniform vec4 fogColor;

vec4 groundColor;
//fog
float fStart = 5.0;
float fEnd = 200.0;

void main()
{

	// with normals
	vec3 norm = normalize(Normal);
		
	if (yModWater <= 0.0)
	{
		float waterTeX = 0.2 * sin(FragPos.x + FragPos.z + (frame / 2));
		groundColor = texture(waterTexture, vec2(waterTeX, TexCoord.y));
	} else {
		vec4 grassTex = texture(grassTexture, TexCoord) * vec4(0.6 - Slope);
		vec4 stoneTex = texture(stoneTexture, TexCoord) * vec4(Slope);
		groundColor = mix(grassTex, stoneTex, stoneTex.a);
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

	FragColor = vec4(result, 1.0) * groundColor;
	
//	if(fogBool)
	//{
		//float FogCoord = abs(ViewMat.z/ViewMat.w);
		//float FogFactor = (fEnd - FogCoord) / (fEnd - fStart);
		//FogFactor = 1.0-clamp(FogFactor, 0.0, 1.0);
		//FragColor = vec4(result, 1.0) * mix(texture(grassTexture, TexCoord), fogColor, FogFactor);
	//} else {
	//	FragColor = vec4(result, 1.0) * groundColor;
		//FragColor = vec4(result, 1.0) * texture(groundTexture0, TexCoord); // object colour
	//}


}

