// type of wall
// holds array of walls in circle


//improve wall implementation.
//	create all wall heights before building walls.
//	find mean wall height and use as basis for further calculations
//	don't create walls above 3x mean wall height regardless of terrain
//	for walls lower than mean wall height build walls all the way down to ground height but implement floor at mean wall height

#include "wall.h"
#define MATH_PI 3.14159265358979323846

class Fort
{
public:
	
	float fortRadius; //the crux
	float wallTan;

	float numWalls = 32.0f; // arbitrary. determines size of fort by allocating amount of tangents it is to be made from
	float wallAngleIncrement = 360.0f / numWalls;
	float wallLength = 1.0f; // from wall class vector construction
	float wallAngle;
	glm::vec3 fortPos = glm::vec3(1270.0f, 0.0f, 966.0f);
	
	vector<Wall> fort;
	Terrain *terrain;
	Wall wall;
	unsigned int VAO;
	float fortArraySize = wall.numVerts * numWalls;
	bool isFortPlaced = false;
	
		
	Fort(Terrain *map)
	{
		terrain = map;
	}

	void placeFort(float fortX, float fortZ)
	{
		glm::vec3 wallPos;
		float *mesh = new float[fortArraySize];
		//	when adding arrays don't forget to delete at end of function
		float *wallHeights = new float[(int)numWalls];
		float *wallPosX = new float[(int)numWalls];
		float *wallPosZ = new float[(int)numWalls];
		float *wallAngles = new float[(int)numWalls];
		
		wallTan = tan(glm::radians(wallAngleIncrement));
		fortRadius = wallLength / wallTan * 0.975f;

		//	first loop to get lay of the land for the wall.
		for (int a = 0; a < numWalls; a++)
		{
			wallAngles[a] = wallAngleIncrement * a;
			wallPosX[a] = cos(glm::radians(wallAngles[a])) * fortRadius + fortX;
			wallPosZ[a] = sin(glm::radians(wallAngles[a])) * fortRadius + fortZ;

			int yPos = (int)wallPosZ[a] * terrain->numPixelX + (int)wallPosX[a];
			wallHeights[a] = terrain->getHeight(yPos);
			
		}

		//	loop through again to build walls after additional calculations have been made			
		for (int a = 0; a < numWalls; a++)
		{
			wallPos = glm::vec3(wallPosX[a], wallHeights[a], wallPosZ[a]);
			//	process wall
			wall.buildFort(wallAngles[a], wallPos, a);
			for (int b = 0; b <= wall.numVerts; b++)
			{
				mesh[a * wall.numVerts + b] = wall.wallArray[b];
			}
		}
		VAO = moveToGPU(mesh);
		isFortPlaced = true;

		delete wallHeights;
		delete wallPosX;
		delete wallPosZ;
		delete wallAngles;
		//delete mesh;
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fortArraySize, &aMesh[0], GL_DYNAMIC_DRAW);
		//placeFort moveToGpu function needs to use GL_DYNAMIC_DRAW because it writes regularly.
		
	}
	/*
	void buildFort()
	{

		glm::vec3 wallPos;
		float *mesh = new float[fortArraySize];
		//	Use to move float mesh[] from stack to heap for memory permanancy once fort systems become more complex
		
		wallTan = tan(glm::radians(wallAngleIncrement));
		fortRadius = wallLength / wallTan * 0.975f;
		cout << "fortRadius: " << fortRadius << endl;
		
		for (int a = 0; a < numWalls; a++)
		{
			wallAngle = wallAngleIncrement * a;
			//cout << "wallAngleDegrees: " << wallAngle << endl;
			//cout << "wallAngleRadians: " << glm::radians(wallAngle) << endl;

			wallPos.x = cos(glm::radians(wallAngle)) * fortRadius + fortPos.x;
			wallPos.z = sin(glm::radians(wallAngle)) * fortRadius + fortPos.z;
			//retrieving height value from terrain
			int yPos = (int)wallPos.z * terrain->pixelWidth + (int)wallPos.x;
			wallPos.y = terrain->getHeight(yPos);

			//cout << "wallPos.x: " << wallPos.x << "\twallPos.y: " << wallPos.y << "\twallPos.z: " << wallPos.z << endl;
			//cout << "height at " << yPos << " = " << wallPos.y << endl;

			//	process wall
			wall.buildFort(wallAngle, wallPos, a);
			for (int b = 0; b <= wall.numVerts; b++)
			{
				mesh[a * wall.numVerts + b] = wall.wallArray[b];
				//cout << wall.wallArray[b] << endl;

			}
			//cout << "\n" << endl;
		}

		VAO = moveToGPU(mesh);
	}*/
	void draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, fortArraySize / 3);
		glBindVertexArray(0);
	}

	void deleteVAO()
	{
		glDeleteVertexArrays(1, &VAO);
	}

private:

	unsigned int moveToGPU(float aMesh[])
	{
		unsigned int VBO, tempVAO;
		glGenVertexArrays(1, &tempVAO);
		glBindVertexArray(tempVAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fortArraySize, &aMesh[0], GL_DYNAMIC_DRAW); //GL_STATIC_DRAW when placed GL_DYNAMIC_DRAW otherwise
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);
		//glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return tempVAO;
	}

};