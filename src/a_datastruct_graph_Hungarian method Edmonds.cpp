/*
Project: Hungarian method Edmonds
Date:    2020/01/02
Author:  Frank Yu
void Init(Graph &G)				   初始化函数，参数：图G，功能：初始化图G
void Print(Graph G)                打印图函数，参数：图G,功能：以矩阵形式打印图，可去除
void PrintP(Graph G)               打印路径函数，参数：图G,功能：打印路径P
void PrintM(Graph G)               打印匹配集合M函数，参数：图G，功能：打印匹配集合M
void Delta()                       对称差函数，参数：无，功能：M与E(P)做对称差
void DFS(Graph G,bool x,int start) 深度遍历函数（递归形式）参数：图G,X点集，开始结点下标start 作用：深度遍历，找可扩路
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
#include<stack>
#include<iterator>
#include<algorithm>
#include<iostream>
#define Vartype string //顶点类型
#define EdgeType int
#define Maxnum 100 //二部图点集最大数量
using namespace std;
//图的数据结构
typedef struct Graph
{
    Vartype X[Maxnum];
    Vartype Y[Maxnum];
    EdgeType Edge[Maxnum][Maxnum];//边表
    int xnum, ynum,edgenum;//顶点数
}Graph;
//M 下标为X下标，值为匹配的Y集合中的元素下标 初始-1
int M[Maxnum];
//M是否饱和X、Y 饱和为True,不饱和为False
bool X[Maxnum],Y[Maxnum];
//可扩路P
vector<int> P;
//邻接点集合与T集合
set<Vartype> NS, T;
//标记是否已遍历过
bool visitedx[Maxnum], visitedy[Maxnum];
//初始化函数
void Init(Graph &G)
{
    memset(G.Edge, 0, sizeof(G.Edge));
    cout << "请输入X、Y顶点集个数:" << endl;
    cin >> G.xnum >> G.ynum;
    Vartype temp;
    cout << "请输入X顶点集顶点名称:" << endl;
    for (int i = 0; i < G.xnum; i++)
    {
        cin >> temp;
        G.X[i] = temp;
    }
    //for (int i = 0; i < G.xnum; i++) cout << G.X[i] << '\t' << endl;
    cout << "请输入Y顶点集顶点名称:" << endl;
    for (int i = 0; i < G.ynum; i++)
    {
        cin >> temp;
        G.Y[i] = temp;
    }
    //for (int i = 0; i < G.xnum; i++) cout << G.X[i] << '\t' << endl;
    cout << "请输入边数：" << endl;
    cin >> G.edgenum;
    cout << "请输入边，空格分隔（例如： x y）：" << endl;
    Vartype x, y;
    for (int i = 0; i < G.edgenum; i++)
    {
        cin >> x >> y;
        int p1 = -1,p2 = -1;
        for (int j = 0; j < G.xnum; j++)
            if (!x.compare(G.X[j])) { p1 = j; break; }
        for (int k = 0; k < G.ynum; k++)
            if (!y.compare(G.Y[k])) { p2 = k; break;}
        //cout << p1 << " " << p2;
        if (p1 != -1 && p2 != -1)
        {
            G.Edge[p1][p2] = 1;
        }
        else
        {
            cout << "未找到该边，请检查端点是否输入有误！" << endl;
            break;
        }
    }
}
//打印图函数
void Print(Graph G)
{
    cout << '\t';
    for (int i = 0; i < G.ynum; i++) cout << G.Y[i] << '\t';
    cout << endl;
    for (int i = 0; i < G.xnum; i++)
    {
        cout << G.X[i] << '\t';
        for (int j = 0; j < G.ynum; j++)cout << G.Edge[i][j]<<'\t';
        cout << endl;
    }
}
//输出可扩路
void PrintP(Graph G)
{
    cout << "P:";
    for (int i = 0; i < P.size(); i++)
    {
        if (i % 2 == 0)cout << G.X[P[i]];
        else cout << G.Y[P[i]];
    }
    cout << endl;
}
//输出集合M
void PrintM(Graph G)
{
    bool flag = false;
    cout << "M:{";
    for (int i = 0; i < G.xnum; i++)
    {
        if (M[i] != -1 && !flag) { cout << G.X[i] << G.Y[M[i]]; flag = true; }
        else if (M[i]!=-1&&flag)cout  << ","<< G.X[i] << G.Y[M[i]];
    }
    cout <<"}"<<endl;
}
//集合M与E（P）做对称差
void Delta()
{
    vector<int>::iterator it;
    for (it = P.begin(); it != P.end();it++)
    {
        int x = *it;
        it++;
        int y = *it;
        X[x] = true;
        Y[y] = true;
        M[x] = y;
    }
}
//深度遍历函数（递归形式）参数：图G,X点集开始结点下标start 作用：深度遍历
void DFS(Graph G,bool x,int start)
{
    /*
    cout << "DFS(";
    if (x)cout << "x,";
    else cout << "y,";
    cout << start << ")" << endl;*/
    //X顶点集
    if (x)
    {
        P.push_back(start);
        cout << "当前路:" << endl;
        PrintP(G);
        visitedx[start] = true;
        for (int i = 0; i < G.ynum; i++) if (G.Edge[start][i] == 1)NS.insert(G.Y[i]);
        if (NS.size() == T.size())
        {
            cout << "N(S)==T，没有完美匹配" << endl;
            system("pause");
        }
        for (int i = 0; i < G.ynum; i++)
        {
            //取Y中M - 饱和顶点
            if (G.Edge[start][i] == 1 && !visitedy[i] && Y[i])//是邻接点且未访问 M - 饱和顶点Y[i]
            {
                T.insert(G.Y[i]);
                cout << "取Y中M - 饱和顶点" << G.Y[i] << endl;
                DFS(G,false,i);//递归深度遍历结点集Y
            }
            //Y为M - 不饱和顶点 找到可扩路P 与M做对称差
            if (G.Edge[start][i] == 1 && !visitedy[i] && !Y[i])
            {
                cout << G.Y[i]<< "为M - 不饱和顶点,找到可扩路"  << endl;
                P.push_back(i);
                PrintP(G);
                Delta();
                PrintM(G);
                //返回步骤一
                for (int i = 0; i < G.xnum; i++)
                {
                    memset(visitedx, false, sizeof(visitedx));
                    memset(visitedy, false, sizeof(visitedy));
                    P.clear();
                    NS.clear();
                    T.clear();
                    //取X中M - 不饱和顶点
                    if (!X[i])DFS(G, true, i);
                }
                cout << "找到完美匹配";
                PrintM(G);
                cout << endl;
                system("pause");
            }
        }
        P.pop_back();
        cout << "返回上一层前的路径:" << endl;
        PrintP(G);
        return;//返回至上一层
    }
    else//Y顶点集
    {
        //cout << G.Y[start];
        P.push_back(start);
        cout << "当前路:" << endl;
        PrintP(G);
        visitedy[start] = true;
        for (int j = 0; j < G.xnum; j++)
        {
            if (M[j]==start)//找到Y[start]X[j]属于M
            {
                cout << "存在"<<G.Y[start]<<G.X[j]<<"属于M" << endl;
                DFS(G, true, j);//递归深度遍历结点集X
            }
        }
        P.pop_back();
        cout << "返回上一层前的路径:" << endl;
        PrintP(G);
        return ;//返回至上一层
    }
}
//匈牙利算法
int Hungarian(Graph &G)
{
    int i;
    memset(M, -1, sizeof(M));
    cout << "1.输入初始M  2.M从空集开始" << endl;
    cout << "请选择:";
    cin >> i;
    if (1 == i)
    {
        int num;
        cout << "请输入M中边的数量:" << endl;
        cin >> num;
        cout << "请输入边，空格分隔（例如： x y）：" << endl;
        Vartype x, y;
        for (int i = 0; i < num; i++)
        {
            cin >> x >> y;
            int p1 = -1, p2 = -1;
            for (int j = 0; j < G.xnum; j++)
                if (!x.compare(G.X[j])) { p1 = j; break; }
            for (int k = 0; k < G.ynum; k++)
                if (!y.compare(G.Y[k])) { p2 = k; break; }
            if (p1 != -1 && p2 != -1)
            {
                M[p1] = p2;
                X[p1] = true;
                Y[p2] = true;
            }
            else
            {
                cout << "未找到该边，请检查端点是否输入有误！" << endl;
                break;
            }

        }
    }
    PrintM(G);
    //步骤1 判断M是否饱和所有X元素
    for (int i = 0; i < G.xnum; i++)
    {
        memset(visitedx, false, sizeof(visitedx));
        memset(visitedy, false, sizeof(visitedy));
        P.clear();
        NS.clear();
        T.clear();
        //取X中M - 不饱和顶点
        if (!X[i])DFS(G, true, i);
    }
    cout << "找到完美匹配";
    PrintM(G);
    cout<< endl;
    return 0;
}
//主函数
int main()
{
    Graph G;
    Init(G);
    Print(G);
    Hungarian(G);
    return 0;
}

