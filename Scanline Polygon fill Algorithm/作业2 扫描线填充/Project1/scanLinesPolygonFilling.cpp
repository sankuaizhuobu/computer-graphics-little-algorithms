// scanLinesPolygonFilling.cpp

#include"scanLinesPolygonFilling.h"

void scanLinesPolygonFilling::readEdges() {
	int x, y;
	rewind(fp);  // 将文件内部的指针重新指向一个流的开头
	while (!feof(fp)) {  // 检测流上的文件结束符，如果文件结束，则返回非0值，否则返回0
		fscanf_s(fp, "%d,%d", &x, &y);
		points.push_back({ x, y });
	}
	/*for (Point& p : points) {
		printf("%d,%d\n", p.x, p.y);
	}*/

	int i;
	for (i = 0; i < points.size() - 1; ++i) {
		if (points[i].y > points[i + 1].y) {
			edges.push_back({ points[i + 1].x, points[i + 1].y, points[i].x, points[i].y });
		}
		else edges.push_back({ points[i].x, points[i].y, points[i + 1].x, points[i + 1].y });  // 确保边的 y1 < y2
	}
	//if (points[i].y > points[0].y) {  // 最后一条边
	//	edges.push_back({ points[0].x, points[0].y, points[i].x, points[i].y });
	//}
	//else edges.push_back({ points[i].x, points[i].y, points[0].x, points[0].y });  // 确保边的 p1.y < p2.y
	for (Edge& e : edges) {
		printf("%d,%d - %d,%d\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	}
	//sort(edges.begin(), edges.end(), cmp);    // 按 p1.y 递增排序（sort函数最后一个参数要求是一个普通函数指针，而不是成员函数指针，所以要加static）
	//for (Edge& e : edges) {
	//	printf("%d,%d - %d,%d\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	//}
}

bool scanLinesPolygonFilling::cmp(Edge a, Edge b) {  // 定义的时候不加static
	return a.p1.y < b.p1.y;
}

void scanLinesPolygonFilling::putPoint(int x, int y) {
	glVertex2f((float)x / maxWd * 2 - 1.0, (float)y / maxHt * 2 - 1.0);
	//glVertex2f((float)x / maxWd, (float)y / maxHt);
}

scanLinesPolygonFilling::edgeNode* scanLinesPolygonFilling::newNode(int j) {  // 由 edges[j] 生成 NET 中的边结点
	edgeNode* en = new edgeNode;

	// 边结点的第一个参数，交点 x 坐标
	en->x = edges[j].p1.x;

	// 边结点的第二个参数，边的 delx
	float k = (float)(edges[j].p2.y - edges[j].p1.y) / (float)(edges[j].p2.x - edges[j].p1.x);
	en->delx = 1 / k;
	//printf("k of the new edge = %f\n", k);
	//printf("delx of the new edge = %f\n", en->delx);

	//边结点的第三个参数，边的最高 y 坐标
	en->ymax = edges[j].p2.y;

	//边结点的第四个参数，边的最低 y 坐标
	en->ymin = edges[j].p1.y;
	return en;
}

scanLinesPolygonFilling::edgeNode* scanLinesPolygonFilling::newNode(edgeNode* p) {  // 由 NET 中的结点生成 AET 中的边结点
	edgeNode* en = new edgeNode;
	en->x = p->x;
	en->delx = p->delx;
	en->ymax = p->ymax;
	en->ymin = p->ymin;
	en->next = NULL;
	return en;
}

void scanLinesPolygonFilling::insertAET() {
	q = AET;
	while (q->next && q->next->x < p->next->x) {
		q = q->next;
	}
	edgeNode* en = newNode(p->next);
	en->next = q->next;
	q->next = en;
}

void scanLinesPolygonFilling::fill(int x1, int x2, int y) {
	for (int x = x1; x < x2; ++x) {
		glBegin(GL_POINTS);
		putPoint(x, y);  // 为什么画出来不是左闭右开？
		glEnd();
	}
	glFlush();
}

void scanLinesPolygonFilling::InitNET() {
	/*printf("edges:\n");
	for (Edge& e : edges) {
		printf("%d,%d - %d,%d\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	}*/
	for (int i = 0; i < maxHt; ++i) {
		p = &NET[i];  // p 指向头结点
		//printf("i = %d\n", i);
		for (int j = 0; j < edges.size(); ++j) {
			if (i == edges[j].p1.y && edges[j].p1.y != edges[j].p2.y) {
				edgeNode* en = newNode(j);
				// 头插法 链接单链表
				en->next = p->next;
				p->next = en;
			}
		}
	}
}

void scanLinesPolygonFilling::printNET() {
	printf("New Edge Table: ========\n");
	for (int i = 0; i < maxHt; ++i) {
		printf("i = %d\n", i);
		if (NET[i].next) {
			p = NET[i].next;
			while (p) {
				printf("%.2f, %.2f, %d\n", p->x, p->delx, p->ymax);
				p = p->next;
			}
		}
	}
}

void scanLinesPolygonFilling::printAET() {
	printf("AET:\n");
	if (AET->next) {
		q = AET->next;
		while (q) {
			printf("%.2f, %.2f, %d\n", q->x, q->delx, q->ymax);
			q = q->next;
		}
	}
}

void scanLinesPolygonFilling::scan() {
	for (int i = 0; i < maxHt; ++i) {

		// 遍历 NET[i] 链表，将边结点插入 AET
		p = &NET[i];
		while (p->next) {
			insertAET();
			//printAET();
			p = p->next;
		}
		printf("i = %d: ", i);
		printAET();

		// 遍历 AET，配对并填色
		glColor3f(0.0, 1.0, 0.0);
		if (AET->next) {
			q = AET->next;
			int count = 0;
			float x1 = 0;
			float x2 = 0;
			while (q) {
				if (q->next && abs(q->x - q->next->x) <= 0.5) {  // 扫描到了顶点
					if (q->ymin == i && q->ymin == q->next->ymin) {  // 凹顶点（填色）
						if (count == 1) {  // 前面有待配对的边
							// 跳过这个点，count保持为1
						}
						else {  // 没有要配对的边
							putPoint(q->x, i);  // 填色，count保持为0
						}
					}
					else if (q->ymax == i && q->ymax == q->next->ymax) {  // 凸顶点（填色）
						if (count == 1) {  // 前面有待配对的边
							//fill(x1, q->x - 1, i);  // 先配对
							//x1 = q->x;  // 然后待配对，count 还是1
						}
						else {  // 没有要配对的边
							putPoint(q->x, i);  // 填色，count保持为0
						}
					}
					else {  // 单调顶点（跟不是顶点一样，只不过要跳过一个点）
						if (count == 1) {  // 前面有待配对的边
							fill(x1, q->x, i);  // 填色
							count = 0;
						}
						else {  // 没有要配对的边
							x1 = q->x;
							count = 1;
						}
					}
					q = q->next->next;
				}
				else {  // 不是顶点
					if (count == 1) {  // 前面有待配对的边
						fill(x1, q->x, i);  // 填色
						count = 0;
					}
					else {  // 没有要配对的边
						x1 = q->x;
						count = 1;
					}
					q = q->next;
				}
			}
			/*while (q && q->next) {
				x1 = q->x;
				x2 = q->next->x;
				fill(x1, x2, i);
				q = q->next->next;
			}*/
		}

		// 遍历 AET，1）删除 ymax < i + 1 的结点，2）ymax >= i + 1 的结点的 x + delx
		q = AET;
		//while (q && q->next) {
		//	if (q->next && q->next->ymax < i + 1) {
		//		while (q->next && q->next->ymax < i + 1) {  // 连续的几个都要删掉，如果不删，会被跳过
		//			q->next = q->next->next;  // 这里 q->next 可能变为 NULL
		//		}
		//	}
		//	else {
		//		q->next->x += q->next->delx;
		//	}
		//	q = q->next;  // 如果上面 q->next 变为 NULL，这里 q 会变为 NULL，所以循环条件要加 q
		//}
		while (q->next) {
			if (q->next && q->next->ymax < i + 1) {  // ymax < i + 1 的结点，删除
				q->next = q->next->next;  // 这里不往下，因为下一个判断的还是 q->next
			}
			else {  // ymax >= i + 1 的结点，x + delx
				q->next->x += q->next->delx;
				q = q->next;
			}
		}
		/*printf("i = %d: ", i);
		printAET();*/

		// 对 AET 冒泡排序
		edgeNode* pre = AET;
		if (pre && pre->next && pre->next->next) {  // 有2个及以上结点
			q = AET->next;
			edgeNode* next = q->next;
			while (next) {
				if (next->x < q->x) {  // 交换结点
					q->next = next->next;
					pre->next = next;
					next->next = q;

					pre = next;
					next = q->next;
				}
				else {
					pre = q;
					q = next;
					next = next->next;
				}
			}
		}
		//printf("i = %d: ", i);
		//printAET();
	}
}

scanLinesPolygonFilling::scanLinesPolygonFilling(const char* fileName) {
	// 读入顶点文件
	fopen_s(&fp, fileName, "r");
	if (fp == NULL) {
		printf("Open file failed!\n");
	}
	readEdges();
	fclose(fp);
}

scanLinesPolygonFilling::~scanLinesPolygonFilling() {
	edgeNode* p = AET;
	edgeNode* f = p;
	while (p) {
		p = p->next;
		delete f;
	}
}

void scanLinesPolygonFilling::drawPoints() {
	for (Point& p : points) {
		glBegin(GL_POINTS);
		putPoint(p.x, p.y);
		glEnd();
	}
	glFlush();  // 强制刷新缓冲，保证绘图命令执行，而不是存储在缓冲区
}

void scanLinesPolygonFilling::drawEdges() {
	for (Edge& e : edges) {
		glBegin(GL_LINES);
		putPoint(e.p1.x, e.p1.y);
		putPoint(e.p2.x, e.p2.y);
		glEnd();
	}
	glFlush();
}