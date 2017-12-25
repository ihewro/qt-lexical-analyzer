//
// Created by hewro on 2017/12/19.
//

#ifndef LEXCODE_MYGRAPH_H
#define LEXCODE_MYGRAPH_H


#include<iostream>
#include <vector>

using namespace std;

/*c++ʵ�ֵ�ͼ�����ݽṹ���ڽӾ���洢ͼ������*/

//�����ȱ�����ٽ�����Ƕ����ģ��ռ��С�Ƕ����ġ�

class MyGraph{

public:
    int mVexNum;//������Ŀ
    int mEdgeNum;//�ߵ���Ŀ
    //���㣨����״̬������
    vector<char> mMatrix[100][100];//�ڽӾ��󣬶�Ӧ��ֵΪת��������������ĸ��Ĭ�϶��ǿ�''����ʾδ����

public:
    MyGraph();//Ĭ�Ϲ��캯��

    void printMyGraph();//��ӡ���ͼ����

    //����һ���ߣ����ϵ�ֵΪת��������
    //����λ������һ����
    void addEdge(int a, int b, char edgeCondition);
    void deleteEdge(int a, int b);
    void deleteEdge(int a, int b,char condition);

    vector<char> getEdgeValue(int a ,int b);


};


#endif //LEXCODE_MYGRAPH_H
