#include "mygraph.h"
#include "lex.h"

using namespace std;



/**
 * @brief MyGraph::MyGraph:默认构造函数,初始边和顶点数目都为0
 */
MyGraph::MyGraph():mVexNum(0),mEdgeNum(0){

    for(int i =0;i<100;i++){
        for(int j =0;j<100;j++){
            mMatrix[i][j] = '^';
        }
    }

}



/**
 * @brief MyGraph::addEdge 增加一条边
 * @param a：节点名称
 * @param b：另一个节点名称
 * @param edgeCondition ：边上的值：转换条件
 */
void MyGraph::addEdge(int a, int b, char edgeCondition){
    if (edgeCondition == '^'){
        cout << "获取连接边的值错误" << endl;
    }else{
        cout << "连接边的值为" << edgeCondition << endl;
    }
    mMatrix[a][b] = edgeCondition;
    cout << "[" << a << "," << b << "]" << "连接" << endl;
}

/**
 * @brief MyGraph::deleteEdge 删除某条边
 * @param a
 * @param b
 */
void MyGraph::deleteEdge(int a, int b){
    mMatrix[a][b] = '^';
    cout << "[" << a << "," << b << "]" << "断开连接" << endl;
}

/**
 * 获取边上的值
 * @param a
 * @param b
 * @return
 */
char MyGraph::getEdgeValue(int a, int b) {
    return mMatrix[a][b];
}

void MyGraph::printMyGraph() {
    for(int i = 0 ;i<100 ;i ++){
        for(int j = 0;j<100;j++){
            if (getEdgeValue(i,j) != '^'){
                cout << "状态" << i << "—— " << getEdgeValue(i,j) << " ——>" << "状态" << j << endl;
            }
        }
    }

}

