/* Created By Easy 2025.12.31
   本程序为着色器进阶教程学习：
   1.在之前的程序里，我们都是在片段着色器中赋予颜色，但是可以直接在顶点着色器中输出一个变量，
   然后在片段着色器中输入与这个变量同名同类型的变量进行传递，这样便可以实现在顶点着色器赋予变量颜色了；
   VertexShader：out vec4 vertexColor；vertexColor=vec4(颜色参数)；
   FragmentShader：in vec4 vertexColorl;out FragmentColor;FragmentColor=vertexColor.
   2.再用Uniform传入全局变量，可在任意位置对颜色进行设置：
   FragmentShader:Uniform vec4 FragColor;
   3.还可直接在顶点数组中设置颜色变量，
   在顶点着色器添加一个Location=1：layout(location=1) in vec3 aColor；
   同样顶点着色器输出片段着色器输入相同变量传输颜色outColor；
   然后在glVertexAttriPointer里绑定颜色属性. */
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

#pragma region 顶点着色器
const char* VertexShaderSource =
"#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec3 aColor;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"	gl_Position=vec4(aPos,1.0);\n"
"	outColor=vec4(aColor,1.0);\n"
"}\0";
#pragma endregion

#pragma region 片段着色器
const char* fragmentShaderSource =
"#version 330 core\n"
//"in vec4 vertexColor;\n"
"in vec4 outColor;\n"
"out vec4 FragColor;\n"
//"uniform vec4 outColor;\n"
"void main()\n"
"{\n"
//"	FragColor=vertexColor;\n"
"	FragColor=outColor;\n"
"}\0";
#pragma endregion

int main()
{
#pragma region 创建窗体
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
	if (window == NULL)
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
#pragma endregion

#pragma region 绘制三角形

	// 定义一个顶点数组
	/*float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};*/
	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	// 顶点缓存数组对象VBO创建完毕，现在创建顶点数组对象VAO用于绘图
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 生成顶点缓冲对象VBO
	// 生成VBO的目的是为了缓存顶点，使其一次性发送给GPU进行渲染，而不是收到发送导致效率降低
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// 使用VAO只需要绑定
	glBindVertexArray(VAO);

	// 在OpenGL里存在很多缓冲类型，对应不同函数，而缓冲类型均放在同一函数里：GL_BindBuffer
	// 因此需要把VBO缓冲绑定在对象的缓冲类型函数里：GL_APPLY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 此时VBO中并没有先前创建的顶点数组数据，因此需要glBufferData函数来把数据放入VBO中，同时发送给显卡，即：CPU到GPU
	// glBufferData(操作目标，数据大小，数据，用途)
	// static_draw代表数据不会被GPU改变，dynamic和stream代表显卡可以高速读取写入改变数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 告诉显卡怎么读取VBO传过来的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 打开0号通道，对应顶点着色器的location=0
	glEnableVertexAttribArray(0);
	// 告诉显卡怎么读取VBO传过来的数据
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	// 打开0号通道，对应顶点着色器的location=0
	glEnableVertexAttribArray(1);
	// 解绑VAO，其实就是标志VAO记录完毕
	glBindVertexArray(0);

	// 此时顶点着色器已经编写完毕，需要将其编译到C++程序中
	// 同样我们也需要生成一个对象vertexShader，再用glCreateShader把GL_VERTEX_SHADER属性与vertexShader绑定
	// 将我们编写的顶点着色器赋予vertexShader里，用glShaderSource函数，然后再用glComplieShader进编译
#pragma region 顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// 此时如果GLSL编译错误，C++并不会报错，因此需要添加一段代码显示报错信息
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#pragma endregion

	// 编译片段着色器
#pragma region 片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#pragma endregion

	// 此时顶点着色器和片段着色器均创建编译完毕，需要有一个着色器程序来链接两个着色器
#pragma region 着色器程序
// 先创建个着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// 然后绑定
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 同样需要检查报错
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#pragma endregion

#pragma endregion


	// 绘制循环
	// 告诉程序在它应该被关闭时被关闭，while循环不断绘制窗体
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘和鼠标的行为
		glfwPollEvents();
		// 键盘输入控制关闭窗体
		KeyControl(window);
		
		// 用glUseProgram函数调用着色器程序
		glUseProgram(shaderProgram);

		//// 传入全局变量Uniform改变G颜色
		//// 设置RGB中G的变化规律
		//float timeValue = glfwGetTime();
		//// 利用相位差，让 R, G, B 不同步，产生彩虹渐变效果
		//// sin 返回 -1 到 1，经过 /2 + 0.5 变成 0 到 1
		//float redValue = (sin(timeValue * 1.0f + 0.0f) / 2.0f) + 0.5f; // 正常
		//float greenValue = (sin(timeValue * 1.0f + 2.0f) / 2.0f) + 0.5f; // 延迟 2 弧度
		//float blueValue = (sin(timeValue * 1.0f + 4.0f) / 2.0f) + 0.5f; // 延迟 4 弧度
		//// 绑定片段着色器的Uniform变量，即查找Uniform的地址
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
		//// 设置Uniform变量
		//glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

		glBindVertexArray(VAO);
		// 渲染绘制
		Render();
		glBindVertexArray(0);

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
	glClearColor(1.0f, 0.9f, 0.9f, 0.9f);
	// 清除背景色
	glClear(GL_COLOR_BUFFER_BIT);
	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}