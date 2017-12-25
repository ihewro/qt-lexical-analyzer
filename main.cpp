#include <iostream>
#include "lex.h"
#include <string>
#include <cstdio>

int main() {
    std::cout << "Hello, World!" << std::endl;

   string regxInput = "a(a|b)*b";
    Lex lexTest;
    lexTest.getNFA(regxInput);
    cout << "�������������������������������������������������������������������������������������ʾ������������������������������������������������������������������������������������" << endl;
    cout << "���յ�NFAջ�Ľڵ���ĿΪ"<< lexTest.lexNFA.NFAGraph.mVexNum << endl;
    cout << "�ߵ���ĿΪ"  << lexTest.lexNFA.NFAGraph.mEdgeNum << endl;
    cout << "��ĸ��Ϊ��";
    for(int i =0;i<lexTest.alphabet.size();i++){
        cout << lexTest.alphabet[i] << " ";
    }
    cout << endl;

    //��ӡ�����յ�
    cout << "���ڵ�Ϊ:" << lexTest.lexNFA.startStatus << endl;
    cout << "�յ�ڵ�Ϊ:" << lexTest.lexNFA.endStatus << endl;

    //��ӡ�ٽ����
    lexTest.printNFA();

    lexTest.generateNFADotString(lexTest.lexNFA.NFAGraph);

    lexTest.getDFA();

    //��ӡDFA
    lexTest.printDFA();

    cout << "��������������������������������DFA�ڵ��������������������������������" << endl;
    for(int i =0;i<lexTest.lexDFA.mVexs.size();i++){
        for (int j = 0; j < lexTest.lexDFA.mVexs[i].size() ; ++j) {
            cout << lexTest.lexDFA.mVexs[i][j] << " ";
        }
        cout << endl;
    }

    cout << "DFA�ڵ���Ŀ" << lexTest.lexDFA.DFAGraph.mVexNum << "�ߵ���Ŀ" << lexTest.lexDFA.DFAGraph.mEdgeNum << endl;

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,0);


    lexTest.minimizeDFA();
    //system("dot -Tjpg aaa.dot -o aaa.jpg");

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,1);


    return 0;
}


