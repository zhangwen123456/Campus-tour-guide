#include "У԰������ѯ.h"
//���Ҷ���
int LocateVex(Map M, char* v) {
	int i;
	for (i = 0; i < M.n; i++) {
		if (strcmp(v, M.spots[i].data.name) == 0)return i;
	}
	return -1;
}
//��ӡ��ͼ
void PrintMap(Map M) {
	int i;
	AdjVexNodeP p;
	for (i = 0; i < M.n; i++) {
		printf("%d %s", i, M.spots[i].data.name);
		p = M.spots[i].firstArc;
		while (p != NULL) {
			printf("  %s(���ȣ�%0.1fkm)", M.spots[p->adjvex].data.name, p->length);
			p = p->nextArc;
		}
		printf("\n");
	}
}
//��ӡ����
void PrintSpots(Map M) {
	int i;
	for (i = 0; i < M.n; i++) {
		printf("%d.%s\n", i, M.spots[i].data.name);
	}
}
//������ͼ
Status CreatMap(Map& M, VexType* vexs, int n, Way* ways, int e) {
	int i, j, k;
	char* v;
	char* w;
	M.n = n;
	M.e = 0;
	M.spots = (ScenicSpot*)malloc(n * sizeof(ScenicSpot));
	M.tags = (int*)malloc(n * sizeof(int));
	for (i = 0; i < M.n; i++) {
		M.tags[i] = UNVISITED;
		M.spots[i].data = vexs[i];//��Ӷ�����Ϣ
		M.spots[i].firstArc = NULL;
	}
	for (k = 0; k < e; k++) {//��ӱ�
		v = ways[k].start;
		w = ways[k].end;
		i = LocateVex(M, v);
		j = LocateVex(M, w);
		if (i < 0 || j < 0)return ERROR;
		if (AddArc(M, i, j, ways[k].length, ways[k].dirS, ways[k].dirE, ways[k].type,
			ways[k].level) == ERROR)return ERROR;
	}
	return OK;
}
//��ʼ����ͼ
void InitMap(Map& M) {//��ʼ������
	static char a[10][20] = {"����","ͼ���","��ѧ¥","�ﾶ��","������","����","һ��",
		"����","����","�ķ�"};
	static char b[10][50] = {"�����ΰ","ѧ����ϰ�ĵط�","ѧ���Ͽεĵط�","ѧ���ܲ��ĵط�",
		"����ë�򳡣���Ӿ�����˶�����","�ж�����򳡣�ʦ��������ĵط�","�������ã�ʦ���Ͳ͵ĵط�",
		"�кܶ�����ŵ꣬װ��Ư��","�������ã�ʦ���Ͳ͵ĵط�","����Ʒ�ֶ������۸�����"};
	static char w[14][6][20] = {
		{"����","ͼ���","��","��","���е�","A"},
		{"ͼ���","��ѧ¥","����","����","���е�","B"},
		{"ͼ���","�ﾶ��","����","����","����","A"},
		{"��ѧ¥","�ﾶ��","��","��","���е�","A"},
		{"�ﾶ��","������","��","��","���е�","A"},
		{"������","����","��","��","����","D"},
		{"��ѧ¥","һ��","����","����","����","C"},
		{"��ѧ¥","����","����","����","���е�","D"},
		{"�ﾶ��","����","����","����","����","C"},
		{"������","����","����","����","����","C"},
		{"����","����","����","����","���е�","A"},
		{"һ��","����","��","��","����","A"},
		{"����","����","��","��","���е�","B"},
		{"����","�ķ�","��","��","���е�","A"}
	};
	static float length[14] = { 0.5,0.2,0.6,0.7,0.1,0.3,0.8,0.6,0.6,0.5,0.4,0.4,0.9,0.5};
	VexType vexs[10];
	Way ways[14];
	int i, n = 10, e = 14;
	for (i = 0; i < n; i++) {
		vexs[i].name = a[i];
		vexs[i].intro = b[i];
	}
	for (i = 0; i < e; i++) {
		ways[i].length = length[i];
		ways[i].start = w[i][0];
		ways[i].end = w[i][1];
		ways[i].dirS = w[i][2];
		ways[i].dirE = w[i][3];
		ways[i].type = w[i][4];
		ways[i].level = w[i][5];
	}
	CreatMap(M, vexs, n, ways, e);
}
//�����
Status AddArc(Map& M, int start, int end, float length, char* dirS,char* dirE,char*type,
	char* level) {
	AdjVexNodeP p;
	if (start < 0 || start >= M.n || end < 0 || end >= M.n)return ERROR;//start��end���㲻����
	p = WayInfo(M, start, end);
	if (p != NULL)return ERROR;//�жϸñ��Ƿ����
	p = (AdjVexNode*)malloc(sizeof(AdjVexNode));
	if (NULL == p)return OVERFLOW;
	p->adjvex = end;//���ڽӱ�����ӱߵ���Ϣ
	p->length = length;
	p->dir = dirS;
	p->type = type;
	p->level = level;
	p->nextArc = M.spots[start].firstArc;
	M.spots[start].firstArc = p;

	p = (AdjVexNode*)malloc(sizeof(AdjVexNode));
	if (NULL == p)return OVERFLOW;
	p->adjvex = start;
	p->length = length;
	p->dir = dirE;
	p->type = type;
	p->level = level;
	p->nextArc = M.spots[end].firstArc;
	M.spots[end].firstArc = p;
	M.e++;
	return OK;
}
//��k����ĵ�һ���ڽӶ���
int FirstAdjVex(Map M, int k, AdjVexNodeP& p) {
	if (k < 0 || k >= M.n)return -1;
	p = M.spots[k].firstArc;
	if (p != NULL)return p->adjvex;
	else return -1;
}
//����һ���ڽӶ���
int NextAdjVex(Map M,int k,AdjVexNodeP& p) {
	if (k < 0 || k >= M.n)return -1;
	if (NULL == p)return -1;
	p = p->nextArc;
	if (p!=NULL)return p->adjvex;
	else return -1;
}
//��·��Ϣ
AdjVexNodeP WayInfo(Map M, int start, int end) {
	AdjVexNodeP p;
	p = M.spots[start].firstArc;
	while (p != NULL) {
		if (p->adjvex == end)break;
		else p = p->nextArc;
	}
	return p;//����ָ�������յ��Ӧ�ıߣ���������ڸñߣ�����NULL
}
//��ѯ������Ϣ
Status InquireSpotInfo(Map M) {
	PrintSpots(M);
	printf("������Ҫ��ѯ�ľ�����ţ�");
	int i=-1;
	while (1) {
		scanf_s("%d", &i);
		getchar();//���ڴ�Ų���ȷ������
		if (i < 0 || i >= M.n)printf("������Ч���������룺");
		else
		{
			printf("��������%s\n��飺%s\n", M.spots[i].data.name, M.spots[i].data.intro);
			system("pause");
			return OK;
		}
	}
}
//��ѯ�ڽӶ�����Ϣ
Status InquireAdjSpots(Map M) {
	PrintMap(M);
	printf("������Ҫ��ѯ�ľ�����ţ�");
	int i=-1,j;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("������Ч���������룺");
		else
		{
			AdjVexNodeP p = NULL;
			for (j = FirstAdjVex(M, i, p); j >= 0; j = NextAdjVex(M, i, p)) {
				printf("%s:%s %0.1fkm\n",M.spots[j].data.name,p->dir,p->length);
			}
			system("pause");
			return OK;
		}
	}
}
//��ѯ��·��Ϣ
Status InquireWayInfo(Map M) {
	int start = -1, end = -1;
	AdjVexNodeP p;
	PrintMap(M);
	printf("������Ҫ��ѯ�ĵ�·�������յ㣨�м��ÿո������:");
	while (1) {
		scanf_s("%d %d", &start, &end);
		getchar();
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("������Ч�����������룺");
		else {
			p = WayInfo(M, start, end);//pָ������ѯ�ıߣ�����߲����ڣ�p==NULL
			if (p == NULL)printf("�����ڴ�%s��%s�ĵ�·\n", M.spots[start].data.name,
				M.spots[end].data.name);
			else printf("%s->%s\n����%s\n���ȣ�%0.1fkm\n��·���ͣ�%s\n��;��ɫ�ȼ���%s\n",
				M.spots[start].data.name, M.spots[end].data.name, p->dir, p->length, p->type,
				p->level);
			system("pause");
			return OK;
		}
	}
}
//�޸ľ���͵�·
void Modify(Map& M) {
	while (1) {
	PrintMap(M);
	printf("1.���Ӿ���\n");
	printf("2.ɾ������\n");
	printf("3.�޸ľ�����Ϣ\n");
	printf("4.���ӵ�·\n");
	printf("5.ɾ����·\n");
	printf("6.�޸ĵ�·��Ϣ\n");
	printf("7.�˳��޸�\n");
	printf("������Ҫִ�еĲ�����");
	int b;
		scanf_s("%d", &b);
		system("cls");
		switch (b) {
		case 1:AddSpot(M); break;
		case 2:RemoveSpot(M); break;
		case 3:ModifySpotInfo(M); break;
		case 4:AddWay(M); break;
		case 5:RemoveWay(M); break;
		case 6:ModifyWayInfo(M); break;
		case 7:return;
		default:printf("������Ч������������\n"); system("pause");
		}
		system("cls");
	}
}
//���Ӷ���
Status AddSpot(Map& M) {
	int i;
	ScenicSpot* newbase;
	newbase = (ScenicSpot*)realloc(M.spots, (M.n + 1) * sizeof(ScenicSpot));
	if (newbase == NULL)return OVERFLOW;
	char* s1 = (char*)malloc(sizeof(char) * MAX);
	char* s2 = (char*)malloc(sizeof(char) * MAX);
	if (s1 == NULL || s2 == NULL)return OVERFLOW;
	M.spots = newbase;
	printf("�����뾰������");
	while (1) {
		scanf_s("%s", s1, MAX);
		for (i = 0; i < M.n; i++) {
			if (strcmp(s1, M.spots[i].data.name) == 0) {//ͨ���ȽϾ������ж��Ƿ��Ѵ��ڸþ���
				printf("�þ����Ѵ��ڣ�����������:");
				break;
			}
		}
		if (i == M.n) {//���þ��㲻���ڣ�¼�뾰����Ϣ
			M.spots[M.n].data.name = s1;
			printf("�����뾰��ļ�飺");
			scanf_s("%s", s2, MAX);
			M.spots[M.n].data.intro = s2;
			M.spots[M.n].firstArc = NULL;
			M.n++;//����������
			printf("����ɹ���\n");
			system("pause");
			return OK;
		}
	}
}
//ɾ������
Status RemoveSpot(Map& M) {
	PrintSpots(M);
	printf("������Ҫɾ���ľ�����ţ�");
	int i=-1, j;
	AdjVexNodeP p, q, pre;
	ScenicSpot* S;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("������Ч�����������룺");
		else {
			for (j = 0; j < M.n; j++) {//������ɾ�����������ıߣ�ɾ����Щ��
				pre = p = M.spots[j].firstArc;//pָ��Ҫɾ���ıߣ�preָ��p��ǰ�����
				if (j != i) {
					while (p) {
						if (p->adjvex > i) {//���λ�����Ҫɾ��������ŵĶ�����Ҫ��1�������
							p->adjvex--;
							pre = p;
							p = p->nextArc;
						}
						else if (p->adjvex == i) {//ɾ��λ��Ϊi�Ľ��
							if (p == pre) {
								M.spots[j].firstArc = p->nextArc;
							}
							else {
								pre->nextArc = p->nextArc;
							}
							q = p;
							p = p->nextArc;
							free(q);
						}
						else {
							pre = p;
							p = p->nextArc;
						}
					}
				}
				else {
					while (p) {//������������λ��Ϊi�Ķ�����ڽ�����Ľ��һһɾ��
						M.e--;//ÿɾһ����㣬�ߵ���Ŀ��1
						q = p;
						p = p->nextArc;
						free(q);
					}
				}
			}
				M.spots[i].firstArc = NULL;
				S = (ScenicSpot*)malloc((M.n - 1) * sizeof(ScenicSpot));
				if (S == NULL)return OVERFLOW;
				for (j = 0; j < M.n - 1; j++) {//������i�����ʣ�µĶ��㸴�Ƶ���һ������������
					if (j < i)S[j] = M.spots[j];
					else
						S[j] = M.spots[j + 1];
				}
				free(M.spots);//�ͷ�ԭ��������ռ�
				M.spots = S;
				M.n--;//������Ŀ��1
				printf("ɾ���ɹ���\n");
				system("pause");
				return OK;
		}

	}
}
//���ӵ�·
Status AddWay(Map& M) {
	PrintMap(M);
	printf("������Ҫ���ӵ�·�������յ���ţ��м��ÿո������:");
	int start=-1, end=-1;
	float length=-1;
	AdjVexNodeP p;
	char* Dir1 = (char*)malloc(sizeof(char) * MAX);
	char* Dir2 = (char*)malloc(sizeof(char) * MAX);
	char* Type = (char*)malloc(sizeof(char) * MAX);
	char* Level = (char*)malloc(sizeof(char) * MAX);
	while (1) {
		scanf_s("%d %d", &start, &end);
		getchar();
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("������Ч�����������룺");
		else {
			p = WayInfo(M, start, end);
			if (p != NULL)printf("ʧ�ܣ���·�Ѵ��ڣ�\n�����������·�������յ���ţ�");
			//�ж�Ҫ���ӵ�·���Ƿ��Ѵ���
			else {
				printf("�������·���ȣ���λ��km����");
				while (1) {
					scanf_s("%f", &length);
					getchar();
					if (length <= 0)printf("������Ч�����������룺");
					else {
						printf("���������������յ�ķ���");
						scanf_s("%s", Dir1, MAX);
						printf("�������յ���������ķ���");
						scanf_s("%s", Dir2, MAX);
						printf("�������·���");
						scanf_s("%s", Type, MAX);
						printf("��������;��ɫ�ȼ���");
						scanf_s("%s", Level, MAX);
						AddArc(M, start, end, length, Dir1, Dir2, Type, Level);
						//���ڽӱ����������ıߵ���Ϣ
						printf("���ӵ�·�ɹ���\n");
						system("pause");
						return OK;
					}
				}
			}
		}
	}
}
//ɾ����·
Status RemoveWay(Map& M) {
	PrintMap(M);
	printf("������Ҫɾ����·�������յ���ţ��м��ÿո������:");
	int start=-1, end=-1;
	AdjVexNodeP p,pre;//pָ��ɾ�ߵĵĽ�㣬preָ��p��ǰ�����
	while (1) {
		scanf_s("%d %d", &start, &end);
		getchar();
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("������Ч�����������룺");
		else {
			pre=p = M.spots[start].firstArc;
			while (p != NULL) {
				if (p->adjvex == end)break;
				else {
					pre = p;
					p = p->nextArc;
				}
			}
			if (p == NULL) {//�����ڸñ�
				printf("ʧ�ܣ���·������!\n����������Ҫɾ����·�������յ���ţ�");
			}
			else {
				if (p == pre) M.spots[start].firstArc = p->nextArc;
				else pre->nextArc = p->nextArc;
				free(p);//ɾ���ڽ�����Ľ��
				pre=p = M.spots[end].firstArc;
				while (p != NULL) //ɾ���յ��ڽ������б߶�Ӧ�Ľ��
				{
					if (p->adjvex == start) {
						if (p == pre)M.spots[end].firstArc = p->nextArc;
						else pre->nextArc = p->nextArc;
						free(p);
						break;
					}
					else {
						pre = p;
						p = p->nextArc;
					}
				}
				M.e--;
				printf("ɾ���ɹ���\n");
				system("pause");
				return OK;
			}
		}
	}
}
//�޸ľ�����Ϣ
Status ModifySpotInfo(Map& M) {
	PrintSpots(M);
	printf("������Ҫ�޸ĵľ�����ţ�");
	int i=-1,j=-1;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("������Ч���������룺");
		else {
			while (1) {
				system("cls");
				printf("1.�޸ľ�����\n");
				printf("2.�޸ľ���ļ��\n");
				printf("3.�޸ľ������;���ļ��\n");
				printf("4.�˳�\n");
				printf("������Ҫִ�еĲ�����");
				scanf_s("%d", &j);
				getchar();
				system("cls");
				switch (j) {
				case 1: {
					printf("�������µľ�������");
					scanf_s("%s", M.spots[i].data.name, MAX);
					printf("�޸ĳɹ���\n");
					break;
				}
				case 2: {
					printf("�������µļ�飺");
					scanf_s("%s", M.spots[i].data.intro, MAX);
					printf("�޸ĳɹ���\n");
					break;
				}
				case 3: {
					printf("�������µľ�������");
					scanf_s("%s", M.spots[i].data.name, MAX);
					printf("�������µļ�飺");
					scanf_s("%s", M.spots[i].data.intro, MAX);
					printf("�޸ĳɹ���\n");
					break;
				}
				case 4:return OK;
				default:printf("������Ч����������\n");
				}
				system("pause");
			}
		}
	}
}
//�޸ĵ�·��Ϣ
Status ModifyWayInfo(Map& M) {
	PrintMap(M);
	printf("����Ҫ�޸ĵ�·�������յ���ţ��м��ÿո��������");
	int s=-1, e=-1;//sΪ���λ��eΪ�յ�λ��
	float l=-1;//��·����
	AdjVexNodeP p, q;
	while (1) {
		scanf_s("%d %d", &s, &e);
		getchar();
		if (s < 0 || s >= M.n || e < 0 || e >= M.n)printf("������Ч�����������룺");
		else {
			p = WayInfo(M, s, e);
			if (p == NULL)printf("�����ڸõ�·���������룺");//�жϱ��Ƿ����
			else {
				printf("�������µĵ�·���ȣ���λ��km��:");
				while (1) {
					scanf_s("%f", &l);
					getchar();
					if (l <= 0)printf("������Ч�����������룺");
					else {
						q = WayInfo(M, e, s);//�����µ���Ϣ
						printf("���������������յ�ķ���");
						scanf_s("%s", p->dir, MAX);
						printf("�������յ���������ķ���");
						scanf_s("%s", q->dir, MAX);
						printf("�������µĵ�·���");
						scanf_s("%s", p->type, MAX);
						printf("��������;��ɫ�ȼ���");
						scanf_s("%s", p->level, MAX);
						q->type = p->type;
						q->level = p->level;
						p->length = l;
						q->length = l;
						printf("�޸ĳɹ���");
						system("pause");
						return OK;
					}
				}
			}
		}

	}
}
//��ѯ���·��
void InquireShortestPath(Map M) {
	PrintMap(M);
	int i = -1, j = -1;
	DistInfo* Dist;
	printf("�����������ţ�");
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("������Ч�����������룺");
		else {
			printf("�������յ���ţ�");
			while (1) {
				scanf_s("%d", &j);
				getchar();
				if (j < 0 || j >= M.n)printf("������Ч�����������룺");
				else {
					Dijkstra(M, i, Dist);
					//ͨ���Ͻ�˹�����㷨�õ�i���㵽�������ɶ�������·��,���·�����Ⱥ����·���ϸö����ǰ�������λ������Distָ���������
					if (Dist[j].lowcost == INFINITY)printf("�����ڴ�%d��%d��ͨ·", i, j);
					//��ͼ�д��ڷ���ͨ���������ܲ�������㵽�յ��ͨ·
					else {
						printf("��%s��%s�����·��Ϊ��\n", M.spots[i].data.name, M.spots[j].data.name);
						Outputpath(M, Dist, j);
						printf("\nȫ��Ϊ��%0.1fkm", Dist[j].lowcost);
					}
					printf("\n");
					system("pause");
					return;
				}
			}
		}
	}
}
//�Ͻ�˹�����㷨
Status Dijkstra(Map M, int i, DistInfo*& Dist) {
	int j, m, k;
	float min;
	AdjVexNodeP p;
	Dist = (DistInfo*)malloc(M.n * sizeof(DistInfo));
	if (Dist == NULL)return OVERFLOW;
	for (j = 0; j < M.n; j++) {//��ʼ��
		Dist[j].lowcost = INFINITY;
		M.tags[j] = UNSELECTED;//���ж�������ڼ���U��
	}
	for (p = M.spots[i].firstArc; p != NULL; p = p->nextArc) {
		//Դ��i���������л�����Ϣ����Dist
		Dist[p->adjvex].pre = i;
		Dist[p->adjvex].lowcost = p->length;
	}
	Dist[i].pre = -1;
	Dist[i].lowcost = 0;//Դ��i��Ϣ����Dist
	M.tags[i] = SELECTED;//��ʼ����U����Դ��i
	for (m = 1; m < M.n; m++) {//��·����������������Դ�㵽������������·��
		min = INFINITY;
		k = 0;
		for (j = 0; j < M.n; j++)
			if (0 == M.tags[j] && Dist[j].lowcost < min) {
				k = j;
				min = Dist[k].lowcost;
			}//k����ǰ������·����k����
		M.tags[k] = SELECTED;//��k������뼯��U
		for (p = M.spots[k].firstArc; p != NULL; p = p->nextArc) {//����Dist����
			j = p->adjvex;
			if (UNSELECTED == M.tags[j] && Dist[k].lowcost + p->length < Dist[j].lowcost) {
				//��j���㾭��k���㵽i�����·�����̣�����·��
				Dist[j].lowcost = Dist[k].lowcost + p->length;
				Dist[j].pre = k;
			}
		}
	}
	return OK;
}
//���Դ�㵽k��������·��
void Outputpath(Map M, DistInfo* Dist, int k) {
	//��Dist�����pre�򣬵ݹ�õ�Դ�㵽k��������·��
	if (-1 == k)return;
	AdjVexNodeP p;
	Outputpath(M, Dist, Dist[k].pre);//����ݹ��ȡ·���ϵĶ���
	if (Dist[k].pre != -1) {
		p = WayInfo(M, Dist[k].pre, k);
		if (p != NULL)printf(" (%s%0.1fkm(%s)) %s", p->dir, p->length, p->type, M.spots[k].data.name);
	//���������ǰ·���ϵĶ���
	}
	else printf("%s", M.spots[k].data.name);
}

