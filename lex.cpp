#include "lex.h"

#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<iomanip>
//#include <QProcess>

using namespace std;

Lex::Lex() = default;

/**
 * @brief Lex::isOperator �ж��Ƿ��������
 * @param ch
 * @return
 */
bool Lex::isOperator(char ch){
    return ch == '|' || ch == '*' || ch == '(' || ch == ')';
}

void Lex::createBasicNFA(char ch){
    cout << "������ĸ���������NFA�Ĳ���" << endl;
    //����Ľڵ��������ǰ�ڵ����+1
    int startPoint = lexNFA.mVexs.size() + 1;
    int endPoint = startPoint + 1;
    lexNFA.NFAGraph.addEdge(startPoint,endPoint,ch);//����һ���߶Աߵ����ӣ��ߵ�������ת�����������Ǹ��ַ���
    cout << "��ʼ����һ��������� NFA,���Ϊ��(" << startPoint << "," << endPoint << ")" << endl;
    //���������ڵ�
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //������1
    lexNFA.NFAGraph.mEdgeNum++;

    //��ӵ��ڵ�������
    lexNFA.mVexs.push_back(startPoint);
    lexNFA.mVexs.push_back(endPoint);

    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = startPoint;
    newNFAStatusPoint[1] = endPoint;

    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //����յ�����
    lexNFA.startStatus = startPoint;
    lexNFA.endStatus = endPoint;
}

/**
 * @brief Lex::repeatCharactorOperation
 * �ظ����ŵļ���*��
 * 0. ������ɺ�Ҫ��NFAջ��Ԫ�س�ջ���������û����
 * 1. ��NFAջ����Ԫ��
 * 2. �½�����ͷβ�ڵ㣬��ͷ�ڵ�������ǰ��ͷ��㣬β�ڵ�������ǰ��β�ڵ�
 * 3. ���µ�NFAѹ��NFAջ��
 */
void Lex::repeatCharacterOperation(){
    cout << "�����ظ����Ĳ���" << endl;
    //1.��ȡջ����һ��Ԫ��
    int top1NFA[2];
    for(int i =0;i<2;i++){
        top1NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();

    //2.
    //���������µĽڵ�
    int newStartPoint1 = lexNFA.mVexs.size()+1;
    int newStartPoint2 = newStartPoint1+1;

    lexNFA.NFAGraph.addEdge(newStartPoint1,newStartPoint2,'E');
    lexNFA.NFAGraph.addEdge(newStartPoint1,top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],newStartPoint2,'E');

    //�ڵ���Ŀ��2
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //�ߵ���Ŀ��4
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 4;
    //��ӵ��ڵ�������
    lexNFA.mVexs.push_back(newStartPoint1);
    lexNFA.mVexs.push_back(newStartPoint2);

    //3.
    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = newStartPoint1;
    newNFAStatusPoint[1] = newStartPoint2;
    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //����յ�����
    lexNFA.startStatus = newStartPoint1;
    lexNFA.endStatus = newStartPoint2;

}

/**
 * @brief Lex::joinerCharacterOperation
 * ���ӷ��ŵļ���&��
 * 0. ����������������������Ҫpop��
 * 1. ��NFAջ������Ԫ��
 * 2. ջ�ζ���β�ڵ�ջ����ͷ�ڵ�����
 * 3. ���µ�NFAѹ��ջ��
 */
