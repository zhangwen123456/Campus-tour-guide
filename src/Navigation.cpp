#include"校园导游咨询.h"
void main() {
	Map M;
	InitMap(M);
	while (1) {
	printf("欢迎来到广东工业大学，本导游系统提供以下操作：\n");
	printf("1.任意景点相关信息的查询\n");
	printf("2.查询每个景点的邻接景点信息\n");
	printf("3.任意道路相关信息的查询\n");
	printf("4.查询任意两个景点之间的一条最短的简单路径\n");
	printf("5.查询任意两个景点之间的所有简单路径\n");
	printf("6.查询途径多个景点的最短路径\n");
	printf("7.景点和道路的修改扩充\n");
	printf("8.校园仿真图\n");
	printf("9.退出程序\n");
		int a=-1;
		printf("请输入操作指令（输入序号即可）:");
		scanf_s("%d", &a);
		getchar();
		system("cls");
		switch (a) {
		case 1:InquireSpotInfo(M);break;
		case 2:InquireAdjSpots(M); break;
		case 3:InquireWayInfo(M); break;
		case 4:InquireShortestPath(M); break;
		case 5:InquireAllPaths(M); break;
		case 6:BestPath(M); break;
		case 7:Modify(M); break;
		case 8:PrintMap(M);	system("pause"); break;
		case 9:printf("退出程序！"); return;
		default:printf("输入无效，请重新输入\n"); system("pause");
		}
		system("cls");
	}
}

