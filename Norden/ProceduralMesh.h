//#include <string>
//#include <cmath>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ProceduralMesh
{
public:
	
	//Function to create 2d array for ground mesh
	std::vector<glm::vec3> meshCreate(int sizeOfMeshX, int sizeOfMeshZ)
	{
		glm::vec3 a, b, c, d;
		//it's a vector of vec3's called vertices
		std::vector<glm::vec3> vertices;

		int numVertsX = sizeOfMeshX, numVertsZ = sizeOfMeshZ;
		float xfloat, zfloat;
		//glm::vec3 normal1, normal2;

		for (int z = 0; z < numVertsZ; z++)
		{
			zfloat = float(z);
			
			for (int x = 0; x < numVertsX; x++)
			{
				xfloat = float(x);

				a.x = xfloat - 1.0f; a.y = zfloat;
				b.x = xfloat; b.z = zfloat;
				c.x = xfloat - 1.0f; c.z = zfloat - 1.0f;
				d.x = xfloat; d.z = zfloat - 1.0f;

				a.y = 0.0f;
				b.y = 0.0f;
				c.y = 0.0f;
				d.y = 0.0f;

				//std::cout << a.x << a.z << b.x << b.z << c.x << c.z << std::endl;
				//std::cout << b.x << b.z << c.x << c.z << d.x << d.z << std::endl;
				// add vec3 to vector
				vertices.push_back(a); vertices.push_back(b); vertices.push_back(c);
				vertices.push_back(b); vertices.push_back(c); vertices.push_back(d);


				/*
				// Calculate Normals
				normal1 = findNormal(a, b, c);
				normal2 = findNormal(d, c, b);

				// add vec3 to vector
				vertices.push_back(a); vertices.push_back(normal1);
				vertices.push_back(b); vertices.push_back(normal1);
				vertices.push_back(c); vertices.push_back(normal1);

				vertices.push_back(b); vertices.push_back(normal2);
				vertices.push_back(c); vertices.push_back(normal2);
				vertices.push_back(d); vertices.push_back(normal2);
				*/

			}
		}

		return vertices;
	}

	
		// Function revices vectors for triangle and returns a normal vertor for lighting.
	glm::vec3 findNormal(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc)
	{
		glm::vec3 normal;
		glm::vec3 pA = pointa;
		glm::vec3 pB = pointb;
		glm::vec3 pC = pointc;
		glm::vec3 U = pB - pA;
		glm::vec3 V = pC - pA;

		normal.x = glm::normalize((U.y * V.z) - (U.z * V.y));
		normal.y = glm::normalize((U.z * V.x) - (U.x * V.z));
		normal.z = glm::normalize((U.x * V.y) - (U.y * V.x));

		return normal;
	}
	

};