# qt-lexical-analyzer
使用c++ qt 写的词法分析器（编译原理作业）

![](https://ws3.sinaimg.cn/large/006tKfTcly1fmt3gzdnnmj31ai1eimys.jpg)

## 编译环境

* 语言： c++ 11
* IDE：qt creator、 CLion
* 界面库：qt、GraphViz
* 开发平台：mac下开发，Windows下未做测试 


## 运行

1. 下载源码
2. 使用qtCreator 打开项目文件夹
3. 下载[graphviz绘图软件](https://graphviz.gitlab.io/download/)
4. 修改lex.h文件中的`dot`常量，该常量为你的系统中dot命令的位置。如果是mac系统，你可以通过 which dot 查看具体位置。


```Bash
	which dot //bash命令，会输出你的dot指令的位置
```

```c++
	const string dot  = "/usr/local/bin/dot";
```



**关于[Graphviz下载安装方法](https://graphviz.gitlab.io/download/)：**

* mac：如果你安装了homeBrew，直接终端输入安装命令

```bash
brew install Graphviz
```

* windows: <https://graphviz.gitlab.io/_pages/Download/Download_windows.html>


**关于code分支：**

该分支为无界面画图的代码，是在Clion的IDE下编译运行通过，你也可以尝试编译运行这份代码。