void Lex::joinerCharacterOperation(){
    /**
     * ������һ���ϲ��ڵ�����⣬���ǵ�NFAջ��ֻ�洢����ʼ��Ϣ�����Ժϲ��ڵ��ú����ѣ�
     * ���Ҳ��ϲ��ڵ������Ψһ��������NFA�Ľڵ�ͱ߱�࣬����Ӱ������ƥ������
     */

    cout << "\n�������ӷ��Ĳ���\n" << endl;
    //1.
    //��ȡNFAջ��������Ԫ�أ�����ջ�ж���
    int top1NFA[2];
    for(int i =0;i<2;i++){
        top1NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();
    int top2NFA[2];
    for(int i =0;i<2;i++){
        top2NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();
    cout << "ǰ�ڵ����:(" << top2NFA[0] << "," << top2NFA[1] << ")" << endl;
    cout << "��ڵ����:(" << top1NFA[0] << "," << top1NFA[1] << ")" << endl;
    cout << endl;
    //��ȡ�����ջ����Ԫ�أ�����ջ�ж�������Ϊ�����꣬��Щ��û���ˣ����õ���������
    operatorStack.pop();

    //2.
    //lexNFA.NFAGraph.addEdge(top2NFA[1],top1NFA[1],lexNFA.NFAGraph.getEdgeValue(top1NFA[0],top1NFA[1]));
    //lexNFA.NFAGraph.deleteEdge(top1NFA[0],top1NFA[1]);
    //��������
    //lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 1;
    //�ڵ�����1����Ϊ�ϲ���һ���ڵ�
    //lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum - 1;

    /*//���ϲ��ڵ㣬ֱ�Ӱ�ǰ�ڵ��β���Ӻ�ڵ��ͷ
    lexNFA.NFAGraph.addEdge(top2NFA[1],top1NFA[0],'E');
    //������1
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 1;*/

    //�ϲ��ڵ㣬�������䣬�ڵ�����1

    for(int i =1;i<lexNFA.NFAGraph.mVexNum + 1;i++){
        if (i == top1NFA[0]){
            for (int j = 1; j <lexNFA.NFAGraph.mVexNum + 1 ; ++j) {
                if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) != '^'){
                    lexNFA.NFAGraph.addEdge(top2NFA[1],j,lexNFA.NFAGraph.getEdgeValue(i,j).at(0));
                    lexNFA.NFAGraph.deleteEdge(top1NFA[0],j);
                    lexNFA.NFAGraph.mVexNum --;//�ڵ�����1
                    lexNFA.mVexs[top1NFA[0]] = -1;
                }
            }
            break;
        }
    }




    //3.
    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = top2NFA[0];
    newNFAStatusPoint[1] = top1NFA[1];
    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //���������յ�
    lexNFA.startStatus = top2NFA[0];
    lexNFA.endStatus = top1NFA[1];

}

/**
 * @brief Lex::selectorCharacterOperation
 * ѡ����ŵļ���|��
 * 0. ������ɺ�Ҫ��ջ������Ԫ�ض���ջ������ͬ��
 * 1. ��NFAջ������Ԫ��
 * 2. �½�����ͷβ�ڵ㣬��ͷ�������������ǰ��ͷ��㣬��β�ڵ�����������ǰ��β�ڵ�
 * 3. ���µ�NFAѹ��ջ��
 */
