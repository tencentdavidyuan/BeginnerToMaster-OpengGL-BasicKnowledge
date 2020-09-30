
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



const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// camera
Camera _myCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float _xLastPos = WINDOW_WIDTH / 2.0f;
float _yLastPos = WINDOW_HEIGHT / 2.0f;
bool _firstMouse = true;

float _deltaTime = 0.0f;
float _lastFrame = 0.0f;

// light
glm::vec3 _lightDir(-0.2f, -1.0f, -0.8f);

void OnSetFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);
void OnCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path);

void CalculatePlaneParameter(float planeParameter[4], float p0[3], float p1[3], float p2[3]);
void ProjectionShadow(float shadow[16], float plane[4], float light[4]);

void HummerCalculatePlaneParameter(glm::vec4& planeParameter, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
void HummerProjectionShadow(glm::mat4& shadow, glm::vec4 plane, glm::vec4 light);

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
		"5_1_hummer_light_casters_directional", NULL, NULL);
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

	float planeParameter[4] = { 0, };
	{

		float p0[3] = { -1.f, 0.f, -1.f };
		float p2[3] = { 1.f, 0.f,  1.f };
		float p1[3] = { 0.f, 0.f, 3.f };
		CalculatePlaneParameter(planeParameter, p0, p1, p2);


	}
	glm::vec4 planeParameter2 = glm::vec4(0.f);
	{
		
		glm::vec3 p0 = glm::vec3(-1.f, -0.5f, -1.f);
		glm::vec3 p2 = glm::vec3( 1.f, -0.5f,  1.f);
		glm::vec3 p1 = glm::vec3( 0.f, -0.5f,  3.f);
		HummerCalculatePlaneParameter(planeParameter2, p0, p1, p2);
	}

	glm::mat4 _projectionShadow;
	HummerProjectionShadow(_projectionShadow, planeParameter2, glm::vec4(-_lightDir, 0.f));

	// build and compile our shader programe
	// -------------------------------------
	
	HummerShader _cubeShader("5_1_hummer_light_casters_directional.vs",
		"5_1_hummer_light_casters_directional.fs");
	HummerShader _planeShader("5_1_hummer_directionalLight_projecction_shadow_ground.vs", 
		"5_1_hummer_directionalLight_projecction_shadow_ground.fs");
	HummerShader _projectionShadowShader("5_1_hummer_projection_shadow.vs", 
		"5_1_hummer_projection_shadow.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// --------------------------------------------------------------------
	float vertices[] = {
		// position			// normal			// texcoord
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};



	unsigned int _vaoCube;
	unsigned int _vboCube;
	{
		glGenVertexArrays(1, &_vaoCube);
		glGenBuffers(1, &_vboCube);

		glBindVertexArray(_vaoCube);
		glBindBuffer(GL_ARRAY_BUFFER, _vboCube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texcoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	unsigned int _diffuseMap;
	unsigned int _specularMap;
	{
		_diffuseMap = loadTexture(
			FileSystem::getPath("resources/textures/container2.png").c_str());
		_specularMap = loadTexture(
			FileSystem::getPath("resources/textures/container2_specular.png").c_str());

		_cubeShader.use();
		glUniform1i(glGetUniformLocation(_cubeShader.ID, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(_cubeShader.ID, "material.specular"), 1);
	}

	unsigned int _vaoPlane;
	unsigned int _vboPlane;
	{
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			 15.0f, -0.51f,  15.0f,  2.0f, 0.0f,
			-15.0f, -0.51f,  15.0f,  0.0f, 0.0f,
			-15.0f, -0.51f, -15.0f,  0.0f, 2.0f,
					   
			 15.0f, -0.51f,  15.0f,  2.0f, 0.0f,
			-15.0f, -0.51f, -15.0f,  0.0f, 2.0f,
			 15.0f, -0.51f, -15.0f,  2.0f, 2.0f
		};

		glGenVertexArrays(1, &_vaoPlane);
		glBindVertexArray(_vaoPlane);

		glGenBuffers(1, &_vboPlane);
		glBindBuffer(GL_ARRAY_BUFFER, _vboPlane);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		// position property
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		// texcoord property
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	unsigned int _planeMap;
	{
		_planeMap = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());

		_planeShader.use();
		glUniform1i(glGetUniformLocation(_planeShader.ID, "planeMap"), 0);
	}
	


	while (!glfwWindowShouldClose(_window))
	{
		// pre-frame time 
		float _currentFrame = glfwGetTime();
		_deltaTime = _currentFrame - _lastFrame;
		_lastFrame = _currentFrame;

		// input
		// --------
		processInput(_window);

		// view / projection transformations
		glm::mat4 _projection = glm::perspective(glm::radians(_myCamera.Zoom),
			(float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.f);
		glm::mat4 _view = _myCamera.GetViewMatrix();
		glm::vec3 _viewPos = _myCamera.Position;

		// render
		// -------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_cubeShader.use();
		_cubeShader.setVec3("light.direction", _lightDir);
		_cubeShader.setVec3("viewPos", _viewPos);

		// light properties
		glm::vec3 _lightColor;
		_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 _ambientColor = _lightColor * glm::vec3(0.2f);
		glm::vec3 _diffuseColor = _lightColor * glm::vec3(0.5f);
		_cubeShader.setVec3("light.ambient", _ambientColor);
		_cubeShader.setVec3("light.diffuse", _diffuseColor);
		_cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		_cubeShader.setFloat("material.shinness", 128.f);

		_cubeShader.setMat4("projection", _projection);
		_cubeShader.setMat4("view", _view);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _specularMap);

		glBindVertexArray(_vaoCube);
		glm::mat4 _model = glm::mat4(1.0f);
		_cubeShader.setMat4("model", _model);

		glDrawArrays(GL_TRIANGLES, 0, 36);	
		glBindVertexArray(0);

		// projection shadow
		_projectionShadowShader.use();
		_projectionShadowShader.setMat4("projection", _projection);
		_projectionShadowShader.setMat4("view", _view);
		_model = glm::mat4(1.0f);
		//_model = glm::translate(_model, glm::vec3(2.0f, 0.0f, 0.0f));
		_projectionShadowShader.setMat4("model", _model);
		_projectionShadowShader.setMat4("projectionShadow", _projectionShadow);

		glBindVertexArray(_vaoCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// reset 
		glBindVertexArray(0);

		// draw second cube
		// -----------------------------------
		_cubeShader.use();
		_cubeShader.setVec3("light.direction", _lightDir);
		_cubeShader.setVec3("viewPos", _viewPos);

		// light properties
		_cubeShader.setVec3("light.ambient", _ambientColor);
		_cubeShader.setVec3("light.diffuse", _diffuseColor);
		_cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		_cubeShader.setFloat("material.shinness", 128.f);

		_cubeShader.setMat4("projection", _projection);
		_cubeShader.setMat4("view", _view);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _specularMap);

		glBindVertexArray(_vaoCube);
		_model = glm::mat4(1.0f);
		_model = glm::translate(_model, glm::vec3(-2.0f, 0.f, -2.0f));
		_cubeShader.setMat4("model", _model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw planes
		// ----------------------------------------------
		_planeShader.use();
		_planeShader.setMat4("projection", _projection);
		_planeShader.setMat4("view", _view);
		_model = glm::mat4(1.0f);
		_planeShader.setMat4("model", _model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _planeMap);

		glBindVertexArray(_vaoPlane);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

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


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int _textureID;
	glGenTextures(1, &_textureID);

	int imgWidth = 0;
	int imgHeight = 0;
	int imgChannel = 0;
	unsigned char* imgData = stbi_load(path, &imgWidth, &imgHeight, &imgChannel, 0);
	if (imgData != NULL) {
		GLenum format;
		if (imgChannel == 1)
			format = GL_RED;
		else if (imgChannel == 3)
			format = GL_RGB;
		else if (imgChannel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format,
			GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(imgData);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(imgData);
	}

	return _textureID;
}

void CalculatePlaneParameter(float planeParameter[4], float p0[3], float p1[3], float p2[3]) {
	float v0[3] = { 0, };
	float v1[3] = { 0, };

	v0[0] = p1[0] - p0[0];
	v0[1] = p1[1] - p0[1];
	v0[2] = p1[2] - p0[2];

	v1[0] = p2[0] - p0[0];
	v1[1] = p2[1] - p0[1];
	v1[2] = p2[2] - p0[2];
	
	planeParameter[0] = v0[1] * v1[2] - v0[2] * v1[1];
	planeParameter[1] = v0[2] * v1[0] - v0[0] * v1[2];
	planeParameter[2] = v0[0] * v1[1] - v0[1] * v1[0];

	float len = sqrt(planeParameter[0] * planeParameter[0]
		+ planeParameter[1] * planeParameter[1]
		+ planeParameter[2] * planeParameter[2]);
	if (len != 0)
	{
		planeParameter[0] /= len;
		planeParameter[1] /= len;
		planeParameter[2] /= len;
	}

	planeParameter[3] = -(planeParameter[0] * p0[0] + planeParameter[1] * p0[1] + planeParameter[2] * p0[2]);
}

void ProjectionShadow(float shadow[16], float plane[4], float light[4])
{
	float dot = plane[0] * light[0]
		+ plane[1] * light[1]
		+ plane[2] * light[2]
		+ plane[3] * light[3];

	shadow[0]  = dot - light[0] * plane[0];
	shadow[4]  = 0.0 - light[0] * plane[1];
	shadow[8]  = 0.0 - light[0] * plane[2];
	shadow[12] = 0.0 - light[0] * plane[3];

	shadow[1]  = 0.0 - light[1] * plane[0];
	shadow[5]  = dot - light[1] * plane[1];
	shadow[9]  = 0.0 - light[1] * plane[2];
	shadow[13] = 0.0 - light[1] * plane[3];
			   
	shadow[2]  = 0.0 - light[2] * plane[0];
	shadow[6]  = 0.0 - light[2] * plane[1];
	shadow[10] = dot - light[2] * plane[2];
	shadow[14] = 0.0 - light[2] * plane[3];
			   
	shadow[3]  = 0.0 - light[3] * plane[0];
	shadow[7]  = 0.0 - light[3] * plane[1];
	shadow[11] = 0.0 - light[3] * plane[2];
	shadow[15] = dot - light[3] * plane[3];
}

void HummerCalculatePlaneParameter(glm::vec4& planeParameter, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) 
{
	glm::vec3 v1;
	glm::vec3 v2;

	v1 = p1 - p0;
	v2 = p2 - p0;

	glm::vec3 norm = glm::normalize(glm::cross(v1, v2));

	planeParameter[0] = norm[0];
	planeParameter[1] = norm[1];
	planeParameter[2] = norm[2];

	planeParameter[3] = -(planeParameter[0] * p0[0] + planeParameter[1] * p0[1] + planeParameter[2] * p0[2]);
}

void HummerProjectionShadow(glm::mat4& shadow, glm::vec4 plane, glm::vec4 light)
{
	float dot = glm::dot(plane, light);

	shadow[0][0] = dot - light[0] * plane[0];
	shadow[1][0] = 0.0 - light[0] * plane[1];
	shadow[2][0] = 0.0 - light[0] * plane[2];
	shadow[3][0] = 0.0 - light[0] * plane[3];

	shadow[0][1] = 0.0 - light[1] * plane[0];
	shadow[1][1] = dot - light[1] * plane[1];
	shadow[2][1] = 0.0 - light[1] * plane[2];
	shadow[3][1] = 0.0 - light[1] * plane[3];

	shadow[0][2] = 0.0 - light[2] * plane[0];
	shadow[1][2] = 0.0 - light[2] * plane[1];
	shadow[2][2] = dot - light[2] * plane[2];
	shadow[3][2] = 0.0 - light[2] * plane[3];

	shadow[0][3] = 0.0 - light[3] * plane[0];
	shadow[1][3] = 0.0 - light[3] * plane[1];
	shadow[2][3] = 0.0 - light[3] * plane[2];
	shadow[3][3] = dot - light[3] * plane[3];
}