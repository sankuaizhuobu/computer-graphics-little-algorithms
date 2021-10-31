// scanLinesPolygonFilling.cpp

#include"scanLinesPolygonFilling.h"

void scanLinesPolygonFilling::readEdges() {
	int x, y;
	rewind(fp);  // ���ļ��ڲ���ָ������ָ��һ�����Ŀ�ͷ
	while (!feof(fp)) {  // ������ϵ��ļ�������������ļ��������򷵻ط�0ֵ�����򷵻�0
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
		else edges.push_back({ points[i].x, points[i].y, points[i + 1].x, points[i + 1].y });  // ȷ���ߵ� y1 < y2
	}
	//if (points[i].y > points[0].y) {  // ���һ����
	//	edges.push_back({ points[0].x, points[0].y, points[i].x, points[i].y });
	//}
	//else edges.push_back({ points[i].x, points[i].y, points[0].x, points[0].y });  // ȷ���ߵ� p1.y < p2.y
	for (Edge& e : edges) {
		printf("%d,%d - %d,%d\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	}
	//sort(edges.begin(), edges.end(), cmp);    // �� p1.y ��������sort�������һ������Ҫ����һ����ͨ����ָ�룬�����ǳ�Ա����ָ�룬����Ҫ��static��
	//for (Edge& e : edges) {
	//	printf("%d,%d - %d,%d\n", e.p1.x, e.p1.y, e.p2.x, e.p2.y);
	//}
}

bool scanLinesPolygonFilling::cmp(Edge a, Edge b) {  // �����ʱ�򲻼�static
	return a.p1.y < b.p1.y;
}

void scanLinesPolygonFilling::putPoint(int x, int y) {
	glVertex2f((float)x / maxWd * 2 - 1.0, (float)y / maxHt * 2 - 1.0);
	//glVertex2f((float)x / maxWd, (float)y / maxHt);
}

scanLinesPolygonFilling::edgeNode* scanLinesPolygonFilling::newNode(int j) {  // �� edges[j] ���� NET �еı߽��
	edgeNode* en = new edgeNode;

	// �߽��ĵ�һ������������ x ����
	en->x = edges[j].p1.x;

	// �߽��ĵڶ����������ߵ� delx
	float k = (float)(edges[j].p2.y - edges[j].p1.y) / (float)(edges[j].p2.x - edges[j].p1.x);
	en->delx = 1 / k;
	//printf("k of the new edge = %f\n", k);
	//printf("delx of the new edge = %f\n", en->delx);

	//�߽��ĵ������������ߵ���� y ����
	en->ymax = edges[j].p2.y;

	//�߽��ĵ��ĸ��������ߵ���� y ����
	en->ymin = edges[j].p1.y;
	return en;
}

scanLinesPolygonFilling::edgeNode* scanLinesPolygonFilling::newNode(edgeNode* p) {  // �� NET �еĽ������ AET �еı߽��
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
		putPoint(x, y);  // Ϊʲô��������������ҿ���
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
		p = &NET[i];  // p ָ��ͷ���
		//printf("i = %d\n", i);
		for (int j = 0; j < edges.size(); ++j) {
			if (i == edges[j].p1.y && edges[j].p1.y != edges[j].p2.y) {
				edgeNode* en = newNode(j);
				// ͷ�巨 ���ӵ�����
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

		// ���� NET[i] �������߽����� AET
		p = &NET[i];
		while (p->next) {
			insertAET();
			//printAET();
			p = p->next;
		}
		printf("i = %d: ", i);
		printAET();

		// ���� AET����Բ���ɫ
		glColor3f(0.0, 1.0, 0.0);
		if (AET->next) {
			q = AET->next;
			int count = 0;
			float x1 = 0;
			float x2 = 0;
			while (q) {
				if (q->next && abs(q->x - q->next->x) <= 0.5) {  // ɨ�赽�˶���
					if (q->ymin == i && q->ymin == q->next->ymin) {  // �����㣨��ɫ��
						if (count == 1) {  // ǰ���д���Եı�
							// ��������㣬count����Ϊ1
						}
						else {  // û��Ҫ��Եı�
							putPoint(q->x, i);  // ��ɫ��count����Ϊ0
						}
					}
					else if (q->ymax == i && q->ymax == q->next->ymax) {  // ͹���㣨��ɫ��
						if (count == 1) {  // ǰ���д���Եı�
							//fill(x1, q->x - 1, i);  // �����
							//x1 = q->x;  // Ȼ�����ԣ�count ����1
						}
						else {  // û��Ҫ��Եı�
							putPoint(q->x, i);  // ��ɫ��count����Ϊ0
						}
					}
					else {  // �������㣨�����Ƕ���һ����ֻ����Ҫ����һ���㣩
						if (count == 1) {  // ǰ���д���Եı�
							fill(x1, q->x, i);  // ��ɫ
							count = 0;
						}
						else {  // û��Ҫ��Եı�
							x1 = q->x;
							count = 1;
						}
					}
					q = q->next->next;
				}
				else {  // ���Ƕ���
					if (count == 1) {  // ǰ���д���Եı�
						fill(x1, q->x, i);  // ��ɫ
						count = 0;
					}
					else {  // û��Ҫ��Եı�
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

		// ���� AET��1��ɾ�� ymax < i + 1 �Ľ�㣬2��ymax >= i + 1 �Ľ��� x + delx
		q = AET;
		//while (q && q->next) {
		//	if (q->next && q->next->ymax < i + 1) {
		//		while (q->next && q->next->ymax < i + 1) {  // �����ļ�����Ҫɾ���������ɾ���ᱻ����
		//			q->next = q->next->next;  // ���� q->next ���ܱ�Ϊ NULL
		//		}
		//	}
		//	else {
		//		q->next->x += q->next->delx;
		//	}
		//	q = q->next;  // ������� q->next ��Ϊ NULL������ q ���Ϊ NULL������ѭ������Ҫ�� q
		//}
		while (q->next) {
			if (q->next && q->next->ymax < i + 1) {  // ymax < i + 1 �Ľ�㣬ɾ��
				q->next = q->next->next;  // ���ﲻ���£���Ϊ��һ���жϵĻ��� q->next
			}
			else {  // ymax >= i + 1 �Ľ�㣬x + delx
				q->next->x += q->next->delx;
				q = q->next;
			}
		}
		/*printf("i = %d: ", i);
		printAET();*/

		// �� AET ð������
		edgeNode* pre = AET;
		if (pre && pre->next && pre->next->next) {  // ��2�������Ͻ��
			q = AET->next;
			edgeNode* next = q->next;
			while (next) {
				if (next->x < q->x) {  // �������
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
	// ���붥���ļ�
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
	glFlush();  // ǿ��ˢ�»��壬��֤��ͼ����ִ�У������Ǵ洢�ڻ�����
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