//��ѯ������������е�·
void InquireAllPaths(Map M) {
	int t=-1;
	char type[MAX];
	while (1) {
		printf("1.��ѯ���е�\n");
		printf("2.��ѯ����\n");
		printf("3.��ѯ�����·�������е��ͳ�����\n");
		printf("4.�˳�\n");
		printf("������Ҫִ�еĲ�����");
		while (1) {
			scanf_s("%d", &t);
			getchar();
			if (t <= 0 || t > 4)printf("������Ч�����������룺");
			else {
				system("cls");
				switch (t) {//�ж�Ҫ��ѯ�ĵ�·��𣬽�type��Ϊ��Ӧ��·���
				case 1:strcpy_s(type, "���е�"); break;
				case 2:strcpy_s(type, "����"); break;
				case 3:strcpy_s(type, "����·��"); break;
				case 4:return;
				}
				AllPaths(M, type);
				break;
			}
		}
		system("cls");
	}
}
//��ѯָ����·�������е�·
Status AllPaths(Map M,char* type) {
	int start = -1, end = -1, found = 0,i;
	//found == 1�����ҵ�start��end��·��,found == 0��ʾû���ҵ�start��end��·��
	SqStack S;
	InitStack_Sq(S, M.n);
	PrintMap(M);
	printf("�����������ţ�");
	while (1) {
		scanf_s("%d", &start);
		getchar();
		if (start < 0 || start >= M.n)printf("������Ч�����������룺");
		else {
			printf("�������յ���ţ�");
			while (1) {
				scanf_s("%d", &end);
				getchar();
				if (end < 0 || end >= M.n)printf("������Ч�����������룺");
				else {
					for (i = 0; i < M.n; i++)M.tags[i] =UNVISITED;//��ʼ���������
					DFS_T(M, S, start, end, found, type);
					//ͨ����ȱ����õ��Ӷ���start������end������·��
					if (found == 0)printf("�����ڴ�%s��%s��%s\n", M.spots[start].data.name, M.spots[end].data.name, type);
					free(S.elem);
					system("pause");
					return OK;
				}
			}
		}
	}
}
//��ȱ���ʵ�ֲ�ѯ���е�·
void DFS_T(Map M, SqStack& S, int start, int end, int& found, char* type) {
	//ջS������ʱ��ŷ��ʵ���·��,found����Ƿ��ҵ�ͨ·��type��ʾ���ҵĵ�·����
	int i, e;
	AdjVexNodeP p;
	Push_Sq(S, start);//�������ջ
	M.tags[start] = VISITED;
	if (start == end) {//�ݹ�����������������յ�
		PrintStack(S, M);//��ӡջ�д����·��
		Pop_Sq(S, e);//���յ��ջ���ұ��Ϊδ������
		M.tags[e] = UNVISITED;
		found = 1;//found��Ϊ1����ʾ�ҵ��˴�start��found��·��
		return;
	}
	for (i = FirstAdjVex(M, start, p); i >= 0; i = NextAdjVex(M, start, p)) {
		//�������ڽӶ���i
		if (strcmp(type, "����·��") == 0) {
			//��i����δ�����ʣ���i������Ϊ������������ȱ���
			if (M.tags[i] == UNVISITED)DFS_T(M, S, i, end, found, type);

		}
		else {
		//������Ϊ���е����߳���ʱ��������i����δ�������ҵ�·�����Ǻϲ�����iΪ������������ȱ���
			if (M.tags[i] == UNVISITED && (strcmp(p->type, type) == 0))
				DFS_T(M, S, i, end, found, type);
		}
	}
	Pop_Sq(S, e);//������ɺ󣬽���ǰ���·���յ��ջ
	M.tags[e] = UNVISITED;//�Ѹõ���Ϊδ������
}
//��ѯ��������������ѣ��̣�·��
Status BestPath(Map M) {
	PrintMap(M);
	int n = -1,i,temp=-1;
	float length = INFINITY;//��ʼ��
	SqStack S,S1;
	InitStack_Sq(S, M.n);
	InitStack_Sq(S1, M.n);
	printf("������Ҫ�����ľ�������");
	while (1) {
		scanf_s("%d", &n);
		getchar();
		if (n <= 0 || n > M.n)printf("������Ч�����������룺");
		else {
			int* index = (int*)malloc(n*sizeof(int));
			if (NULL == index)return OVERFLOW;
			for (i = 0; i < n; i++) {
				printf("������Ҫ�����ĵ�%d��������ţ�",i+1);
				while (1) {
					scanf_s("%d", &temp);
					getchar();
					if (temp < 0 || temp >= M.n)printf("������Ч�����������룺");
					else {
						index[i] = temp;//index���鴢��Ҫ�����ľ������
						break;
					}
				}
			}
			for (i = 0; i < M.n; i++)M.tags[i] = UNVISITED;//��ʼ��
			for (i = 0; i < n; i++) {
				//��ÿ��Ҫ�����Ķ�����Ϊ������������ȱ������������ѷ���·��
				DFS_A(M, S, index[i], n, index, S1, length);
			}
			if (length == INFINITY)printf("��������ͨ��ȫ�����辰������·����\n");

			else {
				printf("��ѷ���·��Ϊ��");
				PrintStack(S1, M);//��ӡ���·��
				printf("ȫ��Ϊ��%0.1fkm\n", length);
			}
			free(S.elem);
			free(S1.elem);
			system("pause");
			return OK;
		}
	}
}
//��ȱ����������а������辰��ĵ�·
void DFS_A(Map M, SqStack& S, int start, int n, int* index,SqStack& S1,float& length) {
	int i, e,j,k,top;
	//topָ���·�����ջ��,ջS������ʱ��õ�·����S1���浱ǰ��õİ����������辰������·��
	float l=0;
	AdjVexNodeP p,q;
	Push_Sq(S, start);//��ѡ�������ջ
	M.tags[start] = VISITED;//���Ϊ������
	for (i = FirstAdjVex(M, start, p); i >= 0; i = NextAdjVex(M, start, p)) {
		if (M.tags[i] == UNVISITED) DFS_A(M, S, i, n, index, S1, length);
		//����ڽӶ���iδ�����ʣ���iΪ������������ȱ������ݹ��������Ϊ����������
	}
	for (j = 0; j < n; j++) {//�ҵ�·�����жϸ�·���Ƿ���������ȫ������
		for (k = 0; k < S.top; k++) {
			if (S.elem[k] == index[j])break;
		}
		if (k == S.top)break;
	}
	if (j < n) {//·��û�а��������ȫ������
		Pop_Sq(S, e);
		M.tags[e] = UNVISITED;
	}
	else {//·�����������ȫ������
		for (k = S.top - 1; k >= 0; k--) {//��·������·�������������辰����ֹͣ
			for (j = 0; j < n; j++) {
				if (S.elem[k] == index[j]) {
					top = k + 1;
					break;
				}
			}
			if (j < n)break;
		}
		//����򻯺��·������
		for (j = 0; j < top-1; j++) {
			q = M.spots[S.elem[j]].firstArc;
			while (q != NULL) {
				if (q->adjvex == S.elem[j + 1]) {
					l += q->length;
					break;
				}
				else q = q->nextArc;
			}
		}
		//�жϵ�ǰ���·���Ƿ��֮ǰ��õ�·������
		if (l < length) {//������̣���������·��
			for (j = 0; j < top; j++)S1.elem[j] = S.elem[j];
			S1.top = top;
			length = l;
		}
		Pop_Sq(S, e);//��δ��·�����յ��ջ�����Ϊδ������
		M.tags[e] = UNVISITED;
	}
}
//��ʼ��ջ
Status InitStack_Sq(SqStack& S, int size) {
	S.elem = (int*)malloc(size * sizeof(int));
	if (NULL == S.elem)return OVERFLOW;
	S.top = 0;
	S.size = size;
	return OK;
}
//��ջ
Status Push_Sq(SqStack& S, int e) {
	if (S.top >= S.size)return ERROR;
	else {
		S.elem[S.top++] = e;
		return OK;
	}
}
//��ջ
Status Pop_Sq(SqStack& S, int& e) {
	if (S.top == 0)return ERROR;
	else {
		e = S.elem[--S.top];
		return OK;
	}
}
//��ӡջ�����·��
void PrintStack(SqStack S, Map M) {
	int i;
	AdjVexNodeP p;
	for (i = 0; i < S.top; i++)
	{
		if (i < S.top - 1) {
			p = WayInfo(M, S.elem[i], S.elem[i + 1]);
			printf("%s (%s) ", M.spots[S.elem[i]].data.name, p->type);
		}
		else printf("%s", M.spots[S.elem[i]].data.name);
	}
	printf("\n");
}