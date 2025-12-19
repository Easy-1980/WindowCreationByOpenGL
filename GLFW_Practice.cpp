/* Created By Easy 2025.12.18 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma region 窗体原始长宽尺寸
const int width = 800;
const int height = 600;
#pragma endregion

#pragma region 封装函数
void size_callback(GLFWwindow* window, int width, int height);
void KeyControl(GLFWwindow* window);
void Render();
#pragma endregion

int main()
{
	// 实例化GLFW窗口
	glfwInit();
	// 告诉OpenGL，GLFW的版本是3.3及以后
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 告诉OpenGL，只可调用GLFW的核心库
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 用GLFWwindow*窗体指针数据类型创建GLFW窗体
	// 前两个参数代表窗体尺寸，第三个代表窗体名称；
	// 1NULL代表窗口模式，传入的如果是显示器指针则为全屏模式；
	// 2NULL代表资源共享，不懂，高级
	GLFWwindow* window = glfwCreateWindow(width, height, "FirstForm", NULL, NULL);
	// 如果创建失败，则打印错误信息，终止glfw窗体进程，return -1代表程序失败
	if (window==NULL)
	{
		std::cout << "无法创建GLFW窗体" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 告诉OpenGL要在哪里绘制窗体
	glfwMakeContextCurrent(window);

	// 防止GLAD库没有初始化导致程序崩溃
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD库未安装，初始化失败" << std::endl;
		return -1;
	}

	// 对视图进行定义
	/*
	// "请把渲染的内容映射到一个矩形区域内，这个矩形从窗口的左下角 (0,0) 开始，宽度为 800 像素，高度为 600 像素。"
	glViewport(0, 0, width, height);
	*/
	// 先对视图进行定义
	glViewport(0, 0, width, height);
	// 拖动窗体触发事件
	glfwSetFramebufferSizeCallback(window, size_callback);

	// 告诉程序在它应该被关闭时被关闭，while循环不断绘制窗体
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘和鼠标的行为
		glfwPollEvents();
		// 键盘输入控制关闭窗体
		KeyControl(window);
		// 添加背景色
		Render();
		// 双缓冲机制
		glfwSwapBuffers(window);
		
	}
	
	// 清空所有进程
	glfwTerminate();
	return 0;
}

// 渲染函数
void Render()
{
	// 赋予背景色
	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
	// 清除背景色
	glClear(GL_COLOR_BUFFER_BIT);
}

// 回调函数
void size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Render();
	glfwSwapBuffers(window);
}

// 键盘输入控制窗体关闭
void KeyControl(GLFWwindow* window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
