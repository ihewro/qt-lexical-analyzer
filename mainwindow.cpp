#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lex.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifdef Q_OS_MAC
// mac
    projectFile = "../../../../lexical";
#endif
#ifdef Q_OS_LINUX
// linux
    projectFile = "../lexical";
#endif

#ifdef Q_OS_WIN
// win
    projectFile = "../lexical";
#endif
    ui->setupUi(this);
    ui->statusBar->showMessage("欢迎使用词法分析器",3000);
    QLabel *permanent = new QLabel(this);
    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    permanent->setText("北京化工大学 何炜");
    ui->statusBar->addPermanentWidget(permanent);
    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath() << endl;
    qDebug()<<"current currentPath: "<<QDir::currentPath() << endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Lex lexTest;
    lexTest.resultBrowser = ui->outputField;//赋值
    ui->outputField->clear();

    string regxInput = ui->regxText->text().toStdString();
    fillContentToOutputFeiled("正则表达式为：" + regxInput);
    lexTest.getNFA(regxInput);
    cout << "————————————————————————————————————————结果显示——————————————————————————————————————————" << endl;
    fillContentToOutputFeiled("————————————————————————————————————————结果显示——————————————————————————————————————————" );
    cout << "最终的NFA栈的节点数目为"<< lexTest.lexNFA.NFAGraph.mVexNum << endl;
    fillContentToOutputFeiled("最终的NFA栈的节点数目为" + to_string(lexTest.lexNFA.NFAGraph.mVexNum));
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
    fillContentToOutputFeiled("————————————————DFA节点输出——————————————");
    for(int i =0;i<lexTest.lexDFA.mVexs.size();i++){
        string tempString = "";
        for (int j = 0; j < lexTest.lexDFA.mVexs[i].size() ; ++j) {
            cout << lexTest.lexDFA.mVexs[i][j] << " ";
            tempString += to_string(lexTest.lexDFA.mVexs[i][j]) + " ";
        }

        fillContentToOutputFeiled(tempString);

        cout << endl;
    }


    cout << "DFA节点数目" << lexTest.lexDFA.DFAGraph.mVexNum << "边的数目" << lexTest.lexDFA.DFAGraph.mEdgeNum << endl;

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,0);

    lexTest.minimizeDFA();
    //system("dot -Tjpg aaa.dot -o aaa.jpg");

    lexTest.generateDFADotString(lexTest.lexDFA.DFAGraph,1);

}


void MainWindow::fillContentToOutputFeiled(string value){
    ui->outputField->append(QString::fromStdString(value));
}

void MainWindow::on_export_nfa_button_clicked()
{
    //加载图片
    QTextImageFormat format;
    format.setName(getImageSrc("nfa.jpg"));
    ui->textBrowser->clear();
    ui->textBrowser->textCursor().insertImage(format);


}

void MainWindow::on_export_dfa_button_clicked()
{
    //加载图片
    QTextImageFormat format;
    format.setName(getImageSrc("dfa.jpg"));
    ui->textBrowser->clear();
    ui->textBrowser->textCursor().insertImage(format);
}

void MainWindow::on_mini_dfa_button_clicked()
{
    //输出最小化DFA
    QTextImageFormat format;

    format.setName(getImageSrc("mindfa.jpg"));
    ui->textBrowser->clear();
    ui->textBrowser->textCursor().insertImage(format);

}

void MainWindow::on_action_about_triggered()
{
    //打开关于对话框
    QMessageBox::about(this,"关于","生成NFA、DFA、最小化DFA的可视化工具");

}

QString MainWindow::getImageSrc(string fileName){
    string resultString = projectFile + "/images/" + fileName;
    cout << "当前图片路径为" << resultString << endl;
    return QString::fromStdString(resultString);
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->outputField->clear();
    ui->textBrowser->clear();
    ui->regxText->clear();

}
