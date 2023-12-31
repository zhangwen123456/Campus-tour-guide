#pragma once
#include"common.h"
typedef struct AdjVexNode {
	int adjvex;//竹??
	struct AdjVexNode* nextArc;
	float length;//??﹞????
	char* dir;//????
	char* type;//??﹞???????????快????
	char* level;//?????????
}AdjVexNode,*AdjVexNodeP;

typedef struct {
	char* name;//??????
	char* intro;//???
}VexType;//???????
typedef struct {
	VexType data;//???????
	struct AdjVexNode* firstArc;
}ScenicSpot;//????

typedef struct {
	char* start;//???
	char* end;//???
	char* dirS;//start?????????end?????
	char* dirE;//end?????????start?????
	char* type;//??﹞???????????快????
	char* level;//?????????
	float length;//??﹞????
}Way;//??﹞

typedef struct {
	ScenicSpot* spots;
	int n, e;//??????????
	int* tags;//??????????
}Map;//???
typedef struct {
	int pre;//??????﹞??????????????????
	float lowcost;//??????﹞???????
}DistInfo;//V-U?忪?????????﹞?????
typedef struct {
	int* elem;
	int top;
	int size;
}SqStack;//?????????????﹞??
int LocateVex(Map M, char* v);//???????
void PrintMap(Map M);//??????
void PrintSpots(Map M);//???????
Status CreatMap(Map& M, VexType* vexs, int n, Way* ways, int e);//???????
void InitMap(Map& M);//????????
Status AddArc(Map& M, int start, int end, float length, char* dirS, char* dirE, char* type,
char*level);//?????
int FirstAdjVex(Map M, int k, AdjVexNodeP& p);//??k???????????????
int NextAdjVex(Map M, int k, AdjVexNodeP& p);//?????????????
AdjVexNodeP WayInfo(Map M, int start, int end);//??﹞???
Status InquireSpotInfo(Map M);//??????????
Status InquireAdjSpots(Map M);//????????????
Status InquireWayInfo(Map M);//?????﹞???
void Modify(Map& M);//????????﹞
Status  AddSpot(Map& M);//???????
Status RemoveSpot(Map& M);//???????
Status AddWay(Map& M);//?????﹞
Status RemoveWay(Map& M);//?????﹞
Status ModifySpotInfo(Map& M);//?????????
Status ModifyWayInfo(Map& M);//????﹞???
void InquireShortestPath(Map M);//??????﹞??
Status Dijkstra(Map M, int i, DistInfo*& Dist);//??????????
void Outputpath(Map M, DistInfo* Dist, int k);//??????k????????﹞??
void InquireAllPaths(Map M);//???????????????快?﹞
Status AllPaths(Map M, char* type);//????????﹞???????快?﹞
void DFS_T(Map M, SqStack& S, int start, int end,int& found,char* type);//????????????快?﹞
Status BestPath(Map M); //???????﹞
void DFS_A(Map M, SqStack& S,int start, int n, int* index,SqStack& S1,float& length);
//?????????????忘??????魚????﹞
Status InitStack_Sq(SqStack& S, int size);//??????
Status Push_Sq(SqStack& S, int e);//???
Status Pop_Sq(SqStack& S, int& e);//???
void PrintStack(SqStack S, Map M);//?????????﹞??




