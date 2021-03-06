//#pragma once

//////////////////
// Take picture	//
// Make terrain	//
//////////////////

/*	maybe this as solution
int size = 0;
for (int i = 0; i < pixels.size(); i++)
{
if (pixels == blue)
{
size++;
}
}

int blueCount = 0;
int** bluePixels;
bluePixels = new int*[size];
for (int i = 0; i < pixels.size(); i++)
{
if (pixels[i] == blue)
{
bluePixels[blueCount] = i;
blueCount++;
}
}*/

// Takes heightmap and turns into fully instances terrain

#include <vector>

class heightMap
{

public:

	char *filename = "Graphics/Images/ireland-2048.png";

	//	manual entry required for height and length of the image
	const static int pixelWidth = 2048;
	const static int pixelLength = 2048;

	//	numInst defines how many pieces to split the map into
	//	pixel array must be divisible by numInst without remainder.
	const static int numInstX = 16;
	const static int numInstZ = 16;
	const static int numInst = numInstX * numInstZ;

	glm::vec3 value;
	int width, length, nrChannels;


	int meshSect = 9; // num of elements per vertex
	int numSlope = 6; // 6 slope elements per iteration
	int numTexture = 12; // per iteration
	int numNormals = 18; // per iteration
	int numCoords = 18; // per iteration

	int arrayModifier = numTexture + numNormals + numCoords + numSlope; // total floats in each iteration
	int arraySize = pixelWidth * pixelLength * arrayModifier; //	total iterations * elements of each iteration
	int instArraySize = arraySize / numInst;
	int instPixelsX = pixelWidth / numInstX;
	int instPixelsZ = pixelLength / numInstZ;
	int instPixelsTot = numInstX * numInstZ;

	glm::vec3 heightmap3[pixelWidth * pixelLength];
	glm::vec4 heightmap4[pixelWidth * pixelLength];
	bool heightmapvec3 = false;

	float theHeightMap[pixelWidth * pixelLength];

	//	waterMapSize needs to be updated with map changes. Find number by checking Counter variable after full run through.
	const static int waterMapSize = 2531497;
	//int waterMap[waterMapSize];
	int waterMapCounter = 0;

	unsigned int VAO[numInst];

	void createLandscape()
	{

		if (arraySize % numInst != 0)
		{
			cout << "ERROR: arraySize must be divisible by numInst. Array error incoming." << endl;
		}

		//////////////////////////
		//	Mesh Array setup	//
		//////////////////////////
		float **mesh;
		mesh = new float*[numInst];
		int mapCounter[numInst];
		for (int i = 0; i < numInst; i++)
		{
			mesh[i] = new float[instArraySize];
			mapCounter[i] = 0;
		}
				
		// declare before loops
		glm::vec3 a, b, c, d;
		int pixelCount = 0;
		int instX, instZ, instPos1D;
		int meshMod;
		float center, zNeg, zPos, xNeg, xPos, xNzP, xPzP, xNzN, xPzN;
		for (int z = 0; z < length; z++)
		{
			float zfloat = float(z);

			for (int x = 0; x < width; x++)
			{
				float xfloat = float(x);

				////////////////////////
				//	ELEVATION
				//	get elevations for current and surrounding ti les.
				//	when facing 0degrees array prints from bottom left to top right
				center = getElevation(pixelCount);

				xNeg = getElevation(pixelCount - 1);
				xPos = getElevation(pixelCount + 1);
				zNeg = getElevation(pixelCount - length);
				zPos = getElevation(pixelCount + length);

				xPzN = getElevation(pixelCount - length + 1);
				xPzP = getElevation(pixelCount + length + 1);
				xNzN = getElevation(pixelCount - length - 1);
				xNzP = getElevation(pixelCount + length - 1);

				a.x = xfloat;
				a.y = mergeElevations(center, xNeg, zPos, xNzP);
				a.z = zfloat + 1.0f;

				b.x = xfloat + 1.0f;
				b.y = mergeElevations(center, xPos, zPos, xPzP);
				b.z = zfloat + 1.0f;

				c.x = xfloat;
				c.y = mergeElevations(center, xNeg, zNeg, xNzN);
				c.z = zfloat;

				d.x = xfloat + 1.0f;
				d.y = mergeElevations(center, xPos, zNeg, xPzN);
				d.z = zfloat;

				//////////////////
				//	FILL ARRAYS
				//	setup variables for iteration management
				instX = (pixelCount - pixelWidth * z) / instPixelsX;
				instZ = ((pixelCount - x) / pixelWidth) / instPixelsZ;
				instPos1D = instZ * numInstX + instX;

				meshMod = mapCounter[instPos1D] * arrayModifier;
				meshFill(mesh[instPos1D], meshMod, a, b, c, d);
				mapCounter[instPos1D]++;
				//check if current tile is water and if so add to water array
				if (center == 0.0f)
				{
					//waterMap[waterMapCounter] = pixelCount;
					waterMapCounter++;
				//	if (waterMapCounter > waterMapSize)
					//	cout << "You need to adjust the waterMapSize in heightMap.h" << endl;
				}

				pixelCount++;
			}// close z
		}// close x
		cout << "Number of water pixels is: " << waterMapCounter << endl;

		 //fill VAOs and release RAM used by mesh arrays
		for (int i = 0; i < numInst; i++)
		{
			VAO[i] = makeVAO(mesh[i]);
			delete mesh[i];
		}
		delete mesh;
	}
	