void Lex::selectorCharacterOperation(){
    cout << "����ѡ����Ĳ���" << endl;
    //1.
    //��ȡNFAջ��������Ԫ�أ�����ջ�ж���
    int top1NFA[2];
    for(int i =0;i<2;i++){
        top1NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    cout << "��ǰNFAջ���Ľڵ�Ϊ��(" << top1NFA[0] << "," << top1NFA[1] << ")" << endl;

    NFAStatusPointStack.pop();
    int top2NFA[2];
    for(int i =0;i<2;i++){
        top2NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();
    //��ȡ�����ջ����Ԫ�أ�����ջ�ж�������Ϊ�����꣬��Щ��û���ˣ����õ���������
    operatorStack.pop();

    //2.
    //���������µĽڵ�
    int newStartPoint1 = lexNFA.mVexs.size()+1;
    int newStartPoint2 = newStartPoint1+1;

    lexNFA.NFAGraph.addEdge(newStartPoint1,top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(newStartPoint1,top2NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top2NFA[1],newStartPoint2,'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],newStartPoint2,'E');

    //�ڵ���Ŀ��2
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //�ߵ���Ŀ��4
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 4;
    //��ӵ��ڵ�������
    lexNFA.mVexs.push_back(newStartPoint1);
    lexNFA.mVexs.push_back(newStartPoint2);



    //3.
    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = newStartPoint1;
    newNFAStatusPoint[1] = newStartPoint2;

    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //���������յ�
    lexNFA.startStatus = newStartPoint1;
    lexNFA.endStatus = newStartPoint2;

}

/**
 * @brief Lex::getNFA ����ȡNFA���ؼ�����
 * @param regxInput
 * 1. �������������ʽ����ѭ��ɨ��ÿһ��Ԫ�أ����������ջ��NFAջ
 * 2. �����յ�NFAջ���д��������Ϊ�յĻ���
 */
void Lex::getNFA(string regxInput){
    unsigned long strLen = regxInput.length();
    char ch;

    cout<< "�ַ���Ϊ:" << regxInput << "�ַ����ĳ���Ϊ:" << strLen << endl;
    /*1. �������������ʽ����ѭ��ɨ��ÿһ��Ԫ�أ����������ջ��NFAջ*/
    for(unsigned long i =0; i < strLen; i++){
        ch = regxInput[i];
        cout<< "\n*************��ǰ�ַ�Ϊ " << ch << " *************\n" << endl;
        //����������
        if(isOperator(ch)){
            switch (ch) {
                case '*'://�ظ������ȼ����һ
                    cout << "ok*" << ch << endl;
                    //������ѹ��NFAջ��
                    repeatCharacterOperation();

                    //�����һ���ַ�����ĸ�����������ţ���Ҫ������ӷ�
                    if((i+1<strLen)&&(regxInput[i+1] == '(' || !isOperator(regxInput[i+1]))){
                        operatorStack.push('&');
                    }

                    break;
                case '|'://ѡ������ȼ�����������ʡ�Բ�д
                    //��Ҫ�������ջ�����е�&��ջ�����㣬����������(ֹͣ
                    if (operatorStack.empty()){
                        cout<<"�����ջΪ��"<< endl;
                        //��������ջΪ�գ���ʲô����Ҳ�����������һ�°Ѹ÷���ѹջ�Ϳ�����
                    }else{
                        cout<<"�����ջ��Ϊ��"<< endl;
                        ch = operatorStack.top();
                        cout << "��ǰջ��Ԫ��Ϊ" << ch << endl;
                        while(ch != '('){
                            //cout << "loop" << endl;
                            if(ch == '&'){
                                cout << "ջ��Ԫ��Ϊ���ӷ�����Ҫ�Ƚ������ӷ��ļ���" << endl;
                                joinerCharacterOperation();
                            } else{
                                break;
                            }
                            ch = operatorStack.top();
                        }
                    }

                    operatorStack.push('|');
                    cout<<"��ѡ���ѹ��ջ��"<<endl;
                    break;
                case '('://������
                    operatorStack.push(ch);
                    cout << "��������ѹ��ջ��" << endl;
                    cout << "��ǰ����ջ��С" << operatorStack.size() << endl;
                    break;
                case ')'://������
                    cout << "����ջ��С" << operatorStack.size() << endl;
                    //������Ҫ�������ڽ��м��㣬���Ѽ�����ѹջ
                    ch = operatorStack.top();

                    while(ch != '('){
                        //cout << "loop2";
                        switch (ch) {
                            case '&':
                                joinerCharacterOperation();
                                break;
                            case '|':
                                selectorCharacterOperation();
                                break;
                            default:
                                break;
                        }
                        ch = operatorStack.top();
                    }
                    cout << "��������" << ch << endl;
                    operatorStack.pop();//��ʱ�����ջ��Ԫ���������ţ���Ҫ�Ƴ���ȥ
                    //�����һ���ַ�����ĸ�����������ţ���Ҫ������ӷ�
                    if((i+1<strLen)&&(regxInput[i+1] == ')' || !isOperator(regxInput[i+1]))){
                        operatorStack.push('&');
                    }
                    break;

                default:
                    cout << "ok" << ch << endl;
                    break;
            }
        }else{//���������
            auto flag = true;//�Ƿ���ӵ���ĸ����
            for (int i =0;i<alphabet.size();i++){
                if (ch == alphabet[i])
                    flag = false;
            }

            if (flag){
                alphabet.push_back(ch);
            }
            //����һ��������NFA��Ҳ���Ǳ���a,a��NFA���������ڵ㣬��ʼ�����ֹ�㣬�ߵ�ֵΪa
            createBasicNFA(ch);

            //�����һ���ַ�����ĸ�Ļ����������ջ�м���һ�����ӷ�&
            if(i+1 <strLen && (!isOperator(regxInput[i+1]) || regxInput[i+1] == '(')){
                operatorStack.push('&');
            }
        }
    }

    /*2. �����յ�NFAջ�������ջ���д��������Ϊ�յĻ���*/
    cout << "---------------------------------------------" << endl;
    if (operatorStack.empty()){
        cout << "�����������ջΪ��" << endl;
    }else{
        cout << "�����������ջ��Ϊ��" << endl;
    }
    while(!operatorStack.empty()){
        ch= operatorStack.top();
        cout << "��ǰ�������ջ��Ԫ��Ϊ" << ch << endl;
        switch (ch) {
            case '|':
                selectorCharacterOperation();
                break;
            case '&':
                joinerCharacterOperation();
                break;
            default:
                break;
        }
    }

}



void Lex::getDFA() {

    //��DFA�ڵ㿪ͷλ�ü�һ����Ԫ��ռλ����Ϊ�ڵ�����Ǻ�λ�ö�Ӧ�ģ����ڵ�����Ǵ�1��ʼ�ġ�
    lexDFA.mVexs.emplace_back(0);

    vector<int> initStatus;
    initStatus.push_back(lexNFA.startStatus);
    vector<int>initStatusTrans(e_closure(vector<int>(initStatus)));

    //��DFA������һ���ڵ�
    int newDFAStartPoint = lexDFA.DFAGraph.mVexNum + 1;
    lexDFA.DFAGraph.mVexNum ++;//�ڵ�����1

    lexDFA.startStatus = newDFAStartPoint;

    stack<int> DFAStatusStack;//DFA״̬ջ,���ջֻ��Ҫ�洢DFA����žͿ����ˣ�û��Ҫ�洢DFA�ڵ��Ӧ��NFA���ϣ��洢��û�о�����ĸ��ת����DFA״̬
    DFAStatusStack.push(newDFAStartPoint);

    lexDFA.mVexs.push_back(initStatusTrans);//NFA����Eת�����ϸ�ֵ��DFA�ĵ�һ���ڵ�

    //cout<<"DFA�ڵ���" << lexDFA.mVexs.size() << " " << lexDFA.DFAGraph.mVexNum << endl;

    while (!DFAStatusStack.empty()){

        int topDFAStack = DFAStatusStack.top();
        DFAStatusStack.pop();

        for (int i = 0; i < alphabet.size() ; ++i) {//����ĸ���ÿ��Ԫ�ض���Ҫ��Ϊת������

            vector<int> tempArray = e_closure(nfaMove(lexDFA.mVexs[topDFAStack],alphabet[i]));

            if (tempArray.empty()){//���ת��������NFA����Ϊ�գ������ô�ת������
                continue;
            }
            int position = isDFAStatusRepeat(tempArray);//�ж�������DFA״̬�Ƿ��Ѿ������ˣ�����Ѿ����ڣ����ظ�״̬�ڽڵ��λ��

            if (position == -1){//����������ɵ�DFA״̬��û���ظ�
                int tempDFAStatusNode = lexDFA.DFAGraph.mVexNum + 1;
                lexDFA.DFAGraph.mVexNum ++;//�ڵ�����1
                if (isEndDFAStatus(tempArray)){
                    lexDFA.endStatus.push_back(tempDFAStatusNode);
                }
                lexDFA.mVexs.push_back(tempArray);//��NFA���ϸ�ֵ��DFA��״̬
                DFAStatusStack.push(tempDFAStatusNode);//���²�����DFA״̬���뵽DFAջ��
                position = tempDFAStatusNode;
            }

            //���ӽڵ㣬������
            lexDFA.DFAGraph.addEdge(topDFAStack,position,alphabet[i]);
            lexDFA.DFAGraph.mEdgeNum ++;
        }
    }
}

/**
 * �ж��Ƿ���DFA����ֹ״̬��ֻҪ������NFA����ֹ״̬��DFA״̬������ֹ״̬
 * @param nfaArray
 * @return
 */
bool Lex::isEndDFAStatus(vector<int> nfaArray){
    for (int i = 0; i < nfaArray.size(); ++i) {
        if (lexNFA.endStatus == nfaArray[i]){
            return true;
        }
    }
    return false;
}


/**
 * �ж��²�����DFA״̬�Ƿ��Ѿ�����DFA״̬����
 * @param a
 * @return
 */
int Lex::isDFAStatusRepeat(vector<int> a){

    int position = -1;
    for (int i = 1; i < lexDFA.mVexs.size()+1; ++i) {
        if (a == lexDFA.mVexs[i]){
            position = i;
            break;
        }
    }

    return position;
}


/**
 * DFA:�ܹ���NFA��״̬����statusArray�е�ĳ��״̬s��ʼֻͨ��Eת�������NFA״̬����
 * @param statusArray
 * @param lexNFA
 * @return
 */
vector<int> Lex::e_closure(vector<int> statusArray){

    cout << "����������������������������������ʼe_closure(T)������������������������������������"<<endl;
    cout << "״̬����Ϊ:" << endl;
    for (int k = 0; k < statusArray.size(); ++k) {
        cout << statusArray[k] << " ";
    }
    cout << endl;

    vector<int> resultsArray;//���״̬�����Eת������
    stack<int> statusStack;//��ŵݹ�����е�״̬������ջΪ�յ�ʱ�򣬵ݹ����

    for (int i = 0; i < statusArray.size(); ++i) {
        statusStack.push(statusArray[i]);//��ʼ��״̬ջ
        resultsArray.push_back(statusArray[i]);//״̬����Ҳ����ͨ��Eת�����ﱾ��������Ҫ��������ӵ����������
    }


    while(!statusStack.empty()){
        int status = statusStack.top();
        statusStack.pop();

        for (int i = 1; i < lexNFA.mVexs.size() + 1 ; ++i) {
            if (i == status){
                for(int j = 1;j<lexNFA.mVexs.size()+1;j++){
                    if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) == 'E'){//�ҵ�ת������ΪE�ĵ��յ�
                        statusStack.push(j);//���뵽״̬ջ��
                        resultsArray.push_back(j);//���뵽���������
                    }
                }
            }
        }
    }

    /*ȥ���ظ�Ԫ�أ�unique()ֻ��ȥ�����ڵ��ظ�Ԫ�أ�������������ȥ��*/
    sort(resultsArray.begin(),resultsArray.end());
    resultsArray.erase(unique(resultsArray.begin(), resultsArray.end()), resultsArray.end());

    cout << "ecolosureת�����Ϊ��" << endl;

    for (int k = 0; k < resultsArray.size(); ++k) {
        cout << resultsArray[k] << " ";
    }
    cout << endl;
    return resultsArray;

}


/**
 * DFA: �ܹ���NFA��״̬����statusArray�е�ĳ��״̬������ͨ������Ϊconditionת�������NFA״̬����
 * @param statusArray
 * @param condition
 * @param lexNFA
 * @return
 */
vector<int> Lex::nfaMove(vector<int> statusArray,char condition){

    cout << "����������������������������������ʼmove(T," << condition << ")������������������������������������"<<endl;

    cout<< "״̬����Ϊ" << endl;
    for (int k = 0; k < statusArray.size(); ++k) {
        cout << statusArray[k] << " " ;
    }
    cout << endl;
    vector<int>resultsArray;//�������
    stack<int> statusStack;//״̬ջ

    for (int j = 0; j < statusArray.size() ; ++j) {
        statusStack.push(statusArray[j]);
    }

    while(!statusStack.empty()){
        int status = statusStack.top();
        statusStack.pop();
        for (int i = 1; i <lexNFA.mVexs.size()+1 ; ++i) {
            if (i == status){
                for (int j = 1; j < lexNFA.mVexs.size() + 1; ++j) {
                    if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) == condition){
                        resultsArray.push_back(j);//���յ���뵽��������У�����ʱ����Ҫ�ٽ��յ�ѹ�뵽״̬ջ��
                    }
                }
            }
        }
    }

    /*ȥ���ظ�Ԫ�أ�unique()ֻ��ȥ�����ڵ��ظ�Ԫ�أ�������������ȥ��*/
    sort(resultsArray.begin(),resultsArray.end());
    resultsArray.erase(unique(resultsArray.begin(), resultsArray.end()), resultsArray.end());

    cout << "move���Ϊ:" << endl;
    for (int l = 0; l < resultsArray.size(); ++l) {
        cout << resultsArray[l] << " ";
    }
    cout << endl;
    return resultsArray;
};




