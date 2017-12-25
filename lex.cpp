#include "lex.h"

#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<iomanip>
//#include <QProcess>
#include <sstream>

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
    appendToResultBox("根据字母创建最基本NFA的操作");
    cout << "根据字母创建最基本NFA的操作" << endl;
    //分配的节点序号是以前节点序号+1
    int startPoint = lexNFA.mVexs.size() + 1;
    int endPoint = startPoint + 1;
    lexNFA.NFAGraph.addEdge(startPoint,endPoint,ch);//增加一个边对边的连接，边的条件是转换条件，就是该字符。
    string tempString = "开始创建一个最基本的 NFA,序号为：(" + to_string(startPoint) + "," + to_string(endPoint) + ")";
    appendToResultBox(tempString);
    cout << tempString<< endl;
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
    insetIntoResultBox("\n进行重复符的操作");

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
    appendToResultBox("进行连接符的操作");
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
    string frontNode = "前节点序号:(" + to_string(top2NFA[0]) + "," + to_string(top2NFA[1])+ ")";
    string lateNode = "后节点序号:(" + to_string(top1NFA[0]) + "," + to_string(top1NFA[1]) + ")";
    cout << frontNode << endl;
    cout << lateNode << endl;
    appendToResultBox(frontNode);
    appendToResultBox(lateNode);

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
                if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) != '^'){
                    lexNFA.NFAGraph.addEdge(top2NFA[1],j,lexNFA.NFAGraph.getEdgeValue(i,j).at(0));
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
    appendToResultBox("进行选择符的操作");
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

    string tempString = "字符串为:" + regxInput + "字符串的长度为:";
    cout<< tempString << endl;
    appendToResultBox(tempString);
    /*1. 对输入的正则表达式进行循环扫描每一个元素，建立运算符栈和NFA栈*/
    for(unsigned long i =0; i < strLen; i++){
        ch = regxInput[i];
        stringstream stream2;
        stream2 << ch;
        string str = stream2.str();
        tempString = "\n*************当前字符为 " + str + " *************\n";
        cout<< "\n*************当前字符为 " << str << " *************\n" << endl;
        appendToResultBox(tempString);
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
                if((i+1<strLen)&&(regxInput[i+1] == '(' || !isOperator(regxInput[i+1]))){
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
            if(i+1 <strLen && (!isOperator(regxInput[i+1]) || regxInput[i+1] == '(')){
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

            if (tempArray.empty()){//如果转换产生的NFA集合为空，跳过该次转换即可
                continue;
            }
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

    cout << "————————————————开始e_closure(T)函数————————————————"<<endl;
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
                    if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) == 'E'){//找到转移条件为E的的终点
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

    cout << "————————————————开始move(T," << condition << ")函数————————————————"<<endl;

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
                    if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) == condition){
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
            if (lexNFA.NFAGraph.getEdgeValue(i,j).at(0) != '^'){
                cout << "状态" << i << " —— " << lexNFA.NFAGraph.getEdgeValue(i,j).at(0) << " ——> " << "状态" << j << endl;
            }
        }
    }
}


