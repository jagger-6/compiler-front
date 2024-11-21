1.输入 
2.种别码表（关键字 标识符（下划线数字字母） 界符 常量 运算符） 动态加入
3.封装二元组以便语法分析
4.autotest
5.错误处理
6.字符串（常量）中的关键字
7.注释
8.扫描函数->有空格跳过（字符串常量例外） 如何识别单词种类 token每识别一种就刷新   

项目规范：
1.include
2.src
3.makefile
4.生成flex 输入文件 -o 输出文件
5.build


第一天：
稿makefile，最后要了个之前跟着做的项目的makefile
以及git与github
syntable文件读取 错误：某一行末尾有空格读取失败 如何解决
仍需要解决的问题：compiler 参数（种别码表文件位置）已解决

第二天：
scanner 符号表和常数表合并 种别码表更改大小写
scanner功能：已能处理实验源程序if x>9 x=2*x+1/3;（空格问题？）已解决
遇到问题：\0处理    已解决
参数功能添加 解决：文件一次性读取所有内容 buffer

第三天：
带下划线的标识符识别
单行注释的识别

第四天：
种别码表扫描（除了"" !=）已添加
标识符大小写识别
十六进制数识别(范围标注：ascii)
遇到问题：处理0开头的数字
双精度常量识别 regex引用

第五天：
STRING识别
多行注释识别
解决标识符不能以数字开头
种别码加入string

第六天
图像识别
sudo apt-get install tesseract-ocr
sudo apt-get install python-imaging
pip install pytesseract
提高图像识别准确度
图片提取代码末尾有FF标志（剔除）
建立总makefile与py文件的makefile联系