void Lex::printNFA(){
    int num = static_cast<int>(lexNFA.mVexs.size() + 1);
    //cout << "�ڵ���Ŀ" << num;
    for(int i = 1 ;i<num  ;i ++){
        for(int j = 1;j<num;j++){
            if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) != '^'){
                cout << "״̬" << i << " ���� " << lexNFA.NFAGraph.getEdgeValue(i,j).at(0) << " ����> " << "״̬" << j << endl;
            }
        }
    }
}


void Lex::printDFA(){
    int num = lexDFA.DFAGraph.mVexNum + 1;
    //cout << "�ڵ���Ŀ" << num;
    for(int i = 0 ;i<num  ;i ++){
        for(int j = 0;j<num;j++){
            if (lexDFA.DFAGraph.getEdgeValue(i,j).at(0) != '^'){
                cout << "״̬" << i << " ���� " << lexDFA.DFAGraph.getEdgeValue(i,j).at(0) << " ����> " << "״̬" << j << endl;
            }
        }
    }
}



string Lex::generateNFADotString(MyGraph myGraph){
    string tab = "    ";
    string result;
    result = "digraph G{\n" + tab + "\"\"[shape=none]\n";
    for (int i = 1; i <lexNFA.mVexs.size() + 1 ; ++i) {
        if (lexNFA.mVexs[i] != -1)
        {
            result += tab + "\"" + to_string(i) +  "\"";
            if (i == lexNFA.endStatus){
                result += "[shape=doublecircle]\n";
            }else{
                result += "[shape=circle]\n";
            };
        }
    }

    result += "\n";
    result += tab + "\"\"->\"" + to_string(lexNFA.startStatus)+ "\"\n";

    int num = lexNFA.mVexs.size()  + 1;
    //cout << "�ڵ���Ŀ" << num;
    for(int i = 1 ;i<num  ;i ++){
        for(int j = 1;j<num;j++){
            if (myGraph.getEdgeValue(i,j).at(0) != '^'){
                result += tab + "\"" +to_string(i) + "\"" + " -> " + to_string(j) + "[label=\"" + myGraph.getEdgeValue(i,j).at(0) + "\"]\n";
            }
        }
    }
    result.append("}");

    ofstream ofile;               //��������ļ�
    ofile.open("/Users/hewro/lexical/dots/nfa.dot");
    ofile << result << endl;
    ofile.close();
    //ִ������nfaͼƬ
    system("/usr/local/bin/dot -Tjpg /Users/hewro/lexical/dots/nfa.dot -o /Users/hewro/lexical/images/nfa.jpg");//����QT��ĺ���
    cout << result << endl;
    return  result;

}

