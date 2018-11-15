#pragma once
//#include "terrain.h"

class Forest
{
public:
	char *filename = "Graphics/Images/forest.png";

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

	int forestCount = 0;
	const static int forestMapSize = 123; // edit to match output of forestCount when map edited
										   // don't forget to change treeMesh.h render value and tree.vert
	//int forestMap[pixelWidth * pixelLength]; //	use to capture number of trees when map change occurs
	int forestMap[forestMapSize];
	
	Terrain *terrain;
	Forest(Terrain *map)
	{
		terrain = map;
	}
	//unsigned int VAO[numInst];

	glm::vec3 treePos[forestMapSize];
	//	convert pixel location to global x and z

	void createTreeVectors()
	{
		for (int i = 0; i < forestMapSize; i++)
		{
			int pixel = forestMap[i];
			int treeZ = pixel / pixelWidth;
			int treeX = pixel - treeZ * pixelWidth;
			treePos[i].x = treeX;
			treePos[i].z = treeZ;
			treePos[i].y = terrain->getHeight(forestMap[i]);

			//cout << treePos[i].x << "\t" << treePos[i].y << "\t" << treePos[i].z << endl;
		}
		cout << "forest data processed" << endl;
	}
	

	//	Rendering needs to be changed to instanced

	//treeShader.use();


	void createForestMap()
	{
		//	don't forget to change width and length of array when changing image
		stbi_set_flip_vertically_on_load(true);
		unsigned char *imgdata = stbi_load(filename, &width, &length, &nrChannels, 0);

		if (nrChannels == 4)
		{
			for (int i = 0; i < width * length; i++)
			{
				// fill array with values
				//(int)imgdata[i * 4];		//R
				//(int)imgdata[i * 4 + 1];	//G
				//(int)imgdata[i * 4 + 2];	//B
				//(int)imgdata[i * 4 + 3];	//A
				if ((int)imgdata[i * 4 + 1] >(int)imgdata[i * 4] && (int)imgdata[i * 4 + 1] > (int)imgdata[i * 4 + 2])
				{
					forestMap[forestCount] = i;
					forestCount++;
				}
			}
		}
		else if (nrChannels == 3)
		{
			for (int i = 0; i < width * length; i++)
			{
				// fill array with values
				//(int)imgdata[i * 3];		//R
				//(int)imgdata[i * 3 + 1];	//G
				//(int)imgdata[i * 3 + 2];	//B
				if ((int)imgdata[i * 3 + 1] >(int)imgdata[i * 3] && (int)imgdata[i * 3 + 1] > (int)imgdata[i * 3 + 2])
				{
					forestMap[forestCount] = i;
					forestCount++;
				}
			}
		}
		else
		{
			cout << nrChannels << endl;
			cout << "error: function only suitable for 3 or 4 channel image" << endl;
		}
		stbi_image_free(imgdata);
	}

};