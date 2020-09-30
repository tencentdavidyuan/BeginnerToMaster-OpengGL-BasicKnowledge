/*
#include <iostream>
#include <glad/glad.h>
#include <GLFW//glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>


class HummerShader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	HummerShader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use() const
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};



const int WINDOW_WIDTH = 1240;
const int WINDOW_HEIGHT = 768;

// camera
Camera _myCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float _xLastPos = WINDOW_WIDTH / 2.0f;
float _yLastPos = WINDOW_HEIGHT / 2.0f;
bool _firstMouse = true;

float _deltaTime = 0.0f;
float _lastFrame = 0.0f;

unsigned int _vaoQuad = 0;
unsigned int _vboQuad = 0;

// light
glm::vec3 _lightPos(0.5f, 1.0f, 0.3f);
glm::vec3 _lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 _cubeColor(1.0f, 1.0f, 1.0f);

void OnSetFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);
void OnCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int LoadTexture(const char* path);

void CreateQuad();
void RenderQuad();

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
	glfwSetCursorPosCallback(_window, OnCursorPosCallback);
	glfwSetScrollCallback(_window, OnScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// Enable Depth
	// -----------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader programe
	// -------------------------------------
	HummerShader _normalMappingShader("4_hummer_normal_mapping.vs", 
		"4_hummer_normal_mapping.fs");
	HummerShader _normalShader("4_hummer_normal.vs", "4_hummer_normal.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// --------------------------------------------------------------------

	CreateQuad();

	// load Texture
	// ----------------------------------
	unsigned diffuseMapping = LoadTexture(
		FileSystem::getPath("resources/textures/brickwall.jpg").c_str());
	unsigned normalMapping = LoadTexture(
		FileSystem::getPath("resources/textures/brickwall_normal.jpg").c_str());
	_normalMappingShader.use();
	_normalMappingShader.setInt("diffuseMapping", 0);
	_normalMappingShader.setInt("normalMapping", 1);

	_normalMappingShader.setVec3("lightColor", _lightColor);

	//
	// --------------
	_normalShader.use();
	_normalShader.setVec3("lightColor", _lightColor);

	while (!glfwWindowShouldClose(_window))
	{
		// pre-frame time 
		float _currentFrame = glfwGetTime();
		_deltaTime = _currentFrame - _lastFrame;
		_lastFrame = _currentFrame;

		// input
		// --------
		processInput(_window);
		glm::mat4 _projection = glm::perspective(glm::radians(_myCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 _view = _myCamera.GetViewMatrix();
		
		// render clear
		// ----------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		// render quad 1
		// --------------------
		_normalMappingShader.use();
		_normalMappingShader.setMat4("projection", _projection);
		_normalMappingShader.setMat4("view", _view);

		// render quad 1
		glm::mat4 _model = glm::mat4(1.0f);
		_model = glm::translate(_model, glm::vec3(1.0f, 0.0f, 1.0f));
		_normalMappingShader.setMat4("model", _model);
		_normalMappingShader.setVec3("viewPos", _myCamera.Position);
		_normalMappingShader.setVec3("lightPos", _lightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapping);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMapping);

		RenderQuad();

		// render quad 2
		// ----------------------------
		_normalShader.use();
		_normalShader.setMat4("projection", _projection);
		_normalShader.setMat4("view", _view);

		_model = glm::mat4(1.0f);
		_model = glm::translate(_model, glm::vec3(-1.2f, 0.0f, 1.0f));
		_normalShader.setMat4("model", _model);

		_normalShader.setVec3("viewPos", _myCamera.Position);
		_normalShader.setVec3("lightPos", _lightPos);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapping);

		RenderQuad();


		// glfw : swap buffers and poll IO event
		// -----------------------------------------------
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}


void OnSetFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}

void OnCursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (_firstMouse)
	{
		_xLastPos = xPos;
		_yLastPos = yPos;
		_firstMouse = false;
	}

	float xoffset = xPos - _xLastPos;
	float yoffset = _yLastPos - yPos; // reversed since y-coordinates go from bottom to top

	_xLastPos = xPos;
	_yLastPos = yPos;

	_myCamera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_myCamera.ProcessKeyboard(FORWARD, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_myCamera.ProcessKeyboard(BACKWARD, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_myCamera.ProcessKeyboard(LEFT, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_myCamera.ProcessKeyboard(RIGHT, _deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	_myCamera.ProcessMouseScroll(yoffset);
}

// load texture
// ----------------------------------------------------------------------
unsigned int LoadTexture(const char* path)
{
	unsigned int textureId = (unsigned int)-1;

	int imgWidth = 0;
	int imgHeight = 0;
	int imgChannel = 0;
	unsigned char* imgData = stbi_load(path, &imgWidth, &imgHeight, &imgChannel, 0);
	if (imgData != NULL) {

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		GLenum format = GL_RED;
		if (imgChannel == 3)
			format = GL_RGB;
		else if (imgChannel = 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 
			0, format, imgWidth, imgHeight, 0, format, 
			GL_UNSIGNED_BYTE,
			imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(imgData);
	return textureId;
}


// ----------------------------------------------------------------------

void CreateQuad()
{
	// postion
	glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
	glm::vec3 pos4( 1.0f,  1.0f, 0.0f);

	// texture coordinate
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);

	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;

	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	float quadVertices[] = {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	glGenVertexArrays(1, &_vaoQuad);
	glGenBuffers(1, &_vboQuad);

	glBindVertexArray(_vaoQuad);
	glBindBuffer(GL_ARRAY_BUFFER, _vboQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
		(void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
		(void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
		(void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

void RenderQuad()
{
	glBindVertexArray(_vaoQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

*/