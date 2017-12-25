//
// Created by hewro on 2017/12/19.
//

#ifndef LEXCODE_LEX_H
#define LEXCODE_LEX_H


#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "mygraph.h"
using namespace std;


class NFA{
public:
    vector<int> mVexs;//DFA�ڵ㼯�ϣ��洢���ǽڵ�����,λ�õ���Ϣ��û�д洢���ݣ���DFA�Ľڵ㼯�ϲ�һ��
    MyGraph NFAGraph;//NFAͼ�Ľṹ

    int startStatus;//��ʼ״̬�����
    int endStatus;//��ֹ״̬�����

    /*int getGraphPointNum(){
        return this->NFAGraph.mVexNum;
    }*/

};

class DFA{
public:
    vector<vector<int>> mVexs;//ֵΪ���ڵ�NFA����,λ��Ϊ�ڵ����
    MyGraph DFAGraph;//DFAͼ�Ľṹ
    int startStatus;//��ʼ״̬
    vector <int> endStatus;//�����ж����ֹ״̬

    //��ȡ��ĳ���ڵ������Ľڵ㣬ת������ȷ��
    int getTargetStatus(int node, char condition);


};

//��NFAջ�е�Ԫ�صĶ����һ���ṹ���洢���������յ����Ϣ��
struct node{
public:
    int nArray[2];
    node(int a[2]){
        for(int i =0;i<2;i++)
            nArray[i] = a[i];
    };
};
class Lex{
public:
    const string dot  = "/usr/local/bin/dot";
    NFA lexNFA;//NFA�ṹ
    DFA lexDFA;//DFA�ṹ
    stack <char> operatorStack;//������ջ
    stack <node> NFAStatusPointStack;//NFAջ�е�NFA����ʼ����ţ�0��λ�ô�����㣬1��λ�ô����յ�
    vector<char> alphabet;//��ĸ���洢��������ʽ�е���ĸ

    //���캯��
    Lex();

    /*��ȡNFA*/

    void getNFA(string regxInput);
    //�ж��Ƿ��ǲ�����
    bool isOperator(char ch);
    //����������NFA�������ڵ�һ���ߣ���������ǰ������ڵ����������ߵ�ֵΪת������
    void createBasicNFA(char ch);
    //�����ظ����ŵĲ���
    void repeatCharacterOperation();
    //����ѡ����Ĳ���
    void selectorCharacterOperation();
    //�������ӷ��Ĳ���
    void joinerCharacterOperation();
    //����NFA��dot�ı�
    string generateNFADotString(MyGraph myGraph);
    //��ӡNFA�����ӱ�
    void printNFA();

    /*��ȡDFA*/

    void getDFA();

    vector<int> e_closure(vector<int> statusArray);
    vector<int> nfaMove(vector<int> statusArray,char condition);
    int isDFAStatusRepeat(vector<int> a);

    string generateDFADotString(MyGraph myGraph,int choice);
    //�ж��Ƿ�����ֹ״̬
    bool isEndDFAStatus(vector<int> nfaArray);
    //��ӡDFA�����ӱ�
    void printDFA();

    /*��С��DFA*/
    void minimizeDFA();
    //�жϽڵ�����Ƿ���DFA��ֹ�ڵ㼯����
    bool isInDFAEndStatus(int i);
    //�ϲ������ڵ�
    void mergeTwoNode(int a, int b);
};

#endif //LEXCODE_LEX_H
