//
// Created by hewro on 2017/12/19.
//

#ifndef LEXCODE_MYGRAPH_H
#define LEXCODE_MYGRAPH_H


#include<iostream>
#include <vector>

using namespace std;

/*c++实现的图的数据结构：邻接矩阵存储图的数据*/

//这里的缺陷是临界矩阵是定死的，空间大小是定死的。

class MyGraph{

public:
    int mVexNum;//顶点数目
    int mEdgeNum;//边的数目
    //顶点（所有状态）集合
    vector<char> mMatrix[100][100];//邻接矩阵，对应的值为转移条件，就是字母，默认都是空''，表示未连接

public:
    MyGraph();//默认构造函数

    void printMyGraph();//打印输出图内容

    //增加一条边，边上的值为转换的条件
    //根据位置增加一条边
    void addEdge(int a, int b, char edgeCondition);
    void deleteEdge(int a, int b);
    void deleteEdge(int a, int b,char condition);

    vector<char> getEdgeValue(int a ,int b);


};


#endif //LEXCODE_MYGRAPH_H
