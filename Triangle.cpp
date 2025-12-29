/* Created by Easy 2025.12.24 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 我们需要一个顶点着色器用于将输入的坐标转换为标准化设备坐标
// 而顶点着色器并不是C++编写的，而是用着色器语言GLSL编写的，所以我们需要用char类型先编写着色器，再对着色器进行编译
const char* VertexShaderSource =
"#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";

// 片段着色器
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor=vec4(1.0f,0.5f,0.2f,0.5f);\n"
"}\0";

int main()
{
	// 定义一个顶点数组
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	// 生成顶点缓冲对象VBO
	// 生成VBO的目的是为了缓存顶点，使其一次性发送给GPU进行渲染，而不是收到发送导致效率降低
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// 在OpenGL里存在很多缓冲类型，对应不同函数，而缓冲类型均放在同一函数里：GL_BindBuffer
	// 因此需要把VBO缓冲绑定在对象的缓冲类型函数里：GL_APPLY_BUFFER
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 此时VBO中并没有先前创建的顶点数组数据，因此需要glBufferData函数来把数据放入VBO中，同时发送给显卡，即：CPU到GPU
	// glBufferData(操作目标，数据大小，数据，用途)
	// static_draw代表数据不会被GPU改变，dynamic和stream代表显卡可以高速读取写入改变数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

	// 用glUseProgram函数调用着色器程序
	glUseProgram(shaderProgram);

	// 显卡需要知道通过glBufferData传输的数据怎么解释，因此需要glVertexAtrribPointer函数替显卡解释数据
	// 0代表前面顶点着色器的location=0，代表0号通道
	// 3代表size，一个顶点中有几个数组成
	// GL_FLOAT 代表数据类型，均是浮点数
	// GL_FALSE 一般用于处理RGB和3D光照法线的处理，/255省内存，其他情况一般设置为FALSE
	// 步长，告诉显卡一共要读几个字节才读完一个顶点
	// 从0开始读取
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 打开0号通道，对应顶点着色器的location=0
	glEnableVertexAttribArray(0);


	// 顶点缓存数组对象VBO创建完毕，现在创建顶点数组对象VAO用于绘图
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	// 使用VAO只需要绑定
	glBindVertexArray(VAO);

	// 绘制三角形
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
