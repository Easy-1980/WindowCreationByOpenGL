***

# OpenGL 学习笔记 - 窗口创建 (Window Creation)

> **Created By:** Easy
> **Date:** 2025.12.18

本项目是基于 OpenGL (GLFW + GLAD) 的基础窗口渲染程序。

##  实现思路与流程

1.  **初始化 (Initialization)**
    *   调用 `glfwInit()` 初始化 GLFW 库。
    *   配置 OpenGL 版本为 3.3 (Major 3, Minor 3)。
    *   配置 Core Profile (核心模式) 以使用现代化 OpenGL 特性。

2.  **创建窗口 (Window Context)**
    *   使用 `glfwCreateWindow` 创建 800x600 的窗口。
    *   **关键点**：必须检查 `window == NULL` 的情况，防止因驱动或内存问题导致的空指针崩溃。

3.  **上下文绑定 (Context Binding)**
    *   OpenGL 是一个状态机，必须通过 `glfwMakeContextCurrent(window)` 告诉它当前在哪个窗口上作画。

4.  **加载函数指针 (GLAD)**
    *   因为 OpenGL 驱动实现各异，需要 GLAD 库来动态加载函数地址。
    *   必须在调用任何 OpenGL 函数（如 `glViewport`）之前初始化 GLAD。

5.  **渲染循环 (Render Loop)**
    *   程序不能一闪而过，需要使用 `while(!glfwWindowShouldClose)` 保持运行。
    *   **双缓冲 (Double Buffering)**：使用 `glfwSwapBuffers` 防止画面闪烁。
    *   **事件处理**：使用 `glfwPollEvents` 响应鼠标和键盘操作。

6.  **交互与回调 (Callbacks)**
    *   **视口调整**：为了解决窗口拉伸时画面不匹配的问题，实现了 `size_callback` 函数。
    *   **输入控制**：封装了 `processInput` 函数，按 `ESC` 键可优雅退出。

7.  **Bug 修复记录**
    *   **白屏问题**：在 Windows 下拖动窗口会导致主循环阻塞，出现白色背景。解决方案是在回调函数中强制插入渲染指令。

***
```C
#pragma region 完整思路
/*
本项目的目的是用OpenGL的GLFW库创建一个小窗口；
1.首先需要实例化GLFW窗口:glfwInit()；
2.接着要告诉GLFW库你所使用的OpenGl库的Major和Minor的版本是什么，调用的是GLFW的核心库；
3.然后告诉程序创建一个窗体用GLFWwindow*窗体指针类型创建，
  此时传入的是一个指针，需要排除空指针的情况，否则程序会崩溃：if（window==null）；
4.此时窗体已经在内存中创建完毕，我们要把它调用出来有两个问题：
  1）OpenGL只是画家，我们需要告诉它在哪里绘制窗体（window）：glfwMakeContextCurrent(window)；
  2）窗口和视图之间的关系，绘制出来的窗体正常来说应该要和视图尺寸一样，此时调用glView函数，
	 但是调用glView需要初始化GLAD库，否则就会崩溃，因此需要写一个if判断；
5.现在窗体已经创建好了，视图也对应上了，但是顺着程序走，会创建完窗体后立刻结束，导致窗体一闪而过。
  因此需要写一个while循环保证只要当我们关闭窗体时窗体才会关闭；
  同时又引出一个问题，需要有一个函数来监控鼠标（或键盘）输入：glfwPollEvents才能保证我们想关闭才关闭；

优化机制(为了未来绘制复杂图形)：
6.此时窗体已经完美显示在屏幕上了，为了绘制更复杂的图形，需要加入双缓冲机制：glfwSwapBuffers；
7.当用户调整窗体大小，视图也需要被调整(此时已经跳出了所规定的800×600像素尺寸)，
  因此需要写一个回调函数：size_callback，使窗口和视图的尺寸始终保持一致；
8.现在可以加一个键盘控制关闭的功能，如：Esc关闭。
  GLFW库的几个输入函数可实现：glfwGetKey，但为了代码整洁，写一个函数来控制：KeyControl；
9.现在加入背景颜色：glClearColor和glClear。前者赋予颜色，后者清除颜色缓存；
10.最后一步：在放大窗体时，会出现一小块白色矩形区域，是因为Windows系统自动填充的白色
   而此时while循环无法进行，始终卡在glfwPollEvents函数里。
   此时把添加背景色、清除背景色、双缓存三行代码加入到回调函数中，强制渲染即可解决；
11.第9步可以封装在函数里：Render--v.渲染...
*/
#pragma endregion
```
