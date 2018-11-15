#pragma once


class Weather
{
public:
	//	sun
	glm::vec3 sunPos = glm::vec3(0.0f);
	glm::vec3 sunColor = glm::vec3(0.5);

	//	clouds
	float cloudSizeMod = 1.0f;
	float cloudVecs[30] = {				//and texture coords
		-cloudSizeMod, -cloudSizeMod, 0.0f, 0.0f, 0.0f,
		-cloudSizeMod, cloudSizeMod, 0.0f, 0.0f, 1.0f,
		cloudSizeMod, -cloudSizeMod, 0.0f, 1.0f, 0.0f,
		cloudSizeMod, cloudSizeMod, 0.0f, 1.0f, 1.0f,
		-cloudSizeMod, cloudSizeMod, 0.0f, 0.0f, 1.0f,
		cloudSizeMod, -cloudSizeMod, 0.0f, 1.0f, 0.0f
	};


	void drawWeather()
	{

	}

	void updateWeather(float frame, glm::vec3 playerPos, float sunRadius)
	{
		/////////////
		//	SUN
		float sunSpeed = 0.1f;
		float sunMoveY = sin(frame * sunSpeed) * sunRadius;
		float sunMoveX = cos(frame * sunSpeed) * sunRadius;
		sunPos = glm::vec3(playerPos.x - sunMoveX, sunMoveY, playerPos.z); 

		///////////////
		//	CLOUDS


		//////////////
		//	SKY
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	}
};