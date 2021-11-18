/*
Project: Tree_parent(树-双亲表示法)
Date:    2019/02/25
Author:  Frank Yu
基本操作函数：
InitTree(Tree &T) 参数T，树根节点 作用：初始化树，先序递归创建
InsertNode(Tree &T, TElemType node) 插入树的结点 参数：树T,结点node 作用：在双亲数组中插入结点，增加树的结点值
InsertParent(Tree &T, TElemType node1, TElemType node2)//插入双亲数组的双亲域 参数：树T ，结点node1,结点node2
                                                       //作用：使双亲数组中,node2对应的双亲域为node1的下标
GetIndegree(Tree &T, TElemType node)                   //得到某结点入度 参数：树T，结点node 结点不存在返回-1
GetOutdegree(Tree &T, TElemType node)                  //得到某结点出度 参数：树T，结点node 结点不存在返回-1
PreOrder(Tree T)  参数：树T,根节点下标 作用：先序遍历树
PostOrder(Tree T) 参数：树T,根节点下标 作用：后序遍历树
LevelOrder(Tree T)参数：树T            作用：层序遍历树
功能实现函数：
CreateTree(Tree &T) 参数T，树根节点 作用：创建树，调用InsertNode,InsertParent
Traverse(Tree T)    参数T，树根节点 作用：PreOrder InOrder PostOrder LevelOrder遍历树
*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<stack>
#include<queue>
#include<algorithm>
#include<iostream>
#define TElemType char
#define Max 100
using namespace std;
//树的结点数据结构
typedef struct TNode
{
    TElemType data;//数据域
    int parent;    //双亲
}TNode;
//树的数据结构
typedef struct Tree
{

    TNode parent[Max];
    int NodeNum;

}Tree;
//********************************基本操作函数********************************//
//初始化树函数 参数：树T 作用：规定数据域为#，则为空，双亲为-1,则为空
void InitTree(Tree &T)
{
    for (int i=0;i<Max;i++)
    {
        T.parent[i].data = '#';
        T.parent[i].parent = -1;
    }
    T.NodeNum = 0;
}
//插入树的结点 参数：树T,结点node 作用：在双亲数组中插入结点，增加树的结点值
bool InsertNode(Tree &T, TElemType node)
{

    if (node != '#')
    {
        T.parent[T.NodeNum++].data = node;//插入到双亲数组中
        return true;
    }
    return false;
}
//插入双亲数组的双亲域 参数：树T ，结点node1,结点node2
//作用：使双亲数组中,node2对应的双亲域为node1的下标
bool InsertParent(Tree &T, TElemType node1, TElemType node2)
{
    int place1, place2;
    place1 = -1;place2 = -1;
    for (int i=0;i<T.NodeNum;i++)//查找两点是否存在
    {
        if (node1 == T.parent[i].data)place1 = i;
        if (node2 == T.parent[i].data)place2 = i;
    }
    if (place1 != -1 && place2 != -1)//两点均存在
    {
        T.parent[place2].parent = place1;
        return true;
    }
    return false;
}
//得到某结点入度 参数：树T，结点node 结点不存在返回-1
int GetIndegree(Tree &T, TElemType node)
{
    int place = -1;
    for (int i = 0;i<T.NodeNum;i++)
    {
        if (T.parent[i].data == node)place = i;
    }
    if (place!=-1)//结点存在
    {
        if(T.parent[place].parent!=-1)return 1;//双亲只能有一个
        else return 0; //根节点没有双亲，即没有入度
    }
    return -1;
}
//得到某结点出度 参数：树T，结点node 结点不存在返回-1
int GetOutdegree(Tree &T, TElemType node)
{
    int place = -1;
    int outdegree = 0;
    for (int i = 0;i<T.NodeNum;i++)
    {
        if (T.parent[i].data == node)place = i;
    }
    if (place != -1)
    {
        for (int i = 0;i < T.NodeNum;i++)
        {
            if (T.parent[i].parent == place)outdegree++;
        }
        return outdegree;
    }
    return -1;
}
//先序遍历 参数：树T,根节点下标
void PreOrder(Tree T,int i)
{
    if (T.NodeNum != 0)
    {
        cout << T.parent[i].data << " ";
        for(int j=0;j<T.NodeNum;j++)
        {
            if(T.parent[j].parent==i)
                PreOrder(T,j);//按左右先序遍历子树
        }
    }
}
//后序遍历 参数：树T,根节点下标
void PostOrder(Tree T,int i)
{
    if (T.NodeNum != 0)
    {

        for (int j = 0;j<T.NodeNum;j++)
        {
            if (T.parent[j].parent == i)
                PostOrder(T, j);//按左右先序遍历子树
        }
        cout << T.parent[i].data << " ";
    }
}
//层序遍历 参数：树T
void LevelOrder(Tree T)
{
    queue<TNode> q;//借助队列
    if (T.NodeNum!=0)
    {
        TNode temp;//暂存要出队的结点
        q.push(T.parent[0]);//根结点入队
        while (!q.empty())//队列非空
        {
            temp = q.front();
            q.pop();
            cout<<temp.data<<" ";
            for (int j = 0;j<T.NodeNum;j++)
            {
                if (T.parent[T.parent[j].parent].data == temp.data)//当前结点的父节点的数据域与弹出的相同
                    //因为temp没有保存下标，只能按这种方式比较，默认结点名称不同
                    q.push(T.parent[j]);//队列先进先出，先入左孩子
            }
        }
    }
}
//**********************************功能实现函数*****************************//
//创建树，调用InsertNode,InsertParent
void CreateTree(Tree &T)
{
    int nodenum = 0;
    int parent;
    TElemType node,node1,node2;
    printf("请输入树的结点个数:");
    cin >> nodenum;
    parent = nodenum - 1;
    printf("请输入树的结点名称（空格隔开）:");
    while (nodenum--)
    {
        cin >> node;
        InsertNode(T,node);
    }
    printf("请输入树的结点间的双亲关系（一对为一双亲关系，A B表示A为B的双亲）:\n");
    while (parent--)
    {
        cin >> node1>>node2;
        InsertParent(T,node1,node2);
    }
    printf("\n");
}
//入度
void Indegree(Tree &T)
{
    TElemType node;
    int indegree;
    printf("请输入结点名称:\n");
    cin >> node;
    indegree = GetIndegree(T, node);
    if (-1 != indegree)
        cout << "该结点入度为：" << indegree << endl;
    else
        cout << "结点不存在。" << endl;
}
//出度
void Outdegree(Tree &T)
{
    TElemType node;
    int outdegree;
    printf("请输入结点名称:\n");
    cin >> node;
    outdegree = GetOutdegree(T, node);
    if (-1 != outdegree)
        cout << "该结点出度为：" << outdegree << endl;
    else
        cout << "结点不存在。" << endl;
}
//遍历功能函数 调用PreOrder InOrder PostOrder LevelOrder
void Traverse(Tree T)
{
    int choice;
    while (1)
    {
        printf("********1.先序遍历    2.后序遍历*********\n");
        printf("********3.层次遍历    4.返回上一单元*********\n");
        printf("请输入菜单序号：\n");
        scanf("%d", &choice);
        if (4 == choice) break;
        switch (choice)
        {
            case 1: {printf("树先序遍历序列:");PreOrder(T,0);printf("\n");}break;
            case 2: {printf("树后序遍历序列:");PostOrder(T,0);printf("\n");}break;
            case 3: {printf("树层序遍历序列:");LevelOrder(T);printf("\n");}break;
            default:printf("输入错误！！！\n");break;
        }
    }
}
//菜单
void menu()
{
    printf("********1.创建     2.某点入度*********\n");
    printf("********3.某点出度 4.遍历*************\n");
    printf("********5.退出\n");
}
//主函数
int main()
{
    Tree T;
    int choice = 0;
    InitTree(T);
    while (1)
    {
        menu();
        printf("请输入菜单序号：\n");

        scanf("%d", &choice);
        if (5 == choice) break;
        switch (choice)
        {
            case 1:CreateTree(T);break;
            case 2:Indegree(T);break;
            case 3:Outdegree(T);break;
            case 4:Traverse(T);break;
            default:printf("输入错误！！！\n");break;
        }
    }
    return 0;
}

