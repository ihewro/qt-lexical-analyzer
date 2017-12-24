#include "lex.h"

#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<iomanip>
#include <QProcess>

using namespace std;

Lex::Lex() = default;

/**
 * @brief Lex::isOperator 判断是否是运算符
 * @param ch
 * @return
 */
bool Lex::isOperator(char ch){
    return ch == '|' || ch == '*' || ch == '(' || ch == ')';
}

void Lex::createBasicNFA(char ch){
    cout << "根据字母创建最基本NFA的操作" << endl;
    //分配的节点序号是以前节点序号+1
    int startPoint = lexNFA.mVexs.size() + 1;
    int endPoint = startPoint + 1;
    lexNFA.NFAGraph.addEdge(startPoint,endPoint,ch);//增加一个边对边的连接，边的条件是转换条件，就是该字符。
    cout << "开始创建一个最基本的 NFA,序号为：(" << startPoint << "," << endPoint << ")" << endl;
    //增加两个节点
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //边数加1
    lexNFA.NFAGraph.mEdgeNum++;

    //添加到节点数组中
    lexNFA.mVexs.push_back(startPoint);
    lexNFA.mVexs.push_back(endPoint);

    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = startPoint;
    newNFAStatusPoint[1] = endPoint;

    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //起点终点设置
    lexNFA.startStatus = startPoint;
    lexNFA.endStatus = endPoint;
}

/**
 * @brief Lex::repeatCharactorOperation
 * 重复符号的计算*：
 * 0. 计算完成后要将NFA栈顶元素出栈，计算完就没用了
 * 1. 出NFA栈顶的元素
 * 2. 新建两个头尾节点，新头节点连接以前的头结点，尾节点连接以前的尾节点
 * 3. 将新的NFA压入NFA栈中
 */
void Lex::repeatCharacterOperation(){
    cout << "进行重复符的操作" << endl;
    //1.获取栈顶的一个元素
    int top1NFA[2];
    for(int i =0;i<2;i++){
        top1NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();

    //2.
    //创建两个新的节点
    int newStartPoint1 = lexNFA.mVexs.size()+1;
    int newStartPoint2 = newStartPoint1+1;

    lexNFA.NFAGraph.addEdge(newStartPoint1,newStartPoint2,'E');
    lexNFA.NFAGraph.addEdge(newStartPoint1,top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],newStartPoint2,'E');

    //节点数目加2
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //边的数目加4
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 4;
    //添加到节点数组中
    lexNFA.mVexs.push_back(newStartPoint1);
    lexNFA.mVexs.push_back(newStartPoint2);

    //3.
    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = newStartPoint1;
    newNFAStatusPoint[1] = newStartPoint2;
    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //起点终点设置
    lexNFA.startStatus = newStartPoint1;
    lexNFA.endStatus = newStartPoint2;

}

/**
 * @brief Lex::joinerCharacterOperation
 * 连接符号的计算&：
 * 0. 把运算符和两个操作数最后都要pop掉
 * 1. 出NFA栈顶两个元素
 * 2. 栈次顶的尾节点栈顶的头节点连接
 * 3. 将新的NFA压入栈中
 */
