#riched20
这是一个riched20代理，可以记录riched20的内容。大家可以发挥想像力，
用它记录比如MSN和skype等一些使用riched20.dll展示聊天内容的软件的聊天内容。
使用时把系统的`riched20.dll`改名为`RichEd20.orig.Dll`，
放到主程序的目录里，再把这个项目的richEd20.dll放到和`RichEd20.orig.Dll`同一个目录里，
然后启动聊天软件，开始聊天。关闭聊天窗口后，聊天记录保存在`%TEMP%\chat`文件夹中。