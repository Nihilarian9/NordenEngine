/*
Ogygia
in Norden Engine v0.1a
by Peter Martin
*/

/*	TODO

-	create vertex.y heightmap for better accuracy than pixel heightmap.
-	improve wall implementation.
=		create all wall heights before building walls.
=		find mean wall height and use as basis for further calculations
=		don't create walls above 3x mean wall height regardless of terrain
=		for walls lower than mean wall height build walls all the way down to ground height but implement floor at mean wall height

-	add boat from Valhalla
-	implement chariot on land
-	make fort.placeFort functional
-	more efficient forest implementation
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "camera.h"
#include "treeModel.h"
#include "terrain.h"
#include "forest.h"
#include "weather.h"
#include "model.h"
#include "menu.h"
#include "fort.h"

#include <iostream>
#include <time.h>

//	Function precursors
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(Shader shader, char* file, char* imgName, unsigned int texNum);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 800;

bool landGame = true;

// camera
Camera camera(glm::vec3(1270.0f, 5.0f, 966.0f));
//Camera camera(glm::vec3(0.0f, 5.0f, -40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
char* fileName; char* shaderName;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//	effects
bool fogBool = false;
bool keyPressedF = false;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__	// fix compilation on OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ogygia", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture  mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//load shaders
	Shader menuShader("Graphics/Shaders/menuShader.vert", "Graphics/Shaders/menuShader.frag");
	Shader landShader("Graphics/Shaders/landShader.vert", "Graphics/Shaders/landShader.frag");
	Shader treeShader("Graphics/Shaders/tree.vert", "Graphics/Shaders/tree.frag");
	Shader sunShader("Graphics/Shaders/sunShader.vert", "Graphics/Shaders/sunShader.frag");
	Shader wallShader("Graphics/Shaders/wallShader.vert", "Graphics/Shaders/wallShader.frag");

	////////////////
	//	MENU LOADING
	Menu menu;
	unsigned int menuTexture;
	fileName = "Graphics/Images/norden.png";
	shaderName = "menuTexture";
	menuTexture = loadTexture(menuShader, fileName, shaderName, 0);
	menu.loading(window, menuShader);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Ground Mesh setup //	Real deal starts here
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	Terrain map;
	map.createHeightMap(); // don't foget to manually update height, width, and nrchannels for array within class
	map.createLandscape();
	// ground textures //
	fileName = "Graphics/Images/grass.png";
	shaderName = "grassTexture";
	unsigned int grassTexture = loadTexture(landShader, fileName, shaderName, 0);

	fileName = "Graphics/Images/stone.png";
	shaderName = "stoneTexture";
	unsigned int stoneTexture = loadTexture(landShader, fileName, shaderName, 1);
	unsigned int wallTexture = loadTexture(wallShader, fileName, shaderName, 0);

	fileName = "Graphics/Images/water.png";
	shaderName = "waterTexture";
	unsigned int waterTexture = loadTexture(landShader, fileName, shaderName, 2);

	//////////////
	//	Forest	//
	//	only the current tile needs to be checked for water/object collision
	//	later to create collision tile calculation to determine which parts of locate instances count
	Forest forest(&map);
	forest.createForestMap();
	forest.createTreeVectors();

	treeShader.use();
	for (unsigned int i = 0; i < forest.forestMapSize; i++)
	{
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		treeShader.setVec3(("offsets[" + index + "]").c_str(), forest.treePos[i]);
	}


	//////////////////////////////
	//	INITIALIZE FORT CLASS	//
	//////////////////////////////

	Fort fort(&map);

//	fort.buildFort();
	//std::cin.get();


	//////////////
	//	MODELS	//
	//////////////
	TreeModel tree("Graphics/models/tree/oak1.obj");
	Model sun("Graphics/models/sphere.obj");

	// pause menu
	fileName = "Graphics/Images/menu0.png";
	shaderName = "menuTexture";
	menuTexture = loadTexture(menuShader, fileName, shaderName, 0);
	menu.pause(window, menuShader);

	float frameTime = 0;
	float setFrame = 1.0 / 60; // divide by desired fps
	int frameCount = 0;
	int secondCount = int(glfwGetTime());
	float renderDistance = 1000.0f;

	Weather weather;

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// frameManagement
		// --------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		frameTime += deltaTime;
		if (secondCount < currentFrame + 1)
		{
			//	print out frames per second
			cout << frameCount << endl;
			frameCount = 0;
			secondCount++;
			cout << "playerRotation: " << camera.Yaw << endl;
			cout << "playerpos.x: " << camera.Position.x << "\nplayerpos.y: " << camera.Position.y << "\nplayerpos.z: " << camera.Position.z << endl;
		}
		processInput(window);
		weather.updateWeather(currentFrame, camera.Position, renderDistance);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, stoneTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterTexture);

		landShader.use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, renderDistance);
		landShader.setMat4("projection", projection);
		landShader.setVec3("sunPos", weather.sunPos);
		landShader.setFloat("frame", currentFrame);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		landShader.setMat4("view", view);
		glm::mat4 model;	//end setup with model
		model = glm::mat4();
		landShader.setMat4("model", model);
		//landShader.setFloat("frame", currentFrame);

		//	call function to render local world
		map.worldPosition(camera.Position.x, camera.Position.z);

		//render trees
		//	NEEDS TO BE UPGRADED TO RENDER LOCAL ONLY
		treeShader.use();
		treeShader.setMat4("projection", projection);
		treeShader.setVec3("sunPos", weather.sunPos);
		treeShader.setMat4("view", view);
		model = glm::mat4();
		treeShader.setMat4("model", model);

		//	instance render. see treeMesh.h
		tree.Draw(treeShader, forest.forestMapSize);

		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		sunShader.setVec3("sunPos", weather.sunPos);
		model = glm::translate(model, weather.sunPos);
		sunShader.setMat4("model", model);
		sun.Draw(sunShader);

		//wallShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, wallTexture);
		//model = glm::mat4();
		//wallShader.setMat4("model", model);


		model = glm::mat4();
		sunShader.setMat4("model", model);

		if (!fort.isFortPlaced)
			fort.placeFort(camera.Position.x, camera.Position.z);
		if (glfwGetKey(window, GLFW_KEY_Q))
			fort.isFortPlaced = false;

		fort.draw();


		//cout << camera.Position.x << " " << camera.Position.z << endl;
		//closing the game loop
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // end game loop
	map.deleteVAOs();
	fort.deleteVAO();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(RUN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		camera.ProcessKeyboard(JUMP, deltaTime);


}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(Shader shader, char* file, char* imgName, unsigned int texNum)
{
	unsigned int thisTexture;
	glGenTextures(1, &thisTexture);
	glBindTexture(GL_TEXTURE_2D, thisTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	shader.use();
	shader.setInt(imgName, texNum);

	return thisTexture;
}