void Lex::joinerCharacterOperation(){
    /**
     * 这里有一个合并节点的问题，我们的NFA栈中只存储了起始信息，所以合并节点变得很困难，
     * 而且不合并节点带来的唯一坏处就是NFA的节点和边变多，不会影响最后的匹配结果。
     */

    cout << "\n进行连接符的操作\n" << endl;
    //1.
    //获取NFA栈顶的两个元素，并从栈中丢掉
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
    cout << "前节点序号:(" << top2NFA[0] << "," << top2NFA[1] << ")" << endl;
    cout << "后节点序号:(" << top1NFA[0] << "," << top1NFA[1] << ")" << endl;
    cout << endl;
    //获取运算符栈顶的元素，并从栈中丢掉，因为运算完，这些就没用了，有用的是运算结果
    operatorStack.pop();

    //2.
    //lexNFA.NFAGraph.addEdge(top2NFA[1],top1NFA[1],lexNFA.NFAGraph.getEdgeValue(top1NFA[0],top1NFA[1]));
    //lexNFA.NFAGraph.deleteEdge(top1NFA[0],top1NFA[1]);
    //边数不变
    //lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 1;
    //节点数减1，因为合并了一个节点
    //lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum - 1;

    /*//不合并节点，直接把前节点的尾连接后节点的头
    lexNFA.NFAGraph.addEdge(top2NFA[1],top1NFA[0],'E');
    //边数加1
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 1;*/

    //合并节点，边数不变，节点数减1

    for(int i =1;i<lexNFA.NFAGraph.mVexNum + 1;i++){
        if (i == top1NFA[0]){
            for (int j = 1; j <lexNFA.NFAGraph.mVexNum + 1 ; ++j) {
                if (lexNFA.NFAGraph.getEdgeValue(i,j) != '^'){
                    lexNFA.NFAGraph.addEdge(top2NFA[1],j,lexNFA.NFAGraph.getEdgeValue(i,j));
                    lexNFA.NFAGraph.deleteEdge(top1NFA[0],j);
                    lexNFA.NFAGraph.mVexNum --;//节点数减1
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

    //设置起点和终点
    lexNFA.startStatus = top2NFA[0];
    lexNFA.endStatus = top1NFA[1];

}

/**
 * @brief Lex::selectorCharacterOperation
 * 选择符号的计算|：
 * 0. 计算完成后，要将栈顶两个元素都出栈，理由同上
 * 1. 出NFA栈顶两个元素
 * 2. 新建两个头尾节点，新头结点连接两个以前的头结点，新尾节点连接两个以前的尾节点
 * 3. 将新的NFA压入栈中
 */
void Lex::selectorCharacterOperation(){
    cout << "进行选择符的操作" << endl;
    //1.
    //获取NFA栈顶的两个元素，并从栈中丢掉
    int top1NFA[2];
    for(int i =0;i<2;i++){
        top1NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    cout << "当前NFA栈顶的节点为：(" << top1NFA[0] << "," << top1NFA[1] << ")" << endl;

    NFAStatusPointStack.pop();
    int top2NFA[2];
    for(int i =0;i<2;i++){
        top2NFA[i] = NFAStatusPointStack.top().nArray[i];
    }
    NFAStatusPointStack.pop();
    //获取运算符栈顶的元素，并从栈中丢掉，因为运算完，这些就没用了，有用的是运算结果
    operatorStack.pop();

    //2.
    //创建两个新的节点
    int newStartPoint1 = lexNFA.mVexs.size()+1;
    int newStartPoint2 = newStartPoint1+1;

    lexNFA.NFAGraph.addEdge(newStartPoint1,top1NFA[0],'E');
    lexNFA.NFAGraph.addEdge(newStartPoint1,top2NFA[0],'E');
    lexNFA.NFAGraph.addEdge(top2NFA[1],newStartPoint2,'E');
    lexNFA.NFAGraph.addEdge(top1NFA[1],newStartPoint2,'E');

    //节点数目加2
    lexNFA.NFAGraph.mVexNum = lexNFA.NFAGraph.mVexNum+2;
    //边的数目加4
    lexNFA.NFAGraph.mEdgeNum = lexNFA.NFAGraph.mEdgeNum + 4;
    //添加到节点数组中
    lexNFA.mVexs.push_back(newStartPoint1);
    lexNFA.mVexs.push_back(newStartPoint2);



    //3.
    int newNFAStatusPoint[2];
    newNFAStatusPoint[0] = newStartPoint1;
    newNFAStatusPoint[1] = newStartPoint2;

    NFAStatusPointStack.push(node(newNFAStatusPoint));

    //设置起点和终点
    lexNFA.startStatus = newStartPoint1;
    lexNFA.endStatus = newStartPoint2;

}

/**
 * @brief Lex::getNFA ：求取NFA，关键步骤
 * @param regxInput
 * 1. 对输入的正则表达式进行循环扫描每一个元素，建立运算符栈和NFA栈
 * 2. 对最终的NFA栈进行处理（如果不为空的话）
 */
void Lex::getNFA(string regxInput){
    unsigned long strLen = regxInput.length();
    char ch;

    cout<< "字符串为:" << regxInput << "字符串的长度为:" << strLen << endl;
    /*1. 对输入的正则表达式进行循环扫描每一个元素，建立运算符栈和NFA栈*/
    for(unsigned long i =0; i < strLen; i++){
        ch = regxInput[i];
        cout<< "\n*************当前字符为 " << ch << " *************\n" << endl;
        //如果是运算符
        if(isOperator(ch)){
            switch (ch) {
            case '*'://重复符优先级别第一
                cout << "ok*" << ch << endl;
                //计算结果压入NFA栈中
                repeatCharacterOperation();

                //如果下一个字符是字母或者是左括号，需要添加连接符
                if((i+1<strLen)&&(regxInput[i+1] == '(' || !isOperator(regxInput[i+1]))){
                    operatorStack.push('&');
                }

                break;
            case '|'://选择符优先级第三，可以省略不写
                //需要将运算符栈符号中的&出栈并计算，遇到左括号(停止
                if (operatorStack.empty()){
                    cout<<"运算符栈为空"<< endl;
                    //如果运算符栈为空，就什么事情也不用做，最后一下把该符号压栈就可以了
                }else{
                    cout<<"运算符栈不为空"<< endl;
                    ch = operatorStack.top();
                    cout << "当前栈顶元素为" << ch << endl;
                    while(ch != '('){
                        //cout << "loop" << endl;
                        if(ch == '&'){
                            cout << "栈顶元素为连接符，需要先进行连接符的计算" << endl;
                            joinerCharacterOperation();
                        } else{
                            break;
                        }
                        ch = operatorStack.top();
                    }
                }

                operatorStack.push('|');
                cout<<"将选择符压入栈中"<<endl;
                break;
            case '('://左括号
                operatorStack.push(ch);
                cout << "将左括号压入栈中" << endl;
                cout << "当前符号栈大小" << operatorStack.size() << endl;
                break;
            case ')'://右括号
                cout << "符号栈大小" << operatorStack.size() << endl;
                //这里需要对括号内进行计算，并把计算结果压栈
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
                cout << "顶部符号" << ch << endl;
                operatorStack.pop();//此时运算符栈顶元素是左括号，需要移除出去
                //如果下一个字符是字母或者是左括号，需要添加连接符
                if((i+1<strLen)&&(regxInput[i+1] == ')' || !isOperator(regxInput[i+1]))){
                    operatorStack.push('&');
                }
                break;

            default:
                cout << "ok" << ch << endl;
                break;
            }
        }else{//不是运算符
            auto flag = true;//是否添加到字母表中
            for (int i =0;i<alphabet.size();i++){
                if (ch == alphabet[i])
                    flag = false;
            }

            if (flag){
                alphabet.push_back(ch);
            }
            //创建一个基本的NFA，也就是比如a,a的NFA就是两个节点，起始点和终止点，边的值为a
            createBasicNFA(ch);

            //如果下一个字符是字母的话，就向符号栈中加入一个连接符&
            if(i+1 <strLen && !isOperator(regxInput[i+1])){
                operatorStack.push('&');
            }
        }
    }

    /*2. 对最终的NFA栈和运算符栈进行处理（如果不为空的话）*/
    cout << "---------------------------------------------" << endl;
    if (operatorStack.empty()){
        cout << "最终中运算符栈为空" << endl;
    }else{
        cout << "最终中运算符栈不为空" << endl;
    }
    while(!operatorStack.empty()){
        ch= operatorStack.top();
        cout << "当前的运算符栈顶元素为" << ch << endl;
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

    //给DFA节点开头位置加一个空元素占位
    lexDFA.mVexs.emplace_back(0);

    vector<int> initStatus;
    initStatus.push_back(lexNFA.startStatus);
    vector<int>initStatusTrans(e_closure(vector<int>(initStatus)));

    //给DFA创建第一个节点
    int newDFAStartPoint = lexDFA.DFAGraph.mVexNum + 1;
    lexDFA.DFAGraph.mVexNum ++;//节点数加1

    lexDFA.startStatus = newDFAStartPoint;

    stack<int> DFAStatusStack;//DFA状态栈,这个栈只需要存储DFA的序号就可以了，没必要存储DFA节点对应的NFA集合，存储还没有经过字母表转换的DFA状态
    DFAStatusStack.push(newDFAStartPoint);

    lexDFA.mVexs.push_back(initStatusTrans);//NFA起点的E转换集合赋值给DFA的第一个节点

    //cout<<"DFA节点数" << lexDFA.mVexs.size() << " " << lexDFA.DFAGraph.mVexNum << endl;

    while (!DFAStatusStack.empty()){

        int topDFAStack = DFAStatusStack.top();
        DFAStatusStack.pop();

        for (int i = 0; i < alphabet.size() ; ++i) {//对字母表的每个元素都需要作为转换条件

            vector<int> tempArray = e_closure(nfaMove(lexDFA.mVexs[topDFAStack],alphabet[i]));

            int position = isDFAStatusRepeat(tempArray);//判断新生成DFA状态是否已经存在了，如果已经存在，返回该状态在节点的位置

            if (position == -1){//这个是新生成的DFA状态，没有重复
                int tempDFAStatusNode = lexDFA.DFAGraph.mVexNum + 1;
                lexDFA.DFAGraph.mVexNum ++;//节点数加1
                if (isEndDFAStatus(tempArray)){
                    lexDFA.endStatus.push_back(tempDFAStatusNode);
                }
                lexDFA.mVexs.push_back(tempArray);//把NFA集合赋值给DFA的状态
                DFAStatusStack.push(tempDFAStatusNode);//把新产生的DFA状态加入到DFA栈中
                position = tempDFAStatusNode;
            }

            //连接节点，产生边
            lexDFA.DFAGraph.addEdge(topDFAStack,position,alphabet[i]);
            lexDFA.DFAGraph.mEdgeNum ++;
        }
    }
}

/**
 * 判断是否是DFA的终止状态，只要包含了NFA的终止状态的DFA状态都是终止状态
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
 * 判断新产生的DFA状态是否已经存在DFA状态表中
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
 * DFA:能够从NFA的状态数组statusArray中的某个状态s开始只通过E转换到达的NFA状态集合
 * @param statusArray
 * @param lexNFA
 * @return
 */
vector<int> Lex::e_closure(vector<int> statusArray){

    cout << "————————————————开始e_closure函数————————————————"<<endl;
    cout << "状态数组为:" << endl;
    for (int k = 0; k < statusArray.size(); ++k) {
        cout << statusArray[k] << " ";
    }
    cout << endl;

    vector<int> resultsArray;//存放状态数组的E转换集合
    stack<int> statusStack;//存放递归过程中的状态，当该栈为空的时候，递归结束

    for (int i = 0; i < statusArray.size(); ++i) {
        statusStack.push(statusArray[i]);//初始化状态栈
        resultsArray.push_back(statusArray[i]);//状态本身也可以通过E转换到达本身，所以需要将自身添加到结果数组中
    }


    while(!statusStack.empty()){
        int status = statusStack.top();
        statusStack.pop();

        for (int i = 1; i < lexNFA.mVexs.size() + 1 ; ++i) {
            if (i == status){
                for(int j = 1;j<lexNFA.mVexs.size()+1;j++){
                    if (lexNFA.NFAGraph.getEdgeValue(i,j) == 'E'){//找到转移条件为E的的终点
                        statusStack.push(j);//加入到状态栈中
                        resultsArray.push_back(j);//加入到结果数组中
                    }
                }
            }
        }
    }

    /*去除重复元素，unique()只能去除相邻的重复元素，所有先排序再去重*/
    sort(resultsArray.begin(),resultsArray.end());
    resultsArray.erase(unique(resultsArray.begin(), resultsArray.end()), resultsArray.end());

    cout << "ecolosure转换结果为：" << endl;

    for (int k = 0; k < resultsArray.size(); ++k) {
        cout << resultsArray[k] << " ";
    }
    cout << endl;
    return resultsArray;

}


/**
 * DFA: 能够从NFA的状态数组statusArray中的某个状态出发，通过条件为condition转换到达的NFA状态集合
 * @param statusArray
 * @param condition
 * @param lexNFA
 * @return
 */
vector<int> Lex::nfaMove(vector<int> statusArray,char condition){

    cout << "————————————————开始move函数————————————————"<<endl;

    cout<< "状态数组为" << endl;
    for (int k = 0; k < statusArray.size(); ++k) {
        cout << statusArray[k] << " " ;
    }
    cout << endl;
    vector<int>resultsArray;//结果集合
    stack<int> statusStack;//状态栈

    for (int j = 0; j < statusArray.size() ; ++j) {
        statusStack.push(statusArray[j]);
    }

    while(!statusStack.empty()){
        int status = statusStack.top();
        statusStack.pop();
        for (int i = 1; i <lexNFA.mVexs.size()+1 ; ++i) {
            if (i == status){
                for (int j = 1; j < lexNFA.mVexs.size() + 1; ++j) {
                    if (lexNFA.NFAGraph.getEdgeValue(i,j) == condition){
                        resultsArray.push_back(j);//把终点加入到结果集合中，但此时不需要再将终点压入到状态栈中
                    }
                }
            }
        }
    }

    /*去除重复元素，unique()只能去除相邻的重复元素，所有先排序再去重*/
    sort(resultsArray.begin(),resultsArray.end());
    resultsArray.erase(unique(resultsArray.begin(), resultsArray.end()), resultsArray.end());

    cout << "move结果为:" << endl;
    for (int l = 0; l < resultsArray.size(); ++l) {
        cout << resultsArray[l] << " ";
    }
    cout << endl;
    return resultsArray;
};




void Lex::printNFA(){
    int num = static_cast<int>(lexNFA.mVexs.size() + 1);
    //cout << "节点数目" << num;
    for(int i = 1 ;i<num  ;i ++){
        for(int j = 1;j<num;j++){
            if (lexNFA.NFAGraph.getEdgeValue(i,j) != '^'){
                cout << "状态" << i << " —— " << lexNFA.NFAGraph.getEdgeValue(i,j) << " ——> " << "状态" << j << endl;
            }
        }
    }
}


void Lex::printDFA(){
    int num = lexDFA.DFAGraph.mVexNum + 1;
    //cout << "节点数目" << num;
    for(int i = 0 ;i<num  ;i ++){
        for(int j = 0;j<num;j++){
            if (lexDFA.DFAGraph.getEdgeValue(i,j) != '^'){
                cout << "状态" << i << " —— " << lexDFA.DFAGraph.getEdgeValue(i,j) << " ——> " << "状态" << j << endl;
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
    //cout << "节点数目" << num;
    for(int i = 1 ;i<num  ;i ++){
        for(int j = 1;j<num;j++){
            if (myGraph.getEdgeValue(i,j) != '^'){
                result += tab + "\"" +to_string(i) + "\"" + " -> " + to_string(j) + "[label=\"" + myGraph.getEdgeValue(i,j) + "\"]\n";
            }
        }
    }
    result.append("}");

    ofstream ofile;               //定义输出文件
    ofile.open("/Users/hewro/lexical/dots/nfa.dot");
    ofile << result << endl;
    ofile.close();
    //执行生成nfa图片
    system("/usr/local/bin/dot -Tjpg /Users/hewro/lexical/dots/nfa.dot -o /Users/hewro/lexical/images/nfa.jpg");//调用QT里的函数
    cout << result << endl;
    return  result;

}

string Lex::generateDFADotString(MyGraph myGraph){

    string tab = "    ";
    string result;
    result = "digraph G{\n" + tab + "\"\"[shape=none]\n";
    for (int i = 1; i <myGraph.mVexNum + 1 ; ++i) {
        result += tab + "\"" + to_string(i) +  "\"";
         //ofile<<tab<<"\""<<to_string(i)<<"\"";
        bool flag = true;

        for (int j = 0; j < lexDFA.endStatus.size(); ++j) {
            if (i == lexDFA.endStatus[j]){
                flag = false;
                break;
            }
        }
        if (!flag){
            //ofile << "[shape=doublecircle]\n";
            result += "[shape=doublecircle]\n";
        }else{
            //ofile << "[shape=circle]\n";
            result += "[shape=circle]\n";
        };
    }

    //ofile<< endl;
    result += "\n";
    //ofile << tab << "\"\"->\"" + to_string(lexDFA.startStatus)+ "\"\n";
    result += tab + "\"\"->\"" + to_string(lexDFA.startStatus)+ "\"\n";

    int num = myGraph.mVexNum + 1;
    //cout << "节点数目" << num;
    for(int i = 0 ;i<num  ;i ++){
        for(int j = 0;j<num;j++){
            if (myGraph.getEdgeValue(i,j) != '^'){
                result += tab + "\"" +to_string(i) + "\"" + " -> " + to_string(j) + "[label=\"" + myGraph.getEdgeValue(i,j) + "\"]\n";
            }
        }
    }
    result.append("}");
    ofstream ofile;               //定义输出文件
    ofile.open("/Users/hewro/lexical/dots/dfa.dot");
    ofile << result << endl;
    ofile.close();
    //执行生成dfa图片
    system("/usr/local/bin/dot -Tjpg /Users/hewro/lexical/dots/dfa.dot -o /Users/hewro/lexical/images/dfa.jpg");//调用QT里的函数
    cout << result << endl;
    return result;

}