string Lex::generateDFADotString(MyGraph myGraph,int choice){

    string tab = "    ";
    string result;
    result = "digraph G{\n" + tab + "\"\"[shape=none]\n";
    for (int i = 1; i <myGraph.mVexNum + 1 ; ++i) {

        if (!lexDFA.mVexs[i].empty()){
            result += tab + "\"" + to_string(i) +  "\"";
            bool flag = true;

            for (int j = 0; j < lexDFA.endStatus.size(); ++j) {
                if (i == lexDFA.endStatus[j]){
                    flag = false;
                    break;
                }
            }
            if (!flag){
                result += "[shape=doublecircle]\n";
            }else{
                result += "[shape=circle]\n";
            };
        }
    }

    result += "\n";
    result += tab + "\"\"->\"" + to_string(lexDFA.startStatus)+ "\"\n";

    int num = myGraph.mVexNum + 1;
    //cout << "�ڵ���Ŀ" << num;
    for(int i = 0 ;i<num  ;i ++){
        for(int j = 0;j<num;j++){
            if (myGraph.getEdgeValue(i,j).at(0) != '^'){
                for (int k = 0; k <myGraph.getEdgeValue(i,j).size() ; ++k) {
                    result += tab + "\"" +to_string(i) + "\"" + " -> \"" + to_string(j) + "\"[label=\"" + myGraph.getEdgeValue(i,j).at(k) + "\"]\n";
                }
            }
        }
    }
    result.append("}");
    ofstream ofile;//��������ļ�
    if (choice == 0){
        ofile.open("/Users/hewro/lexical/dots/dfa.dot");
    }else{
        ofile.open("/Users/hewro/lexical/dots/mindfa.dot");
    }
    ofile << result << endl;
    ofile.close();
    //ִ������dfaͼƬ
    if (choice == 0){
        system("/usr/local/bin/dot -Tjpg /Users/hewro/lexical/dots/dfa.dot -o /Users/hewro/lexical/images/dfa.jpg");//����QT��ĺ���
    }else{
        system("/usr/local/bin/dot -Tjpg /Users/hewro/lexical/dots/mindfa.dot -o /Users/hewro/lexical/images/mindfa.jpg");//����QT��ĺ���
    }
    cout << result << endl;
    return result;

}


