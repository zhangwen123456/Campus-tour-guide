#include "校园导游咨询.h"
//查找顶点
int LocateVex(Map M, char* v) {
	int i;
	for (i = 0; i < M.n; i++) {
		if (strcmp(v, M.spots[i].data.name) == 0)return i;
	}
	return -1;
}
//打印地图
void PrintMap(Map M) {
	int i;
	AdjVexNodeP p;
	for (i = 0; i < M.n; i++) {
		printf("%d %s", i, M.spots[i].data.name);
		p = M.spots[i].firstArc;
		while (p != NULL) {
			printf("  %s(长度：%0.1fkm)", M.spots[p->adjvex].data.name, p->length);
			p = p->nextArc;
		}
		printf("\n");
	}
}
//打印景点
void PrintSpots(Map M) {
	int i;
	for (i = 0; i < M.n; i++) {
		printf("%d.%s\n", i, M.spots[i].data.name);
	}
}
//创建地图
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
		M.spots[i].data = vexs[i];//添加顶点信息
		M.spots[i].firstArc = NULL;
	}
	for (k = 0; k < e; k++) {//添加边
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
//初始化地图
void InitMap(Map& M) {//初始化数据
	static char a[10][20] = {"正门","图书馆","教学楼","田径场","体育馆","网球场","一饭",
		"二饭","三饭","四饭"};
	static char b[10][50] = {"构造宏伟","学生自习的地方","学生上课的地方","学生跑步的地方",
		"有羽毛球场，游泳场等运动场所","有多个网球场，师生打网球的地方","东区饭堂，师生就餐的地方",
		"有很多餐饮门店，装修漂亮","西区饭堂，师生就餐的地方","饭菜品种多样，价格适中"};
	static char w[14][6][20] = {
		{"正门","图书馆","北","南","人行道","A"},
		{"图书馆","教学楼","东北","西南","人行道","B"},
		{"图书馆","田径场","西北","东南","车道","A"},
		{"教学楼","田径场","西","东","人行道","A"},
		{"田径场","体育馆","西","东","人行道","A"},
		{"体育馆","网球场","西","东","车道","D"},
		{"教学楼","一饭","东北","西南","车道","C"},
		{"教学楼","二饭","西北","东南","人行道","D"},
		{"田径场","二饭","东北","西南","车道","C"},
		{"体育馆","三饭","西北","东南","车道","C"},
		{"网球场","三饭","东北","西南","人行道","A"},
		{"一饭","二饭","西","东","车道","A"},
		{"二饭","三饭","西","东","人行道","B"},
		{"三饭","四饭","西","东","人行道","A"}
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
//插入边
Status AddArc(Map& M, int start, int end, float length, char* dirS,char* dirE,char*type,
	char* level) {
	AdjVexNodeP p;
	if (start < 0 || start >= M.n || end < 0 || end >= M.n)return ERROR;//start或end顶点不存在
	p = WayInfo(M, start, end);
	if (p != NULL)return ERROR;//判断该边是否存在
	p = (AdjVexNode*)malloc(sizeof(AdjVexNode));
	if (NULL == p)return OVERFLOW;
	p->adjvex = end;//往邻接表中添加边的信息
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
//求k顶点的第一个邻接顶点
int FirstAdjVex(Map M, int k, AdjVexNodeP& p) {
	if (k < 0 || k >= M.n)return -1;
	p = M.spots[k].firstArc;
	if (p != NULL)return p->adjvex;
	else return -1;
}
//求下一个邻接顶点
int NextAdjVex(Map M,int k,AdjVexNodeP& p) {
	if (k < 0 || k >= M.n)return -1;
	if (NULL == p)return -1;
	p = p->nextArc;
	if (p!=NULL)return p->adjvex;
	else return -1;
}
//道路信息
AdjVexNodeP WayInfo(Map M, int start, int end) {
	AdjVexNodeP p;
	p = M.spots[start].firstArc;
	while (p != NULL) {
		if (p->adjvex == end)break;
		else p = p->nextArc;
	}
	return p;//返回指定起点和终点对应的边，如果不存在该边，返回NULL
}
//查询景点信息
Status InquireSpotInfo(Map M) {
	PrintSpots(M);
	printf("请输入要查询的景点序号：");
	int i=-1;
	while (1) {
		scanf_s("%d", &i);
		getchar();//用于存放不正确的输入
		if (i < 0 || i >= M.n)printf("输入无效请重新输入：");
		else
		{
			printf("景点名：%s\n简介：%s\n", M.spots[i].data.name, M.spots[i].data.intro);
			system("pause");
			return OK;
		}
	}
}
//查询邻接顶点信息
Status InquireAdjSpots(Map M) {
	PrintMap(M);
	printf("请输入要查询的景点序号：");
	int i=-1,j;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("输入无效请重新输入：");
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
//查询道路信息
Status InquireWayInfo(Map M) {
	int start = -1, end = -1;
	AdjVexNodeP p;
	PrintMap(M);
	printf("请输入要查询的道路的起点和终点（中间用空格隔开）:");
	while (1) {
		scanf_s("%d %d", &start, &end);
		getchar();
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("输入无效，请重新输入：");
		else {
			p = WayInfo(M, start, end);//p指向所查询的边，如果边不存在，p==NULL
			if (p == NULL)printf("不存在从%s到%s的道路\n", M.spots[start].data.name,
				M.spots[end].data.name);
			else printf("%s->%s\n方向：%s\n长度：%0.1fkm\n道路类型：%s\n沿途景色等级：%s\n",
				M.spots[start].data.name, M.spots[end].data.name, p->dir, p->length, p->type,
				p->level);
			system("pause");
			return OK;
		}
	}
}
//修改景点和道路
void Modify(Map& M) {
	while (1) {
	PrintMap(M);
	printf("1.增加景点\n");
	printf("2.删除景点\n");
	printf("3.修改景点信息\n");
	printf("4.增加道路\n");
	printf("5.删除道路\n");
	printf("6.修改道路信息\n");
	printf("7.退出修改\n");
	printf("请输入要执行的操作：");
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
		default:printf("输入无效，请重新输入\n"); system("pause");
		}
		system("cls");
	}
}
//增加顶点
Status AddSpot(Map& M) {
	int i;
	ScenicSpot* newbase;
	newbase = (ScenicSpot*)realloc(M.spots, (M.n + 1) * sizeof(ScenicSpot));
	if (newbase == NULL)return OVERFLOW;
	char* s1 = (char*)malloc(sizeof(char) * MAX);
	char* s2 = (char*)malloc(sizeof(char) * MAX);
	if (s1 == NULL || s2 == NULL)return OVERFLOW;
	M.spots = newbase;
	printf("请输入景点名：");
	while (1) {
		scanf_s("%s", s1, MAX);
		for (i = 0; i < M.n; i++) {
			if (strcmp(s1, M.spots[i].data.name) == 0) {//通过比较景点名判断是否已存在该景点
				printf("该景点已存在，请重新输入:");
				break;
			}
		}
		if (i == M.n) {//若该景点不存在，录入景点信息
			M.spots[M.n].data.name = s1;
			printf("请输入景点的简介：");
			scanf_s("%s", s2, MAX);
			M.spots[M.n].data.intro = s2;
			M.spots[M.n].firstArc = NULL;
			M.n++;//景点数增加
			printf("增添成功！\n");
			system("pause");
			return OK;
		}
	}
}
//删除顶点
Status RemoveSpot(Map& M) {
	PrintSpots(M);
	printf("请输入要删除的景点序号：");
	int i=-1, j;
	AdjVexNodeP p, q, pre;
	ScenicSpot* S;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("输入无效，请重新输入：");
		else {
			for (j = 0; j < M.n; j++) {//查找与删除顶点相连的边，删除这些边
				pre = p = M.spots[j].firstArc;//p指向要删除的边，pre指向p的前驱结点
				if (j != i) {
					while (p) {
						if (p->adjvex > i) {//结点位序大于要删除顶点序号的顶点需要减1调整序号
							p->adjvex--;
							pre = p;
							p = p->nextArc;
						}
						else if (p->adjvex == i) {//删除位序为i的结点
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
					while (p) {//将顶点数组中位序为i的顶点的邻接链表的结点一一删除
						M.e--;//每删一个结点，边的数目减1
						q = p;
						p = p->nextArc;
						free(q);
					}
				}
			}
				M.spots[i].firstArc = NULL;
				S = (ScenicSpot*)malloc((M.n - 1) * sizeof(ScenicSpot));
				if (S == NULL)return OVERFLOW;
				for (j = 0; j < M.n - 1; j++) {//将除了i顶点后剩下的顶点复制到另一个顶点数组中
					if (j < i)S[j] = M.spots[j];
					else
						S[j] = M.spots[j + 1];
				}
				free(M.spots);//释放原顶点数组空间
				M.spots = S;
				M.n--;//顶点数目减1
				printf("删除成功！\n");
				system("pause");
				return OK;
		}

	}
}
//增加道路
Status AddWay(Map& M) {
	PrintMap(M);
	printf("请输入要增加道路的起点和终点序号（中间用空格隔开）:");
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
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("输入无效，请重新输入：");
		else {
			p = WayInfo(M, start, end);
			if (p != NULL)printf("失败，道路已存在！\n请重新输入道路的起点和终点序号：");
			//判断要增加的路径是否已存在
			else {
				printf("请输入道路长度（单位：km）：");
				while (1) {
					scanf_s("%f", &length);
					getchar();
					if (length <= 0)printf("输入无效，请重新输入：");
					else {
						printf("请输入起点相对于终点的方向：");
						scanf_s("%s", Dir1, MAX);
						printf("请输入终点相对于起点的方向：");
						scanf_s("%s", Dir2, MAX);
						printf("请输入道路类别：");
						scanf_s("%s", Type, MAX);
						printf("请输入沿途景色等级：");
						scanf_s("%s", Level, MAX);
						AddArc(M, start, end, length, Dir1, Dir2, Type, Level);
						//往邻接表中添加增添的边的信息
						printf("增加道路成功！\n");
						system("pause");
						return OK;
					}
				}
			}
		}
	}
}
//删除道路
Status RemoveWay(Map& M) {
	PrintMap(M);
	printf("请输入要删除道路的起点和终点序号（中间用空格隔开）:");
	int start=-1, end=-1;
	AdjVexNodeP p,pre;//p指向被删边的的结点，pre指向p的前驱结点
	while (1) {
		scanf_s("%d %d", &start, &end);
		getchar();
		if (start < 0 || start >= M.n || end < 0 || end >= M.n)printf("输入无效，请重新输入：");
		else {
			pre=p = M.spots[start].firstArc;
			while (p != NULL) {
				if (p->adjvex == end)break;
				else {
					pre = p;
					p = p->nextArc;
				}
			}
			if (p == NULL) {//不存在该边
				printf("失败，道路不存在!\n请重新输入要删除道路的起点和终点序号：");
			}
			else {
				if (p == pre) M.spots[start].firstArc = p->nextArc;
				else pre->nextArc = p->nextArc;
				free(p);//删除邻接链表的结点
				pre=p = M.spots[end].firstArc;
				while (p != NULL) //删除终点邻接链表中边对应的结点
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
				printf("删除成功！\n");
				system("pause");
				return OK;
			}
		}
	}
}
//修改景点信息
Status ModifySpotInfo(Map& M) {
	PrintSpots(M);
	printf("请输入要修改的景点序号：");
	int i=-1,j=-1;
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("输入无效请重新输入：");
		else {
			while (1) {
				system("cls");
				printf("1.修改景点名\n");
				printf("2.修改景点的简介\n");
				printf("3.修改景点名和景点的简介\n");
				printf("4.退出\n");
				printf("请输入要执行的操作：");
				scanf_s("%d", &j);
				getchar();
				system("cls");
				switch (j) {
				case 1: {
					printf("请输入新的景点名：");
					scanf_s("%s", M.spots[i].data.name, MAX);
					printf("修改成功！\n");
					break;
				}
				case 2: {
					printf("请输入新的简介：");
					scanf_s("%s", M.spots[i].data.intro, MAX);
					printf("修改成功！\n");
					break;
				}
				case 3: {
					printf("请输入新的景点名：");
					scanf_s("%s", M.spots[i].data.name, MAX);
					printf("请输入新的简介：");
					scanf_s("%s", M.spots[i].data.intro, MAX);
					printf("修改成功！\n");
					break;
				}
				case 4:return OK;
				default:printf("输入无效请重新输入\n");
				}
				system("pause");
			}
		}
	}
}
//修改道路信息
Status ModifyWayInfo(Map& M) {
	PrintMap(M);
	printf("输入要修改道路的起点和终点序号（中间用空格隔开）：");
	int s=-1, e=-1;//s为起点位序，e为终点位序
	float l=-1;//道路长度
	AdjVexNodeP p, q;
	while (1) {
		scanf_s("%d %d", &s, &e);
		getchar();
		if (s < 0 || s >= M.n || e < 0 || e >= M.n)printf("输入无效，请重新输入：");
		else {
			p = WayInfo(M, s, e);
			if (p == NULL)printf("不存在该道路请重新输入：");//判断边是否存在
			else {
				printf("请输入新的道路长度（单位：km）:");
				while (1) {
					scanf_s("%f", &l);
					getchar();
					if (l <= 0)printf("输入无效，请重新输入：");
					else {
						q = WayInfo(M, e, s);//输入新的信息
						printf("请输入起点相对于终点的方向：");
						scanf_s("%s", p->dir, MAX);
						printf("请输入终点相对于起点的方向：");
						scanf_s("%s", q->dir, MAX);
						printf("请输入新的道路类别：");
						scanf_s("%s", p->type, MAX);
						printf("请输入沿途景色等级：");
						scanf_s("%s", p->level, MAX);
						q->type = p->type;
						q->level = p->level;
						p->length = l;
						q->length = l;
						printf("修改成功！");
						system("pause");
						return OK;
					}
				}
			}
		}

	}
}
//查询最短路径
void InquireShortestPath(Map M) {
	PrintMap(M);
	int i = -1, j = -1;
	DistInfo* Dist;
	printf("请输入起点序号：");
	while (1) {
		scanf_s("%d", &i);
		getchar();
		if (i < 0 || i >= M.n)printf("输入无效，请重新输入：");
		else {
			printf("请输入终点序号：");
			while (1) {
				scanf_s("%d", &j);
				getchar();
				if (j < 0 || j >= M.n)printf("输入无效，请重新输入：");
				else {
					Dijkstra(M, i, Dist);
					//通过迪杰斯特拉算法得到i顶点到其他所由顶点的最短路径,最短路径长度和最短路径上该顶点的前驱顶点的位序存放在Dist指向的数组中
					if (Dist[j].lowcost == INFINITY)printf("不存在从%d到%d的通路", i, j);
					//若图中存在非连通分量，可能不存在起点到终点的通路
					else {
						printf("从%s到%s的最短路径为：\n", M.spots[i].data.name, M.spots[j].data.name);
						Outputpath(M, Dist, j);
						printf("\n全长为：%0.1fkm", Dist[j].lowcost);
					}
					printf("\n");
					system("pause");
					return;
				}
			}
		}
	}
}
//迪杰斯特拉算法
Status Dijkstra(Map M, int i, DistInfo*& Dist) {
	int j, m, k;
	float min;
	AdjVexNodeP p;
	Dist = (DistInfo*)malloc(M.n * sizeof(DistInfo));
	if (Dist == NULL)return OVERFLOW;
	for (j = 0; j < M.n; j++) {//初始化
		Dist[j].lowcost = INFINITY;
		M.tags[j] = UNSELECTED;//所有顶点均不在集合U中
	}
	for (p = M.spots[i].firstArc; p != NULL; p = p->nextArc) {
		//源点i引出的所有弧的信息存入Dist
		Dist[p->adjvex].pre = i;
		Dist[p->adjvex].lowcost = p->length;
	}
	Dist[i].pre = -1;
	Dist[i].lowcost = 0;//源点i信息存入Dist
	M.tags[i] = SELECTED;//初始集合U仅含源点i
	for (m = 1; m < M.n; m++) {//按路径长度升序，依次求源点到其他顶点的最短路径
		min = INFINITY;
		k = 0;
		for (j = 0; j < M.n; j++)
			if (0 == M.tags[j] && Dist[j].lowcost < min) {
				k = j;
				min = Dist[k].lowcost;
			}//k代表当前求得最短路径的k顶点
		M.tags[k] = SELECTED;//将k顶点加入集合U
		for (p = M.spots[k].firstArc; p != NULL; p = p->nextArc) {//更新Dist数组
			j = p->adjvex;
			if (UNSELECTED == M.tags[j] && Dist[k].lowcost + p->length < Dist[j].lowcost) {
				//若j顶点经过k顶点到i顶点的路径更短，更新路径
				Dist[j].lowcost = Dist[k].lowcost + p->length;
				Dist[j].pre = k;
			}
		}
	}
	return OK;
}
//输出源点到k顶点的最短路径
void Outputpath(Map M, DistInfo* Dist, int k) {
	//沿Dist数组的pre域，递归得到源点到k顶点的最短路径
	if (-1 == k)return;
	AdjVexNodeP p;
	Outputpath(M, Dist, Dist[k].pre);//逆向递归获取路径上的顶点
	if (Dist[k].pre != -1) {
		p = WayInfo(M, Dist[k].pre, k);
		if (p != NULL)printf(" (%s%0.1fkm(%s)) %s", p->dir, p->length, p->type, M.spots[k].data.name);
	//正向输出当前路径上的顶点
	}
	else printf("%s", M.spots[k].data.name);
}

//查询两个景点间所有道路
void InquireAllPaths(Map M) {
	int t=-1;
	char type[MAX];
	while (1) {
		printf("1.查询人行道\n");
		printf("2.查询车道\n");
		printf("3.查询任意道路（含人行道和车道）\n");
		printf("4.退出\n");
		printf("请输入要执行的操作：");
		while (1) {
			scanf_s("%d", &t);
			getchar();
			if (t <= 0 || t > 4)printf("输入无效，请重新输入：");
			else {
				system("cls");
				switch (t) {//判断要查询的道路类别，将type置为相应道路类别
				case 1:strcpy_s(type, "人行道"); break;
				case 2:strcpy_s(type, "车道"); break;
				case 3:strcpy_s(type, "任意路径"); break;
				case 4:return;
				}
				AllPaths(M, type);
				break;
			}
		}
		system("cls");
	}
}
//查询指定道路类别的所有道路
Status AllPaths(Map M,char* type) {
	int start = -1, end = -1, found = 0,i;
	//found == 1代表找到start到end的路径,found == 0表示没有找到start到end的路径
	SqStack S;
	InitStack_Sq(S, M.n);
	PrintMap(M);
	printf("请输入起点序号：");
	while (1) {
		scanf_s("%d", &start);
		getchar();
		if (start < 0 || start >= M.n)printf("输入无效，请重新输入：");
		else {
			printf("请输入终点序号：");
			while (1) {
				scanf_s("%d", &end);
				getchar();
				if (end < 0 || end >= M.n)printf("输入无效，请重新输入：");
				else {
					for (i = 0; i < M.n; i++)M.tags[i] =UNVISITED;//初始化标记数组
					DFS_T(M, S, start, end, found, type);
					//通过深度遍历得到从顶点start到顶点end的所有路径
					if (found == 0)printf("不存在从%s到%s的%s\n", M.spots[start].data.name, M.spots[end].data.name, type);
					free(S.elem);
					system("pause");
					return OK;
				}
			}
		}
	}
}
//深度遍历实现查询所有道路
void DFS_T(Map M, SqStack& S, int start, int end, int& found, char* type) {
	//栈S用于暂时存放访问到的路径,found标记是否找到通路，type表示查找的道路类型
	int i, e;
	AdjVexNodeP p;
	Push_Sq(S, start);//将起点入栈
	M.tags[start] = VISITED;
	if (start == end) {//递归结束条件，起点等于终点
		PrintStack(S, M);//打印栈中储存的路径
		Pop_Sq(S, e);//将终点出栈，且标记为未被访问
		M.tags[e] = UNVISITED;
		found = 1;//found置为1，表示找到了从start到found的路径
		return;
	}
	for (i = FirstAdjVex(M, start, p); i >= 0; i = NextAdjVex(M, start, p)) {
		//求起点的邻接顶点i
		if (strcmp(type, "任意路径") == 0) {
			//若i顶点未被访问，以i顶点作为起点进行深度优先遍历
			if (M.tags[i] == UNVISITED)DFS_T(M, S, i, end, found, type);

		}
		else {
		//若所求为人行道或者车道时，需满足i顶点未被访问且道路类型吻合才能以i为起点进行深度优先遍历
			if (M.tags[i] == UNVISITED && (strcmp(p->type, type) == 0))
				DFS_T(M, S, i, end, found, type);
		}
	}
	Pop_Sq(S, e);//遍历完成后，将当前求得路径终点出栈
	M.tags[e] = UNVISITED;//把该点标记为未被访问
}
//查询经过多个景点的最佳（短）路径
Status BestPath(Map M) {
	PrintMap(M);
	int n = -1,i,temp=-1;
	float length = INFINITY;//初始化
	SqStack S,S1;
	InitStack_Sq(S, M.n);
	InitStack_Sq(S1, M.n);
	printf("请输入要经过的景点数：");
	while (1) {
		scanf_s("%d", &n);
		getchar();
		if (n <= 0 || n > M.n)printf("输入无效，请重新输入：");
		else {
			int* index = (int*)malloc(n*sizeof(int));
			if (NULL == index)return OVERFLOW;
			for (i = 0; i < n; i++) {
				printf("请输入要经过的第%d个景点序号：",i+1);
				while (1) {
					scanf_s("%d", &temp);
					getchar();
					if (temp < 0 || temp >= M.n)printf("输入无效，请重新输入：");
					else {
						index[i] = temp;//index数组储存要经过的景点序号
						break;
					}
				}
			}
			for (i = 0; i < M.n; i++)M.tags[i] = UNVISITED;//初始化
			for (i = 0; i < n; i++) {
				//以每个要经过的顶点作为起点进行深度优先遍历，以求得最佳访问路径
				DFS_A(M, S, index[i], n, index, S1, length);
			}
			if (length == INFINITY)printf("不存在能通过全部所需景点的最佳路径！\n");

			else {
				printf("最佳访问路线为：");
				PrintStack(S1, M);//打印最佳路径
				printf("全长为：%0.1fkm\n", length);
			}
			free(S.elem);
			free(S1.elem);
			system("pause");
			return OK;
		}
	}
}
//深度遍历查找所有包含所需景点的道路
void DFS_A(Map M, SqStack& S, int start, int n, int* index,SqStack& S1,float& length) {
	int i, e,j,k,top;
	//top指向简化路径后的栈顶,栈S储存暂时求得的路径，S1储存当前求得的包含所有所需景点的最佳路径
	float l=0;
	AdjVexNodeP p,q;
	Push_Sq(S, start);//将选定起点入栈
	M.tags[start] = VISITED;//标记为被访问
	for (i = FirstAdjVex(M, start, p); i >= 0; i = NextAdjVex(M, start, p)) {
		if (M.tags[i] == UNVISITED) DFS_A(M, S, i, n, index, S1, length);
		//如果邻接顶点i未被访问，以i为起点进行深度优先遍历，递归结束条件为遍历到最深
	}
	for (j = 0; j < n; j++) {//找到路径后，判断该路径是否包含所需的全部景点
		for (k = 0; k < S.top; k++) {
			if (S.elem[k] == index[j])break;
		}
		if (k == S.top)break;
	}
	if (j < n) {//路径没有包含所需的全部景点
		Pop_Sq(S, e);
		M.tags[e] = UNVISITED;
	}
	else {//路径包含所需的全部景点
		for (k = S.top - 1; k >= 0; k--) {//简化路径，即路径经过所有所需景点后就停止
			for (j = 0; j < n; j++) {
				if (S.elem[k] == index[j]) {
					top = k + 1;
					break;
				}
			}
			if (j < n)break;
		}
		//计算简化后的路径长度
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
		//判断当前求得路径是否比之前求得的路径更短
		if (l < length) {//如果更短，则更新最短路径
			for (j = 0; j < top; j++)S1.elem[j] = S.elem[j];
			S1.top = top;
			length = l;
		}
		Pop_Sq(S, e);//将未简化路径的终点出栈，标记为未被访问
		M.tags[e] = UNVISITED;
	}
}
//初始化栈
Status InitStack_Sq(SqStack& S, int size) {
	S.elem = (int*)malloc(size * sizeof(int));
	if (NULL == S.elem)return OVERFLOW;
	S.top = 0;
	S.size = size;
	return OK;
}
//入栈
Status Push_Sq(SqStack& S, int e) {
	if (S.top >= S.size)return ERROR;
	else {
		S.elem[S.top++] = e;
		return OK;
	}
}
//出栈
Status Pop_Sq(SqStack& S, int& e) {
	if (S.top == 0)return ERROR;
	else {
		e = S.elem[--S.top];
		return OK;
	}
}
//打印栈储存的路径
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