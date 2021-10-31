// main.cpp
#include"scanLinesPolygonFilling.h"

int main(int argc, char** argv) {

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(maxWd, maxHt, "Draw polygon using scanline filling", NULL, NULL);
	if (window == NULL) {
		printf("Open window failed!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed!\n");
		glfwTerminate();
		return -1;
	}

	//glViewport(0, 0, maxWd, maxHt);  // 视口：左下角坐标 + 宽高，以此区域（整个窗口的一部分）为画图区域。默认整个窗口。

	//scanLinesPolygonFilling sl("Poly-1.txt");
	scanLinesPolygonFilling sl("PolyDino.txt");
	sl.InitNET();
	sl.printNET();
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1.0, 0.0, 0.0);
		//sl.drawPoints();
		sl.drawEdges();
		sl.scan();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	system("pause");
	glfwTerminate();
	return 0;
}