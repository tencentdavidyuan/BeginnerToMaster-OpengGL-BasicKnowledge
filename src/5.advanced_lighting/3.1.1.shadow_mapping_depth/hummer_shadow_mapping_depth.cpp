

#include <glad/glad.h> // 头文件一定放在 <GLFW/glfw3.h> 的前面
#include <GLFW/glfw3.h>



#include <learnopengl/camera.h>

#include <iostream>


const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Camera参数
Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));
float _xLastPos = WINDOW_WIDTH / 2.0;
float _yLastPos = WINDOW_HEIGHT / 2.0;

// Plane Geometry 
unsigned int _vaoPlane;



// 函数声明
void HummerFrameBufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight);
void HummerCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void HummerScrollCallback(GLFWwindow* window, double xPos, double yPos);

void SetupPlane();

void HummerRenderScene();

int main()
{
	// glfw: 初始化并并且配置
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

	// 创建glfw窗口
	GLFWwindow* _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpengl",
		NULL, NULL);
	if (_window == NULL) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// 设置framebuffer resize的回调函数，当framebuffer resize时调用
	glfwSetFramebufferSizeCallback(_window, HummerFrameBufferSizeCallback);
	// 设置光标位置回调函数，光标移动时触发。
	glfwSetCursorPosCallback(_window, HummerCursorPosCallback);
	// 设置滚轮回调函数
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
	

	// 创建VAO使用glGenVertexArrays
	glGenVertexArrays(1, &_vaoPlane);

	// 创建VBO
	unsigned int vboPlane;
	glGenBuffers(1, &vboPlane);

	// 需要使用glBindVertexArray设置它为当前操作的VAO
	// 之后我们所有关于顶点的数据设置，包括数据使用的
	// VBO对象，顶点属性设置的信息都会存储再VAO之中	
	glBindVertexArray(_vaoPlane);

	// 绑定VBO，设置VBO中的数据
	glBindBuffer(GL_ARRAY_BUFFER, vboPlane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

	// 设置顶点属性
	// 位置属性, 与shader中的内容有交互 (下面两行代码可以交换)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)0);
	// 法线属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)(3 * sizeof(GL_FLOAT)));
	// 纹理属性
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 
		(void*)(6 * sizeof(GL_FLOAT)));

	// 解绑VAO （解绑主要是为了不影响后续VAO的设置）
	glBindVertexArray(0);

}


void HummerRenderScene() {
	glm::mat4 model = glm::mat4(1.0f);
	glBindVertexArray(_vaoPlane);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}