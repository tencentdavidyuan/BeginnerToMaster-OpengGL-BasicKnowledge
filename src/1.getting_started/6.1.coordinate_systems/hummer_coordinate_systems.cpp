#include <iostream>
#include <glad/glad.h>
#include <GLFW//glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

void OnSetFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);

int main() {

	if (!glfwInit()) {
		std::cout << "Glfw init failed!" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Hummer_Coordinate_System", NULL, NULL);
	if (_window == NULL) {
		std::cout << "Glfw create window failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, OnSetFrameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// build and compile our shader programe
	// -------------------------------------
	Shader ourShader("hummer_coordinate_systems.6.1.vs", "hummer_coordinate_systems.6.1.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// --------------------------------------------------------------------
	float vertices[] = {
			// positions          // colors       // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	// load and create texture
	// -----------------------
	unsigned int _texture1;
	unsigned int _texture2;
	{
		// texture1 
		glGenTextures(1, &_texture1);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set the texture filter parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image
		int imgWidth;
		int imgHeight;
		int imagChannel;
		unsigned char* imgData = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(),
			&imgWidth, &imgHeight, &imagChannel, 0);
		if (imgData != NULL) 
		{
			glTexImage2D(GL_TEXTURE_2D,
				0, GL_RGB,
				imgWidth, imgHeight, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else 
		{
			std::cout << "Failed to load texture1!" << std::endl;
		}
		stbi_image_free(imgData);

		// texture2
		glGenTextures(1, &_texture2);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		// set the texture wrapping 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set the texture filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//
		imgData = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(),
			&imgWidth, &imgHeight, &imagChannel, 0);
		if (imgData != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				imgWidth, imgHeight, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture2!" << std::endl;
		}
		stbi_image_free(imgData);

		ourShader.use();
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);
	}



	while (!glfwWindowShouldClose(_window))
	{
		// render
		// -------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);

		// activate shader
		ourShader.use();

		// 
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
			projection = glm::perspective(glm::radians(60.f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

			// retrieve the matrix uniform locations
			unsigned int modelLocation = glGetUniformLocation(ourShader.ID, "model");
			unsigned int viewLocation = glGetUniformLocation(ourShader.ID, "view");
			unsigned int projectionLocation = glGetUniformLocation(ourShader.ID, "projection");

			// pass them to the shaders (3 different ways) 
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			ourShader.setMat4("projection", projection);
		}

		// render container
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// glfw : swap buffers and poll IO event
		// -----------------------------------------------
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);


	glfwTerminate();

	return 0;
}


void OnSetFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}