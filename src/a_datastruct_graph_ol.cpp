/*
Project: euler_graph
Date:    2020/08/31
Author:  Frank Yu
*/
#include<string>
#include<vector>
#include<unordered_map>
#include<iterator>
#include<algorithm>
#include<iostream>
using namespace std;
//邻接表
unordered_map<string, vector<string>> adj;
void show_adj()
{
    cout << "邻接表:" << endl;
    for (auto p:adj)
    {
        for (auto q:p.second)
        {
            cout << p.first << "->" << q<<endl;
        }
    }
}
//当做栈使用
vector<string> stk;
//深度遍历
void dfs(const string& curr) {
    //找到点 且还有相邻边
    while (adj.count(curr) && adj[curr].size() > 0) {
        string tmp = adj[curr].back();
        adj[curr].pop_back();
        dfs(move(tmp));
    }
    stk.emplace_back(curr);
}
//Hierholzer 算法，根据邻接表，返回欧拉环游/回路
vector<string> Hierholzer(string start,unordered_map<string, vector<string>> adj)
{
    dfs(start);
    reverse(stk.begin(), stk.end());
    return stk;
}
//主函数
int main()
{
    vector<string> a;
    a.emplace_back("b");
    a.emplace_back("c");
    vector<string> b;
    b.emplace_back("a");
    vector<string> c;
    c.emplace_back("b");
    //插入点a(b) 及点a(b)的边 形成邻接表
    adj.emplace(make_pair("a",a));
    adj.emplace(make_pair("b", b));
    adj.emplace(make_pair("c", c));
    show_adj();
    vector<string> e_tour = Hierholzer("a", adj);
    cout << "欧拉迹/通路、欧拉环游/回路:" << endl;
    for (string p:e_tour)
    {
        cout << p << " ";
    }
    cout << endl;
    return 0;
}

