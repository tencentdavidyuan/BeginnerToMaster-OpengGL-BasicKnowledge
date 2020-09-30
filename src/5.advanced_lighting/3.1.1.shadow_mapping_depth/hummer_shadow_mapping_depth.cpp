

#include <glad/glad.h> // ͷ�ļ�һ������ <GLFW/glfw3.h> ��ǰ��
#include <GLFW/glfw3.h>



#include <learnopengl/camera.h>

#include <iostream>


const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Camera����
Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));
float _xLastPos = WINDOW_WIDTH / 2.0;
float _yLastPos = WINDOW_HEIGHT / 2.0;

// Plane Geometry 
unsigned int _vaoPlane;



// ��������
void HummerFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);
void HummerCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void HummerScrollCallback(GLFWwindow* window, double xPos, double yPos);

void SetupPlane();

void HummerRenderScene();

int main()
{
	// glfw: ��ʼ������������
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to init glfw!" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// ����glfw����
	GLFWwindow* _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpengl",
		NULL, NULL);
	if (_window == NULL) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// ����framebuffer resize�Ļص���������framebuffer resizeʱ����
	glfwSetFramebufferSizeCallback(_window, HummerFrameBufferSizeCallback);
	// ���ù��λ�ûص�����������ƶ�ʱ������
	glfwSetCursorPosCallback(_window, HummerCursorPosCallback);
	// ���ù��ֻص�����
	glfwSetScrollCallback(_window, HummerScrollCallback);


	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	SetupPlane();

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		HummerRenderScene();

		glfwSwapBuffers(_window);
	}

	glfwTerminate();
	return 0;
}



void HummerFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

void HummerCursorPosCallback(GLFWwindow* window, double xPos, double yPos) {

}

void HummerScrollCallback(GLFWwindow* window, double xPos, double yPos) {

}

void SetupPlane()
{
	float planeVertices[] = {
			// position				// normal		// texcoords
		 25.0f, -0.5f,  25.0f,	0.0f, 1.0f, 0.0f,	25.0f, 0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   25.0f, 10.0f
	};
	

	// ����VAOʹ��glGenVertexArrays
	glGenVertexArrays(1, &_vaoPlane);

	// ����VBO
	unsigned int vboPlane;
	glGenBuffers(1, &vboPlane);

	// ��Ҫʹ��glBindVertexArray������Ϊ��ǰ������VAO
	// ֮���������й��ڶ�����������ã���������ʹ�õ�
	// VBO���󣬶����������õ���Ϣ����洢��VAO֮��	
	glBindVertexArray(_vaoPlane);

	// ��VBO������VBO�е�����
	glBindBuffer(GL_ARRAY_BUFFER, vboPlane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

	// ���ö�������
	// λ������, ��shader�е������н��� (�������д�����Խ���)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)0);
	// ��������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)(3 * sizeof(GL_FLOAT)));
	// ��������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)(6 * sizeof(GL_FLOAT)));

	// ���VAO �������Ҫ��Ϊ�˲�Ӱ�����VAO�����ã�
	glBindVertexArray(0);

}


void HummerRenderScene() {
	glm::mat4 model = glm::mat4(1.0f);
	glBindVertexArray(_vaoPlane);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}