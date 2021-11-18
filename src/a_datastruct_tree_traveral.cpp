/*
Project: 二叉树(BiTree)-北邮2017真题
Date:    2019/01/19
Author:  Frank Yu
题目：输入二叉树的前序和中序遍历结果，输出二叉树后序遍历结果。
输入格式：
第一行为二叉树的前序遍历结果
第二行为二叉树的中序遍历结果
输出格式：
二叉树后序遍历结果
Example:
Inputs:
426315
623415
Outputs:
632514
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
using namespace std;
//由前序中序推出后序遍历
int PreIn2Post(string Preorder, string Inorder)
{
    int length = Preorder.length();
    if (length == 0)return 0;
    char Root = Preorder[0];
    int i = 0;
    for (; i<length; i++)
    {
        if (Root == Inorder[i])
            break;
    }
    cout <<endl<< "左子树前序遍历:" << Preorder.substr(1, i) << " 左子树中序遍历:" << Inorder.substr(0, i) << endl;
    PreIn2Post(Preorder.substr(1, i), Inorder.substr(0, i));//递归访问左子树
    cout << endl << "右子树前序遍历:" << Preorder.substr(i+1, length) << " 右子树中序遍历:" << Inorder.substr(i + 1, length) << endl;
    PreIn2Post(Preorder.substr(i + 1, length), Inorder.substr(i + 1, length));//递归访问右子树
    cout << Root;//访问根
    return 0;
}
//主函数
int main()
{
    string Preorder, Inorder;
    cin >> Preorder >> Inorder;
    PreIn2Post(Preorder, Inorder);
    cout << endl;
    return 0;
}

