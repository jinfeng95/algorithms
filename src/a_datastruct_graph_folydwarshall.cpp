/*
Project: 图-最短路径-Bellman-Ford算法（可含有负权弧）
Date:    2019/10/24
Author:  Frank Yu
基本操作函数:
InitGraph(Graph &G)             初始化函数 参数：图G 作用：初始化图的顶点表，邻接矩阵等
InsertNode(Graph &G,VexType v) 插入点函数 参数：图G,顶点v 作用：在图G中插入顶点v,即改变顶点表
InsertEdge(Graph &G,VexType v,VexType w) 插入弧函数 参数：图G,某弧两端点v和w 作用：在图G两点v,w之间加入弧，即改变邻接矩阵
Adjancent(Graph G,VexType v,VexType w) 判断是否存在弧(v,w)函数 参数：图G，某弧两端点v和w 作用：判断是否存在弧(v,w)
BFS(Graph G, int start) 广度遍历函数 参数：图G,开始结点下标start 作用：宽度遍历
DFS(Graph G, int start) 深度遍历函数（递归形式）参数：图G,开始结点下标start 作用：深度遍历
Dijkstra(Graph G, int v)  最短路径 - Dijkstra算法 参数：图G、源点v
Bellman_Ford(Graph G, int v)  最短路径 - Bellman_Ford算法  参数：图G、源点v 作用：计算不含负圈图的最短路径 返回是否有圈
Floyd_Wallshall(Graph G)    最短路径 - Floyd_Wallshall算法  参数：图G 作用：计算不含负圈图的最短路径 返回是否有圈
功能实现函数：
CreateGraph(Graph &G) 创建图功能实现函数 参数：图G  InsertNode 作用：创建图
BFSTraverse(Graph G)  广度遍历功能实现函数 参数：图G 作用：宽度遍历
DFSTraverse(Graph G)  深度遍历功能实现函数 参数：图G 作用：深度遍历
Shortest_Dijkstra(Graph &G) 调用最短路径-Dijkstra算法 参数：图G、源点v
Shortest_Bellman_Ford(Graph &G) 调用最短路径- - Bellman_Ford算法  参数：图G
Shortest_Floyd_Wallshall(Graph &G) 调用最短路径- - Floyd_Wallshall算法  参数：图G
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<set>
#include<list>
#include<queue>
#include<vector>
#include<map>
#include<iterator>
#include<algorithm>
#include<iostream>
#define MaxVerNum 100 //顶点最大数目值
#define VexType char //顶点数据类型
#define EdgeType int //弧数据类型,有向图时邻接矩阵不对称，有权值时表示权值，没有时1连0不连
#define INF 0x3f3f3f3f//作为最大值
using namespace std;
//图的数据结构
typedef struct Graph
{
    VexType Vex[MaxVerNum];//顶点表
    EdgeType Edge[MaxVerNum][MaxVerNum];//弧表
    int vexnum, arcnum;//顶点数、弧数
}Graph;
//迪杰斯特拉算法全局变量
bool S[MaxVerNum]; //顶点集
int D[MaxVerNum];  //到各个顶点的最短路径
int F_D[MaxVerNum][MaxVerNum];//Floyd的D矩阵 记录最短路径
int Pr[MaxVerNum]; //记录前驱
//*********************************************基本操作函数*****************************************//
//初始化函数 参数：图G 作用：初始化图的顶点表，邻接矩阵等
int P[MaxVerNum][MaxVerNum];//最短路径记录矩阵
void InitGraph(Graph &G)
{
    memset(G.Vex, '#', sizeof(G.Vex));//初始化顶点表
    //初始化弧表
    for (int i = 0; i < MaxVerNum; i++)
        for (int j = 0; j < MaxVerNum; j++)
            G.Edge[i][j] = INF;
    G.arcnum = G.vexnum = 0;          //初始化顶点数、弧数
}
//插入点函数 参数：图G,顶点v 作用：在图G中插入顶点v,即改变顶点表
bool InsertNode(Graph &G, VexType v)
{
    if (G.vexnum < MaxVerNum)
    {
        G.Vex[G.vexnum++] = v;
        return true;
    }
    return false;
}
//插入弧函数 参数：图G,某弧两端点v和w 作用：在图G两点v,w之间加入弧，即改变邻接矩阵
bool InsertEdge(Graph &G, VexType v, VexType w, int weight)
{
    int p1, p2;//v,w两点下标
    p1 = p2 = -1;//初始化
    for (int i = 0; i<G.vexnum; i++)//寻找顶点下标
    {
        if (G.Vex[i] == v)p1 = i;
        if (G.Vex[i] == w)p2 = i;
    }
    if (-1 != p1&&-1 != p2)//两点均可在图中找到
    {
        G.Edge[p1][p2] = weight;//有向图邻接矩阵不对称
        G.arcnum++;
        return true;
    }
    return false;
}
//判断是否存在弧(v,w)函数 参数：图G，某弧两端点v和w 作用：判断是否存在弧(v,w)
bool Adjancent(Graph G, VexType v, VexType w)
{
    int p1, p2;//v,w两点下标
    p1 = p2 = -1;//初始化
    for (int i = 0; i<G.vexnum; i++)//寻找顶点下标
    {
        if (G.Vex[i] == v)p1 = i;
        if (G.Vex[i] == w)p2 = i;
    }
    if (-1 != p1&&-1 != p2)//两点均可在图中找到
    {
        if (G.Edge[p1][p2] == 1)//存在弧
        {
            return true;
        }
        return false;
    }
    return false;
}
bool visited[MaxVerNum];//访问标记数组，用于遍历时的标记
//广度遍历函数 参数：图G,开始结点下标start 作用：宽度遍历
void BFS(Graph G, int start)
{
    queue<int> Q;//辅助队列
    cout << G.Vex[start];//访问结点
    visited[start] = true;
    Q.push(start);//入队
    while (!Q.empty())//队列非空
    {
        int v = Q.front();//得到队头元素
        Q.pop();//出队
        for (int j = 0; j<G.vexnum; j++)//邻接点
        {
            if (G.Edge[v][j] < INF && !visited[j])//是邻接点且未访问
            {
                cout << "->";
                cout << G.Vex[j];//访问结点
                visited[j] = true;
                Q.push(j);//入队
            }
        }
    }//while
    cout << endl;
}
//深度遍历函数（递归形式）参数：图G,开始结点下标start 作用：深度遍历
void DFS(Graph G, int start)
{
    cout << G.Vex[start];//访问
    visited[start] = true;
    for (int j = 0; j < G.vexnum; j++)
    {
        if (G.Edge[start][j] <INF && !visited[j])//是邻接点且未访问
        {
            cout << "->";
            DFS(G, j);//递归深度遍历
        }
    }
}
//最短路径 - Dijkstra算法 参数：图G、源点v3
void Dijkstra(Graph G, int v)
{
    //初始化
    int n = G.vexnum;//n为图的顶点个数
    for (int i = 0; i < n; i++)
    {
        S[i] = false;
        D[i] = G.Edge[v][i];
        if (D[i] < INF)Pr[i] = v; //v与i连接，v为前驱
        else Pr[i] = -1;
    }
    S[v] = true;
    D[v] = 0;
    //初始化结束,求最短路径，并加入S集
    for (int i = 1; i < n; i++)
    {
        int min = INF;
        int temp;
        for (int w = 0; w < n; w++)
            if (!S[w] && D[w] < min) //某点temp未加入s集，且为当前最短路径
            {
                temp = w;
                min = D[w];
            }
        S[temp] = true;
        //更新从源点出发至其余点的最短路径 通过temp
        for (int w = 0; w < n; w++)
            if (!S[w] && D[temp] + G.Edge[temp][w] < D[w])
            {
                D[w] = D[temp] + G.Edge[temp][w];
                Pr[w] = temp;
            }
    }
}
//最短路径 - Bellman_Ford算法  参数：图G、源点v 作用：计算不含负圈图的最短路径 返回是否有圈
bool Bellman_Ford(Graph G, int v)
{
    //初始化
    int n = G.vexnum;//n为图的顶点个数
    for (int i = 0; i < n; i++)
    {
        D[i] = G.Edge[v][i];
        if (D[i] < INF)Pr[i] = v; //v与i连接，v为前驱
        else Pr[i] = -1;
    }
    D[v] = 0;
    //初始化结束，开始双重循环
    for (int i = 2; i<G.vexnum - 1; i++)
        for (int j = 0; j<G.vexnum; j++) //j为源点
            for (int k = 0; k<G.vexnum; k++) //k为终点
                if (D[k] > D[j] + G.Edge[j][k])
                {
                    D[k] = D[j] + G.Edge[j][k];
                    Pr[k] = j;
                }
    //判断是否含有负圈
    bool flag = true;
    for (int j = 0; j<G.vexnum - 1; j++) //j为源点
        for (int k = 0; k<G.vexnum - 1; k++) //k为终点
            if (D[k] > D[j] + G.Edge[j][k])
            {
                flag = false;
                break;
            }
    return flag;
}
//最短路径 - Floyd_Wallshall算法  参数：图G 作用：计算不含负圈图的最短路径 返回是否有圈
bool Floyd_Wallshall(Graph G)
{
    //初始化
    for (int i = 0; i<G.vexnum; i++)
        for (int j = 0; j < G.vexnum; j++)
        {
            if (i == j)F_D[i][j] = 0;
            else F_D[i][j] = G.Edge[i][j];
            P[i][j] = -1;
        }
    //初始化结束，开始迭代
    for(int k=0;k<G.vexnum;k++)
        for (int i = 0; i<G.vexnum; i++)
            for (int j = 0; j<G.vexnum; j++)
                if (F_D[i][j] > F_D[i][k] + F_D[k][j])
                {
                    F_D[i][j] = F_D[i][k] + F_D[k][j];
                    P[i][j] = k;
                }
    bool flag = true;
    for (int i = 0; i < G.vexnum; i++)
        for (int j = 0; j < G.vexnum; j++)
            if (i==j&&F_D[i][j] < 0)
            {
                flag = false;
                break;
            }
    return flag;
}
//输出最短路径
void Path(Graph G, int v)
{
    if (Pr[v] == -1)
        return;
    Path(G, Pr[v]);
    cout << G.Vex[Pr[v]] << "->";
}
// 输出Floyd最短路径 v是终点
void F_Path(Graph G, int v, int w)
{
    cout << "->"<< G.Vex[P[v][w]] ;
    if (P[v][w] == -1)
        return;
    F_Path(G, v,P[v][w]);

}
//**********************************************功能实现函数*****************************************//
//打印图的顶点表
void PrintVex(Graph G)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        cout << G.Vex[i] << " ";
    }
    cout << endl;
}
//打印图的弧矩阵
void PrintEdge(Graph G)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (G.Edge[i][j] == INF)cout << "∞ ";
            else cout << G.Edge[i][j] << " ";
        }
        cout << endl;
    }
}
//创建图功能实现函数 参数：图G  InsertNode 作用：创建图
void CreateGraph(Graph &G)
{
    VexType v, w;
    int vn, an;//顶点数，弧数
    cout << "请输入顶点数目:" << endl;
    cin >> vn;
    cout << "请输入弧数目:" << endl;
    cin >> an;
    cout << "请输入所有顶点名称:" << endl;
    for (int i = 0; i<vn; i++)
    {
        cin >> v;
        if (InsertNode(G, v)) continue;//插入点
        else {
            cout << "输入错误！" << endl; break;
        }
    }
    cout << "请输入所有弧（每行输入起点，终点及权值）:" << endl;
    for (int j = 0; j<an; j++)
    {
        int weight;
        cin >> v >> w >> weight;
        if (InsertEdge(G, v, w, weight)) continue;//插入弧
        else {
            cout << "输入错误！" << endl; break;
        }
    }
    cout << "图的顶点及邻接矩阵：" << endl;
    PrintVex(G);
    PrintEdge(G);
}
//广度遍历功能实现函数 参数：图G 作用：宽度遍历
void BFSTraverse(Graph G)
{
    for (int i = 0; i<MaxVerNum; i++)//初始化访问标记数组
    {
        visited[i] = false;
    }
    for (int i = 0; i < G.vexnum; i++)//对每个连通分量进行遍历
    {
        if (!visited[i])BFS(G, i);
    }
}
//深度遍历功能实现函数 参数：图G 作用：深度遍历
void DFSTraverse(Graph G)
{
    for (int i = 0; i<MaxVerNum; i++)//初始化访问标记数组
    {
        visited[i] = false;
    }
    for (int i = 0; i < G.vexnum; i++)//对每个连通分量进行遍历
    {
        if (!visited[i])
        {
            DFS(G, i); cout << endl;
        }
    }
}
//调用最短路径-Dijkstra算法 参数：图G
void Shortest_Dijkstra(Graph &G)
{
    char vname;
    int v = -1;
    cout << "请输入源点名称:" << endl;
    cin >> vname;
    for (int i = 0; i < G.vexnum; i++)
        if (G.Vex[i] == vname)v = i;
    if (v == -1)
    {
        cout << "没有找到输入点！" << endl;
        return;
    }
    Dijkstra(G, v);
    cout << "目标点" << "\t" << "最短路径值" << "\t" << "最短路径" << endl;
    for (int i = 0; i < G.vexnum; i++)
    {
        if (i != v)
        {
            cout << "  " << G.Vex[i] << "\t" << "        " << D[i] << "\t";
            Path(G, i);
            cout << G.Vex[i] << endl;
        }
    }
}
//调用最短路径- - Bellman_Ford算法  参数：图G
void Shortest_Bellman_Ford(Graph &G)
{
    char vname;
    int v = -1;
    cout << "请输入源点名称:" << endl;
    cin >> vname;
    for (int i = 0; i < G.vexnum; i++)
        if (G.Vex[i] == vname)v = i;
    if (v == -1)
    {
        cout << "没有找到输入点！" << endl;
        return;
    }
    if (Bellman_Ford(G, v))
    {
        cout << "目标点" << "\t" << "最短路径值" << "\t" << "最短路径" << endl;
        for (int i = 0; i < G.vexnum; i++)
        {
            cout << "  " << G.Vex[i] << "\t" << "        " << D[i] << "\t";
            Path(G, i);
            cout << G.Vex[i] << endl;
        }
    }
    else
    {
        cout << "输入的图中含有负圈，不能使用该算法！" << endl;
    }
}
//调用最短路径- - Floyd_Wallshall算法  参数：图G
void Shortest_Floyd_Wallshall(Graph &G)
{
    if (Floyd_Wallshall(G))
    {
        cout << "最短路径值" << "\t" << "最短路径" << endl;
        for (int i = 0; i < G.vexnum; i++)
            for (int j = 0; j < G.vexnum; j++)
            {
                cout << "     "<<F_D[i][j] << "   \t";
                cout << G.Vex[i];
                F_Path(G, i,j);
                cout << G.Vex[j] << endl;
            }
    }
    else
    {
        cout << "输入的图中含有负圈，不能使用该算法！" << endl;
    }
}
//菜单
void menu()
{
    cout << "************1.创建图       2.广度遍历******************" << endl;
    cout << "************3.深度遍历     4.迪杰斯特拉****************" << endl;
    cout << "************5.贝尔曼福特   6.弗洛伊德******************" << endl;
    cout << "************7.退出*************************************" << endl;
}
//主函数
int main()
{
    int choice = 0;
    Graph G;
    InitGraph(G);
    while (1)
    {
        menu();
        printf("请输入菜单序号：\n");
        scanf("%d", &choice);
        if (7 == choice) break;
        switch (choice)
        {
            case 1:CreateGraph(G); break;
            case 2:BFSTraverse(G); break;
            case 3:DFSTraverse(G); break;
            case 4:Shortest_Dijkstra(G); break;
            case 5:Shortest_Bellman_Ford(G); break;
            case 6:Shortest_Floyd_Wallshall(G); break;
            default:printf("输入错误！！！\n"); break;
        }
    }
    return 0;
}

