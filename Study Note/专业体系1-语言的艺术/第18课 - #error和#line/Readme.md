# 第18课 - #error和#line

## #error的用法
#error用于生成一个编译错误消息，并停止编译。

用法：#error message

注：message不需要用双引号包围。

#error编译指示字用于自定义程序员特有的编译错误消息类似的，#warning用于生成编译警告，但不会停止编译。


## #line的用法
#line用于强制指定新的行号和编译文件名，并对源程序 ，并对源程序的代码重新编号。

用法：#line number filename   注：filename可省略。

line编译指示字的本质是重定义`__LINE__和__FILE__`

 