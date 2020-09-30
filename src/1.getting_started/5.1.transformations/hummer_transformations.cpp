#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl//filesystem.h>
#include <learnopengl/shader.h>

// settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// 函数声明
void HummerFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);

void HummerProcessInput(GLFWwindow* window);

int main() {

	if (!glfwInit()) {
		std::cout << "glfw init fail!" << std::endl;
		return 0;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 创建glfw窗口
	GLFWwindow* _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpengl",
		NULL, NULL);
	if (_window == NULL) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, HummerFrameBufferSizeCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("hummer_transform.5.1.vs", "hummer_transform.5.1.fs");
	//Shader ourShader("hummer_texture.vs", "hummer_texture.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
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

	// 创建几何体
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		// Color属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// 纹理UV属性
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	unsigned int _texture1;
	unsigned int _texture2;
	{
		// Load and create texture 0
		// ------------------------------------------------------------------
		glGenTextures(1, &_texture1);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set the texture filter parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int imgWidth = 0;
		int imgHeight = 0;
		int imgChannels = 0;
		unsigned char* imgData = stbi_load(
			FileSystem::getPath("resources/textures/container.jpg").c_str(),
			&imgWidth,
			&imgHeight,
			&imgChannels,
			0);
		if (imgData != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB,
				GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture0" << std::endl;
		}
		stbi_image_free(imgData);

		// Load and create texture 1
		// ------------------------------------------------------------------
		glGenTextures(1, &_texture2);
		glBindTexture(GL_TEXTURE_2D, _texture2);
		// set the texture wrap parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set the texture filter parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//
		stbi_set_flip_vertically_on_load(true);
		// 
		imgData = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(),
			&imgWidth, &imgHeight, &imgChannels, 0);
		if (imgData != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture1" << std::endl;
		}
		stbi_image_free(imgData);
	}

	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	
	//	
	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on co
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2);

		{
			// create transformations
			glm::mat4 _transform = glm::mat4(1.0f);
			_transform = glm::translate(_transform, glm::vec3(0.25f, -0.25f, 0.0f));
			_transform = glm::scale(_transform, glm::vec3(0.25f, 0.25f, 0.25f));
			_transform = glm::rotate(_transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			ourShader.use();
			unsigned int locTransform = glGetUniformLocation(ourShader.ID, "transform");
			glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(_transform));
		}

		ourShader.use();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void HummerFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

void HummerProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}