	void meshFill(float Mesh[], int MeshMod, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D)
	{
		glm::vec3 a = A;
		glm::vec3 b = B;
		glm::vec3 c = C;
		glm::vec3 d = D;
		int meshMod = MeshMod;
		float* thisMesh = Mesh;
		//////////////////////
		//	Calculate Normals

		glm::vec3 normal1 = glm::normalize(glm::cross(c - a, b - a));
		glm::vec3 normal2 = glm::normalize(glm::cross(d - b, d - c));
		glm::vec3 ground1 = glm::vec3(normal1.x, 0.0f, normal1.z);
		glm::vec3 ground2 = glm::vec3(normal2.x, 0.0f, normal2.z);
		float slope1 = glm::dot(normal1, ground1);
		float slope2 = glm::dot(normal2, ground2);

		//	first triangle
		thisMesh[meshMod] = a.x;	thisMesh[meshMod + 1] = a.y;	thisMesh[meshMod + 2] = a.z;
		thisMesh[meshMod + 3] = normal1.x;	thisMesh[meshMod + 4] = normal1.y;	thisMesh[meshMod + 5] = normal1.z;
		thisMesh[meshMod + 6] = 0.0f;	thisMesh[meshMod + 7] = 0.0f;
		thisMesh[meshMod + 8] = slope1;

		thisMesh[meshMod + meshSect] = b.x;	thisMesh[meshMod + meshSect + 1] = b.y;	thisMesh[meshMod + meshSect + 2] = b.z;
		thisMesh[meshMod + meshSect + 3] = normal1.x;	thisMesh[meshMod + meshSect + 4] = normal1.y;	thisMesh[meshMod + meshSect + 5] = normal1.z;
		thisMesh[meshMod + meshSect + 6] = 0.0f;	thisMesh[meshMod + meshSect + 7] = 1.0f;
		thisMesh[meshMod + meshSect + 8] = slope1;

		thisMesh[meshMod + meshSect * 2] = c.x;	thisMesh[meshMod + meshSect * 2 + 1] = c.y;	thisMesh[meshMod + meshSect * 2 + 2] = c.z;
		thisMesh[meshMod + meshSect * 2 + 3] = normal1.x;	thisMesh[meshMod + meshSect * 2 + 4] = normal1.y;	thisMesh[meshMod + meshSect * 2 + 5] = normal1.z;
		thisMesh[meshMod + meshSect * 2 + 6] = 1.0f;	thisMesh[meshMod + meshSect * 2 + 7] = 0.0f;
		thisMesh[meshMod + meshSect * 2 + 8] = slope1;

		//	second triangle
		thisMesh[meshMod + meshSect * 3] = b.x;	thisMesh[meshMod + meshSect * 3 + 1] = b.y;	thisMesh[meshMod + meshSect * 3 + 2] = b.z;
		thisMesh[meshMod + meshSect * 3 + 3] = normal2.x;	thisMesh[meshMod + meshSect * 3 + 4] = normal2.y;	thisMesh[meshMod + meshSect * 3 + 5] = normal2.z;
		thisMesh[meshMod + meshSect * 3 + 6] = 0.0f;	thisMesh[meshMod + meshSect * 3 + 7] = 1.0f;
		thisMesh[meshMod + meshSect * 3 + 8] = slope2;

		thisMesh[meshMod + meshSect * 4] = c.x;	thisMesh[meshMod + meshSect * 4 + 1] = c.y;	thisMesh[meshMod + meshSect * 4 + 2] = c.z;
		thisMesh[meshMod + meshSect * 4 + 3] = normal2.x;	thisMesh[meshMod + meshSect * 4 + 4] = normal2.y;	thisMesh[meshMod + meshSect * 4 + 5] = normal2.z;
		thisMesh[meshMod + meshSect * 4 + 6] = 1.0f;	thisMesh[meshMod + meshSect * 4 + 7] = 0.0f;
		thisMesh[meshMod + meshSect * 4 + 8] = slope2;

		thisMesh[meshMod + meshSect * 5] = d.x;	thisMesh[meshMod + meshSect * 5 + 1] = d.y;	thisMesh[meshMod + meshSect * 5 + 2] = d.z;
		thisMesh[meshMod + meshSect * 5 + 3] = normal2.x;	thisMesh[meshMod + meshSect * 5 + 4] = normal2.y;	thisMesh[meshMod + meshSect * 5 + 5] = normal2.z;
		thisMesh[meshMod + meshSect * 5 + 6] = 1.0f;	thisMesh[meshMod + meshSect * 5 + 7] = 1.0f;
		thisMesh[meshMod + meshSect * 5 + 8] = slope2;
	}

