// scanLinesPolygonFilling.h
#pragma once

#include <iostream>
#include<algorithm>
#include <math.h>
#include<vector>
#define GLEW_STATIC  // ���� #include "glew.h" ǰ��
#include "glew.h"  // ������ -> ���� -> ���������opengl32.lib glfw3.lib glew32s.lib
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
		float x = 0;  // Ϊʲô x ����Ϊ float ����� x Ϊ int���� x += delx ��ʱ������ x-0.5������ x-1����һ��Ҳ�� x-1�������൱�� delx �����-1
		float delx = 0;
		int ymax = 0;
		int ymin = 0;  // �жϰ�͹�����ʱ����õ�
		edgeNode* next = NULL;
	};
	edgeNode NET[maxHt];
	edgeNode* AET = new edgeNode;
	edgeNode* p = NULL;
	edgeNode* q = AET;

	FILE* fp;

	void readEdges();
	static bool cmp(Edge a, Edge b);  // ��̬��Ա��������ͨ������ֱ�ӵ��ã�����������Ϊ�������β� this��������Ҫ��ǰ����ĵ�ַ�����Բ�����û�д������󣬶����Ե��þ�̬��Ա������
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