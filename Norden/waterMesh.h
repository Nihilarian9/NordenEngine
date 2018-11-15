///////////////////////
//		WATERMESH
//		class by hex
///////////////////////
//	TODO:
//	wind strength and direction affecting weather

#include <iostream>


class WaterMesh
{
public:
	const static int numVertsX = 50;
	const static int numVertsZ = 50;
	const static int numTexture = 12;
	const static int numNormals = 18;
	const static int numCoords = 18;
	const static int arrayModifier = numTexture + numNormals + numCoords;
	const static int arraySize = numVertsX * numVertsZ * arrayModifier;
	int coordsTotal = numVertsX * numVertsZ * numCoords;
	bool printWaveDetails = false;

	glm::vec3 a, b, c, d;
	glm::vec3 normal1, normal2;
	float xfloat, zfloat;
	float waterMesh[arraySize];
	float windStrength = 2.0f;
	float winDir = 360.0f;

	/////////////
	//declare vector counter
	int v00 = 0, v01 = 1, v02 = 2, n00 = 3, n01 = 4, n02 = 5, t00 = 6, t01 = 7,
		v10 = 8, v11 = 9, v12 = 10, n10 = 11, n11 = 12, n12 = 13, t10 = 14, t11 = 15,
		v20 = 16, v21 = 17, v22 = 18, n20 = 19, n21 = 20, n22 = 21, t20 = 22, t21 = 23,

		v30 = 24, v31 = 25, v32 = 26, n30 = 27, n31 = 28, n32 = 29, t30 = 30, t31 = 31,
		v40 = 32, v41 = 33, v42 = 34, n40 = 35, n41 = 36, n42 = 37, t40 = 38, t41 = 39,
		v50 = 40, v51 = 41, v52 = 42, n50 = 43, n51 = 44, n52 = 45, t50 = 46, t51 = 47;

	//	gibs arraySize to people that need it, like the main.cpp
	float getArraySize()
	{
		return arraySize;
	}

	////////////////////////////////
	//2d array for creation of vector coords and mesh.
	void waterMeshSetup()
	{
		for (int z = 0; z < numVertsZ; z++)
		{
			zfloat = float(z - numVertsZ / 2);			//convert loop int to float for coords

			for (int x = 0; x < numVertsX; x++)
			{				
				xfloat = float(x - numVertsX / 2);		//convert loop int to float for coords

				///////////////////////
				//	create vector coords
				a.x = xfloat - 1.0f;
				a.y = 0.0f;
				a.z = zfloat;

				b.x = xfloat;
				b.y = 0.0f;
				b.z = zfloat;

				c.x = xfloat - 1.0f;
				c.y = 0.0f;
				c.z = zfloat - 1.0f;

				d.x = xfloat;
				d.y = 0.0f;
				d.z = zfloat - 1.0f;

				//////////////////////
				//	Calculate Normals
				normal1 = findNormal(a, b, c);
				
				////////////////////////
				//	fill array with spacial and texture coords
				//	first triangle
				waterMesh[v00] = a.x;	waterMesh[v01] = a.y;	waterMesh[v02] = a.z;	waterMesh[n00] = normal1.x;	waterMesh[n01] = normal1.y;	waterMesh[n02] = normal1.z;	waterMesh[t00] = 0.0f;	waterMesh[t01] = 0.0f;
				waterMesh[v10] = b.x;	waterMesh[v11] = b.y;	waterMesh[v12] = b.z;	waterMesh[n10] = normal1.x;	waterMesh[n11] = normal1.y;	waterMesh[n12] = normal1.z; waterMesh[t10] = 0.0f;	waterMesh[t11] = 1.0f;
				waterMesh[v20] = c.x;	waterMesh[v21] = c.y;	waterMesh[v22] = c.z;	waterMesh[n20] = normal1.x;	waterMesh[n21] = normal1.y;	waterMesh[n22] = normal1.z; waterMesh[t20] = 1.0f;	waterMesh[t21] = 0.0f;
				//	second triangle
				waterMesh[v30] = b.x;	waterMesh[v31] = b.y;	waterMesh[v32] = b.z;	waterMesh[n30] = normal1.x;	waterMesh[n31] = normal1.y;	waterMesh[n32] = normal1.z; waterMesh[t30] = 0.0f;	waterMesh[t31] = 1.0f;
				waterMesh[v40] = c.x;	waterMesh[v41] = c.y;	waterMesh[v42] = c.z;	waterMesh[n40] = normal1.x;	waterMesh[n41] = normal1.y;	waterMesh[n42] = normal1.z; waterMesh[t40] = 1.0f;	waterMesh[t41] = 0.0f;
				waterMesh[v50] = d.x;	waterMesh[v51] = d.y;	waterMesh[v52] = d.z;	waterMesh[n50] = normal1.x;	waterMesh[n51] = normal1.y;	waterMesh[n52] = normal1.z; waterMesh[t50] = 1.0f;	waterMesh[t51] = 1.0f;

				//////////////////////
				//update arrayCounter
				v00 += arrayModifier, v01 += arrayModifier, v02 += arrayModifier, n00 += arrayModifier, n01 += arrayModifier, n02 += arrayModifier, t00 += arrayModifier, t01 += arrayModifier,
				v10 += arrayModifier, v11 += arrayModifier, v12 += arrayModifier, n10 += arrayModifier, n11 += arrayModifier, n12 += arrayModifier, t10 += arrayModifier, t11 += arrayModifier,
				v20 += arrayModifier, v21 += arrayModifier, v22 += arrayModifier, n20 += arrayModifier, n21 += arrayModifier, n22 += arrayModifier, t20 += arrayModifier, t21 += arrayModifier,

				v30 += arrayModifier, v31 += arrayModifier, v32 += arrayModifier, n30 += arrayModifier, n31 += arrayModifier, n32 += arrayModifier, t30 += arrayModifier, t31 += arrayModifier,
				v40 += arrayModifier, v41 += arrayModifier, v42 += arrayModifier, n40 += arrayModifier, n41 += arrayModifier, n42 += arrayModifier, t40 += arrayModifier, t41 += arrayModifier,
				v50 += arrayModifier, v51 += arrayModifier, v52 += arrayModifier, n50 += arrayModifier, n51 += arrayModifier, n52 += arrayModifier, t50 += arrayModifier, t51 += arrayModifier;
			}
		}
	}