/**
 * ��С��DFA
 */

bool isContain(int a, vector<int> b){
    for (int i = 0; i <b.size() ; ++i) {
        if (a == b[i])
            return true;
    }
    return false;
}

/**
 * �ж�ĳ��DFA״̬�ڻ��ֵļ����е����
 * @param a
 * @param b
 * @return
 */
int getContainPosition(int a, vector<pair<vector<int>,bool>> b) {
    for (int i = 0; i <b.size() ; ++i) {
        for (int j = 0; j <b[i].first.size() ; ++j) {
            if (a == b[i].first[j]){
                return i;
            }
        }
    }
    return -1;
}



void Lex::minimizeDFA() {
    cout << "����������������������������������������������С��DFA������������������������������������" << endl;


    vector<int> noEndPointArray;//����ֹ̬�ڵ㼯��
    vector<int> endPointArray(lexDFA.endStatus);

    /*stack<int>noEndDFAStack;//DFA�ķ���ֹ״̬ջ
    stack<int> endDFAStack;//��ֹ״̬��ջ*/
    cout << "����ֹ״̬����" << endl;
    for (int i = 1; i < lexDFA.mVexs.size(); ++i) {
        if (!isInDFAEndStatus(i)){
            noEndPointArray.push_back(i);
            cout << i << " ";
            //noEndDFAStack.push(i);
        }
    }//��ʼ������ֹ�ڵ㼯��
    cout << endl;

    cout << "��ֹ״̬����" << endl;
    for (int n = 0; n < endPointArray.size(); ++n) {
        cout << endPointArray[n] << " ";
    }
    cout << endl;
    vector<pair<vector<int>,bool>> dividedArrays;//first�洢���ǻ��ֵļ��ϣ�second�洢���Ǹû��ּ����Ƿ�ɼ�������
    dividedArrays.emplace_back(noEndPointArray, true);
    dividedArrays.emplace_back(endPointArray, true);

    bool flag = true;
    while(flag){

        for (int j = 0; j < dividedArrays.size(); ++j) {//�Ի��ֵ�ÿ�����Ͻ��в���

            cout << "����������������������ǰ�����ļ��ϵ�λ��Ϊ" << j << "���ϴ�СΪ" << dividedArrays.size()  << "��������������������" <<endl;
            for (int m = 0; m < dividedArrays[j].first.size(); ++m) {
                cout << dividedArrays[j].first[m] << " ";
            }
            cout << endl;

            int canNotBeDivided = 0;//����һ����ĸ���ת��������ü��ϵ�ת��״ֻ̬��һ����˵���ü��ϲ��ܱ�����ĸ���֣��ñ���+1

            if (dividedArrays[j].first.size() == 1){
                dividedArrays[j].second = false;//�������Ԫ��ֻ��һ������ֵΪfalse���������ٻ���
                continue;
            }

            for (int i = 0; i < alphabet.size() ; ++i) {

                cout << "��ǰ��ĸΪ" << alphabet[i] << endl;
                vector<int> arrayNumVector;//���DFA״̬����ĳ����ĸת�����ļ�����ŵ�����

                //first Ϊת��״̬���ڵļ�����ţ�second DFA����״̬�ڵ�
                vector<pair<int,int>> statusMap;//�����ÿ���ڵ��ת�������ڵļ�����š����ýڵ㱾�����

                for (int k = 0; k < dividedArrays[j].first.size(); ++k) { //��ȡ���ü��ϵ�ÿ��Ԫ�ص�ת��״̬���ڵļ������
                    int transStatus = lexDFA.getTargetStatus(dividedArrays[j].first[k],alphabet[i]);//��ȡ�ڵ��ת��DFA�ڵ�

                    int statusInArrayNum = getContainPosition(transStatus,dividedArrays);//ת��״̬���ڵļ������

                    cout << dividedArrays[j].first[k] << "ת����������ڵļ������" + to_string(statusInArrayNum) << endl;

                    if(statusInArrayNum == -1){//������л��֣����ʱ����Ȼû��ת�������������Ҫ�����������Ϊ����һ��Ψһ����
                        statusInArrayNum = -1;
                        arrayNumVector.push_back(statusInArrayNum);
                    }else{
                        if (!isContain(statusInArrayNum,arrayNumVector)){//��ֹ������ŵ��ظ�
                            arrayNumVector.push_back(statusInArrayNum);//��������ż��뵽�������������
                        }
                    }
                    statusMap.emplace_back(statusInArrayNum,dividedArrays[j].first[k]);//��������š����������ڵ�DFA״̬��ѹ��
                }

                if (arrayNumVector.size() == 1){
                    cout << "��ǰ�������" << j << "canNotBeDivided��1" <<endl;
                    canNotBeDivided ++ ;

                    continue;
                }else{
                    cout << "\n*******�ü�����Ҫ����*******\n"<< endl;

                    cout << "��ͬ��״̬����Ϊ" << arrayNumVector.size() << endl;
                    for (int m = 0; m < arrayNumVector.size(); ++m) {
                        cout << arrayNumVector[m] << " ";
                    }
                    cout << endl;
                    cout << "���ֵļ���Ϊ��" << endl;
                    for (int l = 0; l <  arrayNumVector.size(); ++l) {//���л���
                        cout << "��" << l + 1 << "�����ֵ��Ӽ�Ϊ" << endl;
                        vector<int> tempArray;
                        for (int k = 0; k < statusMap.size(); ++k) {
                            if (arrayNumVector[l] == -1 && statusMap[k].first == -1){//keyΪ-1.˵����һ��Ҫ���ֵ�
                                //ɾ����Ԫ��
                                statusMap[k].first = -2;//-2����ɾ��״̬
                                tempArray.push_back(statusMap[k].second);
                                cout << "��״̬���ڵļ������Ϊ" << statusMap[k].first << "  ";
                                cout << statusMap[k].second << endl ;
                                //dividedArrays.emplace_back(tempArray, true);
                                //tempArray.clear();
                                break;
                            } else{
                                if (statusMap[k].first == arrayNumVector[l]){//���ݼ�����Ž��л���
                                    cout << "��״̬���ڵļ������Ϊ" << statusMap[k].first << "   ";
                                    tempArray.push_back(statusMap[k].second);
                                    cout << statusMap[k].second << endl ;
                                }
                            }

                        }
                        cout << endl;
                        dividedArrays.emplace_back(tempArray, true);
                    }

                    auto iter =  dividedArrays.begin()+j;
                    dividedArrays.erase(iter);
                    j--;
                    break;//��ǰ���Ͻ�����������һ��λ�õļ��ϣ���Ϊɾ���˸�Ԫ�أ�����Ԫ��ǰ��һλ,����j--
                }
            }
            if (canNotBeDivided == alphabet.size()){
                dividedArrays[j].second = false;//���һ�����Ͼ���ת�����Ǹü��ϱ����ü��������ٽ��л���
            }
        }


        //�ж��Ƿ����ѭ����������ּ�����������м��϶����ɻ��־��˳�ѭ��
        flag = false;
        for (int m = 0; m < dividedArrays.size(); ++m) {
            if (dividedArrays[m].second == true){
                flag = true;
                break;
            }
        }
    }

    //�����ӡ���ļ���

    cout << "���������������������������������������յĻ��ּ���Ϊ��������������������������������������������" << endl;
    for (int i1 = 0; i1 < dividedArrays.size(); ++i1) {
        for (int i = 0; i < dividedArrays[i1].first.size() ; ++i) {
            cout << dividedArrays[i1].first[i] << " ";
        }
        cout << endl;
    }


    //�ϲ�DFA�ȼ�״̬

    for (int j1 = 0; j1 < dividedArrays.size(); ++j1) {
        if (dividedArrays[j1].first.size() > 1){//ֻҪÿ�����ϵĴ�С����1��˵������Ҫ�ϲ���
            int represent = dividedArrays[j1].first[0];
            for (int i = 1; i < dividedArrays[j1].first.size(); ++i) {//���˵�һ���ڵ㣬�����ڵ㶼�͵�һ���ڵ�ϲ�
                mergeTwoNode(represent,dividedArrays[j1].first[i]);//�ϲ��������ڵ�
            }
        }
    }



}