void Lex::printDFA(){
    int num = lexDFA.DFAGraph.mVexNum + 1;
    //cout << "节点数目" << num;
    for(int i = 0 ;i<num  ;i ++){
        for(int j = 0;j<num;j++){
            if (lexDFA.DFAGraph.getEdgeValue(i,j).at(0) != '^'){
                cout << "状态" << i << " —— " << lexDFA.DFAGraph.getEdgeValue(i,j).at(0) << " ——> " << "状态" << j << endl;
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
            if (myGraph.getEdgeValue(i,j).at(0) != '^'){
                result += tab + "\"" +to_string(i) + "\"" + " -> " + to_string(j) + "[label=\"" + myGraph.getEdgeValue(i,j).at(0) + "\"]\n";
            }
        }
    }
    result.append("}");

    ofstream ofile;               //定义输出文件
    ofile.open("../../../../lexical/dots/nfa.dot");
    ofile << result << endl;
    ofile.close();
    //执行生成nfa图片

    string temp = dot + " -Tjpg ../../../../lexical/dots/nfa.dot -o ../../../../lexical/images/nfa.jpg";
    const char *systemString = temp.c_str();

    cout << "命令行为" + temp << endl;
    system(systemString);//调用QT里的函数
    appendToResultBox(result);
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
    //cout << "节点数目" << num;
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
    ofstream ofile;//定义输出文件
    if (choice == 0){
        //system("pwd");
        ofile.open("../../../../lexical/dots/dfa.dot");
    }else{
        ofile.open("../../../../lexical/dots/mindfa.dot");
    }
    ofile << result << endl;
    ofile.close();
    //执行生成dfa图片
    string temp;
    if (choice == 0){
        temp = dot + " -Tjpg ../../../../lexical/dots/dfa.dot -o ../../../../lexical/images/dfa.jpg";
    }else{
        temp = dot + " -Tjpg ../../../../lexical/dots/mindfa.dot -o ../../../../lexical/images/mindfa.jpg";
    }
    const char *systemString = temp.c_str();

    cout << "命令行为" + temp << endl;

    system(systemString);
    appendToResultBox(result);
    cout << result << endl;
    return result;

}


/**
 * 最小化DFA
 */

bool isContain(int a, vector<int> b){
    for (int i = 0; i <b.size() ; ++i) {
        if (a == b[i])
            return true;
    }
    return false;
}

/**
 * 判断某个DFA状态在划分的集合中的序号
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
    cout << "——————————————————————最小化DFA——————————————————" << endl;


    vector<int> noEndPointArray;//非终止态节点集合
    vector<int> endPointArray(lexDFA.endStatus);

    /*stack<int>noEndDFAStack;//DFA的非终止状态栈
    stack<int> endDFAStack;//终止状态的栈*/
    cout << "非终止状态集合" << endl;
    for (int i = 1; i < lexDFA.mVexs.size(); ++i) {
        if (!isInDFAEndStatus(i)){
            noEndPointArray.push_back(i);
            cout << i << " ";
            //noEndDFAStack.push(i);
        }
    }//初始化非终止节点集合
    cout << endl;

    cout << "终止状态集合" << endl;
    for (int n = 0; n < endPointArray.size(); ++n) {
        cout << endPointArray[n] << " ";
    }
    cout << endl;
    vector<pair<vector<int>,bool>> dividedArrays;//first存储的是划分的集合，second存储的是该划分集合是否可继续划分
    dividedArrays.emplace_back(noEndPointArray, true);
    dividedArrays.emplace_back(endPointArray, true);

    bool flag = true;
    while(flag){

        for (int j = 0; j < dividedArrays.size(); ++j) {//对划分的每个集合进行操作



            int canNotBeDivided = 0;//经过一次字母表的转换，如果该集合的转换状态只有一个，说明该集合不能被该字母区分，该变量+1

            if (dividedArrays[j].first.size() == 1){
                dividedArrays[j].second = false;//如果集合元素只有一个，赋值为false，即不可再划分
                continue;
            }

            for (int i = 0; i < alphabet.size() ; ++i) {
                cout << "——————————当前操作的集合为——————————" <<endl;
                for (int m = 0; m < dividedArrays[j].first.size(); ++m) {
                    cout << dividedArrays[j].first[m] << " ";
                }
                cout << endl;

                cout << "当前转换的字母为" << alphabet[i] << endl;

                vector<int> arrayNumVector;//存放DFA状态经过某个字母转换到的集合序号的数组

                //first 为转换状态属于的集合序号，second DFA起点的状态节点
                vector<pair<int,int>> statusMap;//存放了每个节点的转换后属于的集合序号——该节点本身序号

                for (int k = 0; k < dividedArrays[j].first.size(); ++k) { //获取到该集合的每个元素的转换状态属于的集合序号
                    int transStatus = lexDFA.getTargetStatus(dividedArrays[j].first[k],alphabet[i]);//获取节点的转换DFA节点

                    int statusInArrayNum = getContainPosition(transStatus,dividedArrays);//转换状态属于的集合序号

                    if(statusInArrayNum == -1){//必须进行划分，这个时候虽然没有转换结果，所以需要将集合序号人为设置一个唯一的数
                        statusInArrayNum = static_cast<int>(statusMap.size() + 1);
                    }

                    if (!isContain(statusInArrayNum,arrayNumVector)){//防止集合序号的重复
                        arrayNumVector.push_back(statusInArrayNum);//将集合序号加入到集合序号数组中
                    }
                    statusMap.emplace_back(statusInArrayNum,dividedArrays[j].first[k]);//将集合序号————对于的DFA状态组压入
                }

                if (arrayNumVector.size() == 1){
                    canNotBeDivided ++ ;

                    continue;
                }else{
                    cout << "\n*******该集合需要划分*******\n"<< endl;

                    cout << "划分的集合为：" << endl;
                    for (int l = 0; l <  arrayNumVector.size(); ++l) {//进行划分
                        cout << "第" << l + 1 << "个划分的子集为" << endl;
                        vector<int> tempArray;
                        for (int k = 0; k < statusMap.size(); ++k) {
                            if (statusMap[k].first == arrayNumVector[l]){//根据集合序号进行划分
                                tempArray.push_back(statusMap[k].second);
                                cout << statusMap[k].second << " " ;
                            }
                        }
                        cout << endl;
                        dividedArrays.emplace_back(tempArray, true);
                    }

                    auto iter =  dividedArrays.begin()+j;
                    dividedArrays.erase(iter);
                    //j--;
                }
            }
            if (canNotBeDivided == alphabet.size()){
                dividedArrays[j].second = false;//如果一个集合经过转换后还是该集合本身，该集合无需再进行划分
            }
        }


        //判断是否结束循环，如果划分集合下面的所有集合都不可划分就退出循环
        flag = false;
        for (int m = 0; m < dividedArrays.size(); ++m) {
            if (dividedArrays[m].second == true){
                flag = true;
                break;
            }
        }
    }

    //输出打印最后的集合

    cout << "——————————————————最终的划分集合为——————————————————————" << endl;
    for (int i1 = 0; i1 < dividedArrays.size(); ++i1) {
        for (int i = 0; i < dividedArrays[i1].first.size() ; ++i) {
            cout << dividedArrays[i1].first[i] << " ";
        }
        cout << endl;
    }


    //合并DFA等价状态

    for (int j1 = 0; j1 < dividedArrays.size(); ++j1) {
        if (dividedArrays[j1].first.size() > 1){//只要每个集合的大小大于1，说明有需要合并的
            int represent = dividedArrays[j1].first[0];
            for (int i = 1; i < dividedArrays[j1].first.size(); ++i) {//除了第一个节点，其他节点都和第一个节点合并
                mergeTwoNode(represent,dividedArrays[j1].first[i]);//合并这两个节点
            }
        }
    }



}

/**
 * 获取DFA的目标节点
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
    return -1;//表示目标节点不存在
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

void Lex::insetIntoResultBox(string content){
    resultBrowser->textCursor().insertText(QString::fromStdString(content));
}

void Lex::appendToResultBox(string content){
    resultBrowser->append(QString::fromStdString(content));
}
