#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fillContentToOutputFeiled(string value);
    QString getImageSrc(string fileName);
    string projectFile;

private slots:
    void on_pushButton_clicked();

    void on_export_nfa_button_clicked();

    void on_export_dfa_button_clicked();

    void on_mini_dfa_button_clicked();

    void on_action_about_triggered();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