	void waterMeshUpdate(float frame, float posX, float posZ)
	{
		float currentFrame = frame;
		float boatPosX = posX;
		float boatPosZ = posZ;
		float scaleMod = 20.0f;
		float texMod = 0.05f;
		bool firstX;

		winDir = limitToCircle(winDir);

		v00 = 0, v01 = 1, v02 = 2, n00 = 3, n01 = 4, n02 = 5, t00 = 6, t01 = 7,
		v10 = 8, v11 = 9, v12 = 10, n10 = 11, n11 = 12, n12 = 13, t10 = 14, t11 = 15,
		v20 = 16, v21 = 17, v22 = 18, n20 = 19, n21 = 20, n22 = 21, t20 = 22, t21 = 23,

		v30 = 24, v31 = 25, v32 = 26, n30 = 27, n31 = 28, n32 = 29, t30 = 30, t31 = 31,
		v40 = 32, v41 = 33, v42 = 34, n40 = 35, n41 = 36, n42 = 37, t40 = 38, t41 = 39,
		v50 = 40, v51 = 41, v52 = 42, n50 = 43, n51 = 44, n52 = 45, t50 = 46, t51 = 47;

		for (int z = 0; z < numVertsZ; z++)
		{
			zfloat = float(z - numVertsZ / 2) * scaleMod + boatPosZ;	//	convert loop int to float for coords
			firstX = true;
			for (int x = 0; x < numVertsX; x++)
			{
				///////////////////////
				//create vector coords
				xfloat = float(x - numVertsX / 2) * scaleMod + boatPosX;	//	convert loop int to float for coords
				// cos(winDir) gives result between -1 and 1
				// result * x - numVertsX / 2 + boatPosX?
				a.x = xfloat - scaleMod;
				a.z = zfloat;
				a.y = waveYcalc(a.x, a.z, currentFrame);
				//a.y = 0.0f;

				b.x = xfloat;
				b.z = zfloat;
				b.y = waveYcalc(b.x, b.z, currentFrame);
				//b.y = 0.0f;

				c.x = xfloat - scaleMod;
				c.z = zfloat - scaleMod;
				c.y = waveYcalc(c.x, c.z, currentFrame);
				//c.y = 0.0f;

				d.x = xfloat;
				d.z = zfloat - scaleMod;
				d.y = waveYcalc(d.x, d.z, currentFrame);
				//d.y = 0.0f;

				/////////////////////
				//	Calculate Normals
				if (firstX)
				{
					normal1 = findNormal(a, b, c);
					firstX = false;
				}
				
				///////////////////
				//	fill array with spacial and texture coords
								
				//	first triangle
				waterMesh[v00] = a.x;	waterMesh[v01] = a.y;	waterMesh[v02] = a.z;	waterMesh[n00] = normal1.x;	waterMesh[n01] = normal1.y;	waterMesh[n02] = normal1.z;	waterMesh[t00] = 0.0f - (boatPosZ * texMod);	waterMesh[t01] = 0.0f + (boatPosX * texMod);
				waterMesh[v10] = b.x;	waterMesh[v11] = b.y;	waterMesh[v12] = b.z;	waterMesh[n10] = normal1.x;	waterMesh[n11] = normal1.y;	waterMesh[n12] = normal1.z; waterMesh[t10] = 0.0f - (boatPosZ * texMod);	waterMesh[t11] = 1.0f + (boatPosX * texMod);
				waterMesh[v20] = c.x;	waterMesh[v21] = c.y;	waterMesh[v22] = c.z;	waterMesh[n20] = normal1.x;	waterMesh[n21] = normal1.y;	waterMesh[n22] = normal1.z; waterMesh[t20] = 1.0f - (boatPosZ * texMod);	waterMesh[t21] = 0.0f + (boatPosX * texMod);

				//	second triangle
				waterMesh[v30] = b.x;	waterMesh[v31] = b.y;	waterMesh[v32] = b.z;	waterMesh[n30] = normal1.x;	waterMesh[n31] = normal1.y;	waterMesh[n32] = normal1.z; waterMesh[t30] = 0.0f - (boatPosZ * texMod);	waterMesh[t31] = 1.0f + (boatPosX * texMod);
				waterMesh[v40] = c.x;	waterMesh[v41] = c.y;	waterMesh[v42] = c.z;	waterMesh[n40] = normal1.x;	waterMesh[n41] = normal1.y;	waterMesh[n42] = normal1.z; waterMesh[t40] = 1.0f - (boatPosZ * texMod);	waterMesh[t41] = 0.0f + (boatPosX * texMod);
				waterMesh[v50] = d.x;	waterMesh[v51] = d.y;	waterMesh[v52] = d.z;	waterMesh[n50] = normal1.x;	waterMesh[n51] = normal1.y;	waterMesh[n52] = normal1.z; waterMesh[t50] = 1.0f - (boatPosZ * texMod);	waterMesh[t51] = 1.0f + (boatPosX * texMod);

				/////////////////////
				//update arrayCounter
				v00 += arrayModifier, v01 += arrayModifier, v02 += arrayModifier, n00 += arrayModifier, n01 += arrayModifier, n02 += arrayModifier, t00 += arrayModifier, t01 += arrayModifier,
				v10 += arrayModifier, v11 += arrayModifier, v12 += arrayModifier, n10 += arrayModifier, n11 += arrayModifier, n12 += arrayModifier, t10 += arrayModifier, t11 += arrayModifier,
				v20 += arrayModifier, v21 += arrayModifier, v22 += arrayModifier, n20 += arrayModifier, n21 += arrayModifier, n22 += arrayModifier, t20 += arrayModifier, t21 += arrayModifier,

				v30 += arrayModifier, v31 += arrayModifier, v32 += arrayModifier, n30 += arrayModifier, n31 += arrayModifier, n32 += arrayModifier, t30 += arrayModifier, t31 += arrayModifier,
				v40 += arrayModifier, v41 += arrayModifier, v42 += arrayModifier, n40 += arrayModifier, n41 += arrayModifier, n42 += arrayModifier, t40 += arrayModifier, t41 += arrayModifier,
				v50 += arrayModifier, v51 += arrayModifier, v52 += arrayModifier, n50 += arrayModifier, n51 += arrayModifier, n52 += arrayModifier, t50 += arrayModifier, t51 += arrayModifier;


				/* stationary watermesh y coord only update.
				///////////////////
				// efficient updates if no mesh relocation
				// use when player based on land.
				//v01 = 1, v11 = 6, v21 = 11, v31 = 16, v41 = 21, v51 = 26;
				//	update Y coords in array
				waterMesh[v01] = a.y;	waterMesh[v11] = b.y;	waterMesh[v21] = c.y;
				waterMesh[v31] = b.y;	waterMesh[v41] = c.y;	waterMesh[v51] = d.y;

				/////////////////////
				//	update arrayCounter
				v01 += arrayModifier, v11 += arrayModifier, v21 += arrayModifier,
				v31 += arrayModifier, v41 += arrayModifier, v51 += arrayModifier;
				*/
			}
		}
	}

	float waveYcalc(float inputX, float inputZ, float currentFrameF)
	{
		float currentFrame = currentFrameF; // current frame determines wave speed
		float waveHeight = windStrength;
		float waveLength = 0.01f * waveHeight;
		float waveSpeed = windStrength / 2;
		float x = inputX * cos(glm::radians(winDir));
		float z = inputZ * sin(glm::radians(winDir));
		float y = waveHeight * sin((x + z) * waveLength + (currentFrame / waveSpeed));
		return y;
	}

	glm::vec3 findNormal(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc)
	{
		//	something is wrong with these calculations
		glm::vec3 normal;
		glm::vec3 p1 = pointa;
		glm::vec3 p2 = pointb;
		glm::vec3 p3 = pointc;
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;

		normal.x = glm::normalize((U.y * V.z) - (U.z * V.y));
		normal.y = glm::normalize((U.z * V.x) - (U.x * V.z));
		normal.z = glm::normalize((U.x * V.y) - (U.y * V.x));

		return normal;
	}
	
	float limitToCircle(float degree)
	{
		float degrees = degree;
		if (degrees > 360.9f)
		{
			degrees = 1.0f;
		}

		if (degrees < 0.1f)
		{
			degrees = 360.0f;
		}
		return degrees;
	}

};