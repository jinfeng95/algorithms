/*
    Project: cycle_left_2010(顺序表-循环左移)
    Date:    2018/09/18
    Author:  Frank Yu
	王道18页，2010年计算机联考真题 n(n>1)个整数放到一维数组R中，将R中保存的顺序序列循环左移p(0<p<n)个位置，
	即R中数据由(X0,X1...Xn-1)变换为(Xp,Xp+1...Xn-1,X0,...Xp-1)
	CycleLeft(SqList &L,int p) 参数：顺序表（数组）L，左移个数p 功能：循环左移p个位置 时间复杂度：O(n) 空间复杂度：O(p)
	思想:动态创建大小为p的辅助数组S，将R中前p个整数一次暂存在S中，同时将R中后n-p个整数左移，然后将S中的p个数，
	依次放到R的后续单元中。

	注意，本程序的L就相当于R。 L.length就相当于n
	*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<iostream>
#define MaxSize 100
#define ElemType int
#define Status int
using namespace std;
//顺序表数据结构
typedef struct
{
    ElemType data[MaxSize];//顺序表元素
    int length;            //顺序表当前长度
}SqList;
//***************************基本操作函数*******************************//
//初始化表函数，构造一个空的顺序表
Status InitList(SqList &L)
{
    memset(L.data,0,sizeof(L));//初始化数据为0
    L.length=0;                //初始化长度为0
    return 0;
}
//插入函数 位置i插入数据 i及之后元素后移  1=<i<=length+1
bool InsertList(SqList &L,int i,ElemType e)
{
    if(i<1||i>L.length+1) //判断位置是否有效
    {
        printf("位置无效！！！\n");
        return false;
    }
    if(L.length>=MaxSize)//判断存储空间是否已满
    {
        printf("当前存储空间已满！！！\n");
        return false;
    }
    for(int j=L.length;j>=i;j--)//位置i及之后元素后移
    {
        L.data[j]=L.data[j-1];
    }
    L.data[i-1]=e;
    L.length++;
    return true;
}
//删除函数 删除位置i的元素 i之后的元素依次前移
bool  ListDelete(SqList &L,int i)
{
    if(i<1||i>L.length)
    {
        printf("位置无效！！！\n");
        return false;
    }
    for(int j=i;j<=L.length-1;j++)//位置i之后元素依次前移覆盖
    {
        L.data[j-1]=L.data[j];
    }
    L.length--;
    return true;
}
//查找函数 按位置从小到大查找第一个值等于e的元素 并返回位置
int LocateElem(SqList L,ElemType e)
{
    for(int i=0;i<L.length;i++)//从低位置查找
    {
        if(L.data[i]==e)
            return i+1;
    }
    return 0;

}
//循环左移函数
bool CycleLeft(SqList &L,int p)
{
    if(p<0||p>L.length)return false;//p违法
    ElemType* S = new ElemType[p];//动态创建辅助数组S
    for(int i=0;i<p;i++)          //前p个元素保存S中
    {
        S[i]=L.data[i];
    }
    for(int i=p;i<L.length;i++)   //后p个元素依次左移 p个位置
    {
        L.data[i-p]=L.data[i];
    }
    for(int i=0;i<p;i++)        //S中元素放在后面
    {
        L.data[i+L.length-p]=S[i];
    }
    return true;
}
//********************************功能函数*****************************************//
//输出功能函数 按位置从小到大输出顺序表所有元素
void PrintList(SqList L)
{
    printf("当前顺序表所有元素:");
    for(int i=0;i<L.length;i++)
    {
        printf("%d ",L.data[i]);
    }
    printf("\n");
}
//插入功能函数 调用InsertList完成顺序表元素插入 调用PrintList函数显示插入成功后的结果
void Insert(SqList &L)
{
    int place;ElemType e;bool flag;
    printf("请输入要插入的位置(从1开始)及元素:\n");
    scanf("%d%d",&place,&e);
    flag=InsertList(L,place,e);
    if(flag)
    {
        printf("插入成功！！！\n");
        PrintList(L);
    }
}
//删除功能函数 调用ListDelete函数完成顺序表的删除 调用PrintList函数显示插入成功后的结果
void Delete(SqList &L)
{
    int place;bool flag;
    printf("请输入要删除的位置(从1开始):\n");
    scanf("%d",&place);
    flag=ListDelete(L,place);
    if(flag)
    {
        printf("删除成功！！！\n");
        PrintList(L);
    }
}
//查找功能函数 调用LocateElem查找元素
void Search(SqList L)
{
    ElemType e;int flag;
    printf("请输入要查找的值:\n");
    scanf("%d",&e);
    flag=LocateElem(L,e);
    if(flag)
    {
        printf("该元素位置为:%d\n",flag);
    }
    else
        printf("未找到该元素！\n");
}
//2010真题 左移功能实现函数 调用CycleLeft与PrintList
void CycleToLeft(SqList &L)
{
    int p;bool flag;
    printf("请输入要左移多少个(p>0):\n");
    scanf("%d",&p);
    flag=CycleLeft(L,p);
    if(flag)PrintList(L);
    else printf("左移个数非法！！！\n");
}
//菜单
void menu()
{
    printf("********1.插入     2.删除*********\n");
    printf("********3.查找     4.输出*********\n");
    printf("********5.循环左移 6.退出*********\n");
}
int main()
{
    SqList L;int choice;
    InitList(L);
    while(1)
    {
        menu();
        printf("请输入菜单序号：\n");
        scanf("%d",&choice);
        if(6==choice) break;
        switch(choice)
        {
            case 1:Insert(L);break;
            case 2:Delete(L);break;
            case 3:Search(L);break;
            case 4:PrintList(L);break;
            case 5:CycleToLeft(L);break;
            default:printf("输入错误！！！\n");
        }
    }
    return 0;
}