/**
 * ��ȡDFA��Ŀ��ڵ�
 * @param node
 * @param condition
 * @return
 */
int DFA::getTargetStatus(int node, char condition) {
    for (int i = 0; i < mVexs.size() ; ++i) {
        if (DFAGraph.getEdgeValue(node,i).at(0) == condition){
            return i;
        }
    }
    return -1;//��ʾĿ��ڵ㲻����
}


void Lex::mergeTwoNode(int a,int b){
    for (int i = 1; i < lexDFA.mVexs.size()+1 ; ++i) {
        if (i == b){
            for (int j = 1; j < lexDFA.mVexs.size()+1 ; ++j) {
                if (lexDFA.DFAGraph.getEdgeValue(b,j).at(0) != '^'){
                    if (j == b){
                        lexDFA.DFAGraph.addEdge(a,a,lexDFA.DFAGraph.getEdgeValue(b,j).at(0));
                    } else{
                        lexDFA.DFAGraph.addEdge(a,j,lexDFA.DFAGraph.getEdgeValue(b,j).at(0));
                    }
                    lexDFA.DFAGraph.deleteEdge(b,j);
                    lexDFA.mVexs[b] = vector<int>();
                }
            }
        } else{
            for (int j = 1; j < lexDFA.mVexs.size() + 1; ++j) {
                if (j == b && lexDFA.DFAGraph.getEdgeValue(i,b).at(0)!='^'){
                    lexDFA.DFAGraph.addEdge(i,a,lexDFA.DFAGraph.getEdgeValue(i,b).at(0));
                    lexDFA.DFAGraph.deleteEdge(i,j);
                    lexDFA.mVexs[b] = vector<int>();
                    break;
                }
            }
        }
    }
};


bool Lex::isInDFAEndStatus(int i){
    for (int j = 0; j < lexDFA.endStatus.size(); ++j) {
        if (i == lexDFA.endStatus[j]){
            return true;
        }
    }
    return false;
};
