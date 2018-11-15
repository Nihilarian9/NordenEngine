//////////////
//	MENU	//
//////////////

class Menu
{
public:

	///////////////////////////////////////////
	//	MENU setup
	float menuSizeMod = 1.0f;
	float menuVecs[18] = {
		-menuSizeMod, -menuSizeMod, 0.0f,// 0.0f, 0.0f,
		-menuSizeMod, menuSizeMod, 0.0f,// 0.0f, 1.0f,
		menuSizeMod, -menuSizeMod, 0.0f,// 1.0f, 0.0f,
		menuSizeMod, menuSizeMod, 0.0f,// 1.0f, 1.0f,
		-menuSizeMod, menuSizeMod, 0.0f,// 0.0f, 1.0f,
		menuSizeMod, -menuSizeMod, 0.0f//, 1.0f, 0.0f
	};
	float menuTex[12] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f
	};
	bool menu = true;


	// menu declarations
	unsigned int menuVAO;
	unsigned int menuVBO1, menuVBO2;
	char* fileName; char* shaderName;
	
	void loading(GLFWwindow *window, Shader menuShader)
	{
		
		glGenVertexArrays(1, &menuVAO);
		glBindVertexArray(menuVAO);
		//	first vbo
		glGenBuffers(1, &menuVBO1);
		glBindBuffer(GL_ARRAY_BUFFER, menuVBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(menuVecs), menuVecs, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//	second vbo
		glGenBuffers(1, &menuVBO2);
		glBindBuffer(GL_ARRAY_BUFFER, menuVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(menuTex), menuTex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//	MENU TEXTURE
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		menuShader.use();
		glBindVertexArray(menuVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		processInputMenu(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	void pause(GLFWwindow *window, Shader menuShader)
	{

		while (menu)
		{
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			menuShader.use();
			glBindVertexArray(menuVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			processInputMenu(window);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glDeleteVertexArrays(1, &menuVAO);
		glDeleteBuffers(1, &menuVBO1);
		glDeleteBuffers(1, &menuVBO2);
		
	}

	void processInputMenu(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			menu = false;
		}
	}

};