	void deleteVAOs()
	{
		for (int i = 0; i < numInst; i++)
		{
			glDeleteVertexArrays(1, &VAO[i]);
		}
	}

	float getElevation(int count)
	{
		float heightValue = 0.0f;
		if (count < 0 || count > width * length - 1)
		{
			return heightValue;
		}
		else
		{
			if (heightmapvec3)
			{
				heightValue = (heightmap3[count].x + heightmap3[count].y + heightmap3[count].z) / 40;
				return heightValue;
			}
			else
			{
				if (heightmap4[count].a == 0)
				{
					return heightValue;
				}
				else
				{
					heightValue = (heightmap4[count].x + heightmap4[count].y + heightmap4[count].z) / 40;
					return heightValue;
				}
			}	
		}
	}

	float mergeElevations(float current, float h1, float h2, float h3)
	{
		int divCount = 0;
		if (current != 0.0f)
			divCount++;
		if (h1 != 0.0f)
			divCount++;
		if (h2 != 0.0f)
			divCount++;
		if (h3 != 0.0f)
			divCount++;
		//	return 0 or merged results
		if (divCount == 0)
			return 0.0f;
		else
			return (current + h1 + h2 + h3) / divCount;
	}

	unsigned int makeVAO(float Mesh[])
	{
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instArraySize, &Mesh[0], GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, meshSect * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshSect * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, meshSect * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, meshSect * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return VAO;
	}

	//	function for choosing tiles to render given player's position.
	void worldPosition(float playerPosX, float playerPosZ)
	{
		int instX = playerPosX / instPixelsX;
		int instZ = playerPosZ / instPixelsZ;
		int instPos1D = instZ * numInstX + instX;

		/*	array layout
		9	10	11	12	13
		14	1	2	3	15
		16	4	0	5	17
		18	6	7	8	19
		20	21	22	23	24
		*/

		//	setup array
		int arrayNum = 25;
		int* render;
		render = new int[arrayNum];

		//	add inner circle
		render[0] = instPos1D;
		render[1] = instPos1D - numInstX - 1;
		render[2] = instPos1D - numInstX;
		render[3] = instPos1D - numInstX + 1;

		render[4] = instPos1D - 1;
		render[5] = instPos1D + 1;

		render[6] = instPos1D + numInstX - 1;
		render[7] = instPos1D + numInstX;
		render[8] = instPos1D + numInstX + 1;

		//	finally add outter circle
		render[9] = instPos1D - 2 * numInstX - 2;
		render[10] = instPos1D - 2 * numInstX - 1;
		render[11] = instPos1D - 2 * numInstX;
		render[12] = instPos1D - 2 * numInstX + 1;
		render[13] = instPos1D - 2 * numInstX + 2;

		render[14] = instPos1D - numInstX - 2;
		render[15] = instPos1D - numInstX + 2;

		render[16] = instPos1D - 2;
		render[17] = instPos1D + 2;

		render[18] = instPos1D + numInstX - 2;
		render[19] = instPos1D + numInstX + 2;

		render[20] = instPos1D + 2 * numInstX - 2;
		render[21] = instPos1D + 2 * numInstX - 1;
		render[22] = instPos1D + 2 * numInstX;
		render[23] = instPos1D + 2 * numInstX + 1;
		render[24] = instPos1D + 2 * numInstX + 2;

		for (int i = 0; i < arrayNum; i++)
		{
			if (render[i] >= 0 && render[i] < numInst)
			{
				glBindVertexArray(VAO[render[i]]);
				glDrawArrays(GL_TRIANGLES, 0, instArraySize / meshSect);
			}
		}
		glBindVertexArray(0);

		delete render;
	}
	

	void createHeightMap()
	{
		//	don't forget to change width and length of array when changing image
		stbi_set_flip_vertically_on_load(true);
		unsigned char *imgdata = stbi_load(filename, &width, &length, &nrChannels, 0);

		if (nrChannels == 4)
		{
			for (int i = 0; i < width * length; i++)
			{
				// fill array with values
				heightmap4[i].x = (int)imgdata[i * 4];		//R
				heightmap4[i].y = (int)imgdata[i * 4 + 1];	//G
				heightmap4[i].z = (int)imgdata[i * 4 + 2];	//B
				heightmap4[i].a = (int)imgdata[i * 4 + 3];	//A
			}
		}
		else if (nrChannels == 3)
		{
			heightmapvec3 = true;
			for (int i = 0; i < width * length; i++)
			{
				// fill array with values
				heightmap3[i].x = (int)imgdata[i * 3];		//R
				heightmap3[i].y = (int)imgdata[i * 3 + 1];	//G
				heightmap3[i].z = (int)imgdata[i * 3 + 2];	//B
			}
		}
		else
		{
			cout << nrChannels << endl;
			cout << "error: function only suitable for 3 or 4 channel image" << endl;
		}
		stbi_image_free(imgdata);
	}

};//	close class