#include"У԰������ѯ.h"
void main() {
	Map M;
	InitMap(M);
	while (1) {
	printf("��ӭ�����㶫��ҵ��ѧ��������ϵͳ�ṩ���²�����\n");
	printf("1.���⾰�������Ϣ�Ĳ�ѯ\n");
	printf("2.��ѯÿ��������ڽӾ�����Ϣ\n");
	printf("3.�����·�����Ϣ�Ĳ�ѯ\n");
	printf("4.��ѯ������������֮���һ����̵ļ�·��\n");
	printf("5.��ѯ������������֮������м�·��\n");
	printf("6.��ѯ;�������������·��\n");
	printf("7.����͵�·���޸�����\n");
	printf("8.У԰����ͼ\n");
	printf("9.�˳�����\n");
		int a=-1;
		printf("���������ָ�������ż��ɣ�:");
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
		case 9:printf("�˳�����"); return;
		default:printf("������Ч������������\n"); system("pause");
		}
		system("cls");
	}
}

