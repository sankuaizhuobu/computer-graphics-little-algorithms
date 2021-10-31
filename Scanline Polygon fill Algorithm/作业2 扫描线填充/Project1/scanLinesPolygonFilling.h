// scanLinesPolygonFilling.h
#pragma once

#include <iostream>
#include<algorithm>
#include <math.h>
#include<vector>
#define GLEW_STATIC  // 放在 #include "glew.h" 前面
#include "glew.h"  // 链接器 -> 输入 -> 附加依赖项：opengl32.lib glfw3.lib glew32s.lib
#include "glfw3.h"

#define maxWd 800
#define maxHt 600

class scanLinesPolygonFilling {
private:
	struct Point {
		int x;
		int y;
	};
	std::vector<Point> points;

	struct Edge {
		Point p1;
		Point p2;
	};
	std::vector<Edge> edges;

	struct edgeNode {
		float x = 0;  // 为什么 x 声明为 float ？如果 x 为 int，在 x += delx 的时候，例如 x-0.5，会变成 x-1，下一个也是 x-1，这样相当于 delx 变成了-1
		float delx = 0;
		int ymax = 0;
		int ymin = 0;  // 判断凹凸顶点的时候会用到
		edgeNode* next = NULL;
	};
	edgeNode NET[maxHt];
	edgeNode* AET = new edgeNode;
	edgeNode* p = NULL;
	edgeNode* q = AET;

	FILE* fp;

	void readEdges();
	static bool cmp(Edge a, Edge b);  // 静态成员函数可以通过类来直接调用，编译器不会为它增加形参 this，它不需要当前对象的地址，所以不管有没有创建对象，都可以调用静态成员函数。
	void putPoint(int x, int y);
	edgeNode* newNode(int j);
	edgeNode* newNode(edgeNode* p);
	void insertAET();
	void fill(int x1, int x2, int y);

public:
	void InitNET();
	void printNET();
	void printAET();
	void scan();
	scanLinesPolygonFilling(const char* fileName);
	~scanLinesPolygonFilling();

	void drawPoints();
	void drawEdges();
};