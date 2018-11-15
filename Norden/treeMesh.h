#ifndef TREEMESH_H
#define TREEMESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct TreeVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct TreeTexture {
	unsigned int id;
	string type;
	aiString path;
};

class TreeMesh {
public:
	/* Mesh Data */
	vector<TreeVertex> vertices;
	vector<unsigned int> indices;
	vector<TreeTexture> textures;
	unsigned int treeVAO;

	/* Functions */
	TreeMesh(vector<TreeVertex> vertices, vector<unsigned int> indices, vector<TreeTexture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void Draw(Shader shader, int forestSize)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
											  // retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// draw mesh
		//	this is where you need to change the render to instance
		glBindVertexArray(treeVAO);

		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, forestSize);
		
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/* Render Data */
	unsigned int treeVBO, treeEBO;

	/* Functions */
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &treeVAO);
		glGenBuffers(1, &treeVBO);
		glGenBuffers(1, &treeEBO);

		glBindVertexArray(treeVAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
		
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TreeVertex), &vertices[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, treeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TreeVertex), (void*)offsetof(TreeVertex, Bitangent));

		glBindVertexArray(0);
		cout << "Mesh processed" << endl;

	}

};
#endif