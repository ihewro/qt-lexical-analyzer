#include "mygraph.h"
#include "lex.h"

using namespace std;



/**
 * @brief MyGraph::MyGraph:Ĭ�Ϲ��캯��,��ʼ�ߺͶ�����Ŀ��Ϊ0
 */
MyGraph::MyGraph():mVexNum(0),mEdgeNum(0){

    for(int i =0;i<100;i++){
        for(int j =0;j<100;j++){
            mMatrix[i][j].push_back('^') ;
        }
    }

}



/**
 * @brief MyGraph::addEdge ����һ����
 * @param a���ڵ�����
 * @param b����һ���ڵ�����
 * @param edgeCondition �����ϵ�ֵ��ת������
 */
void MyGraph::addEdge(int a, int b, char edgeCondition){
    if (edgeCondition == '^'){
        cout << "��ȡ���ӱ�("<<a << "," << b <<")ֵ����" << endl;
    }else{
        cout << "���ӱߵ�ֵΪ" << edgeCondition << endl;
    }
    if (mMatrix[a][b].at(0) == '^'){
        mMatrix[a][b].clear();
    }
    bool flag = true;
    for (int i = 0; i < mMatrix[a][b].size() ; ++i) {
        if (mMatrix[a][b].at(i) == edgeCondition){
            flag = false;
            break;
        }
    }
    if (flag){
        mMatrix[a][b].push_back(edgeCondition);
        cout << "[" << a << "," << b << "]" << "����" << endl;
    }else{
        cout << "[" << a << "," << b << "]" << "�Ѿ����ӣ��ظ����ӣ�" << endl;
    }
}

/**
 * @brief MyGraph::deleteEdge ɾ��ĳ����
 * @param a
 * @param b
 */
void MyGraph::deleteEdge(int a, int b){
    mMatrix[a][b].clear();
    mMatrix[a][b].push_back('^');
    cout << "[" << a << "," << b << "]" << "�Ͽ�����" << endl;
}

void MyGraph::deleteEdge(int a, int b,char condition){
    mMatrix[a][b].clear();
    mMatrix[a][b].push_back('^');
    cout << "[" << a << "," << b << "]" << "�Ͽ�����" << endl;
}


/**
 * ��ȡ���ϵ�ֵ
 * @param a
 * @param b
 * @return
 */
vector<char> MyGraph::getEdgeValue(int a, int b) {
    return mMatrix[a][b];
}

void MyGraph::printMyGraph() {
    for(int i = 0 ;i<100 ;i ++){
        for(int j = 0;j<100;j++){
            if (getEdgeValue(i,j).at(0) != '^'){
                cout << "״̬" << i << "���� " << getEdgeValue(i,j).at(0) << " ����>" << "״̬" << j << endl;
            }
        }
    }

}



