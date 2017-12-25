#include <iostream>
#include "lex.h"
#include <string>
#include <cstdio>

int main() {
    std::cout << "Hello, World!" << std::endl;

   string regxInput = "a(a|b)*b";
    Lex lexTest;
    lexTest.getNFA(regxInput);
    cout << "————————————————————————————————————————结果显示——————————————————————————————————————————" << endl;
    cout << "最终的NFA栈的节点数目为"<< lexTest.lexNFA.NFAGraph.mVexNum << endl;
    cout << "边的数目为"  << lexTest.lexNFA.NFAGraph.mEdgeNum << endl;
    cout << "字母表为：";
    for(int i =0;i<lexTest.alphabet.size();i++){
        cout << lexTest.alphabet[i] << " ";
    }
    cout << endl;

    //打印起点和终点
    cout << "起点节点为:" << lexTest.lexNFA.startStatus << endl;
    cout << "终点节点为:" << lexTest.lexNFA.endStatus << endl;

    //打印临界矩阵
    lexTest.printNFA();

    lexTest.generateNFADotString(lexTest.lexNFA.NFAGraph);

    lexTest.getDFA();

    //打印DFA
    lexTest.printDFA();

    cout << "————————————————DFA节点输出——————————————" << endl;
    for(int i =0;i<lexTest.lexDFA.mVexs.size();i++){
        for (int j = 0; j < lexTest.lexDFA.mVexs[i].size() ; ++j) {
            cout << lexTest.lexDFA.mVexs[i][j] << " ";
        }
        cout << endl;
    }

    cout << "DFA节点数目" << lexTest.lexDFA.DFAGraph.mVexNum << "边的数目" << lexTest.lexDFA.DFAGraph.mEdgeNum << endl;

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,0);


    lexTest.minimizeDFA();
    //system("dot -Tjpg aaa.dot -o aaa.jpg");

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,1);


    return 0;
}


