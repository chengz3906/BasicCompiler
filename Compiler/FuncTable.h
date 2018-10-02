#ifndef FUNCTABLE_H
#define FUNCTABLE_H

#include <string>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include "ErrorHandle.h"
using namespace std;

struct FEle{
    int paraNo;
    int length;//该函数局部变量、内务信息区在运行栈中所占存储单元总数
    bool isVoid;//是否为无返回值函数
    bool isLeaf;//是否为叶子函数，即函数体内不存在函数调用
    int srNum;//需要使用的全局寄存器数量
};

class FuncTable
{
    public:
        FuncTable();
        static int addEle(bool isVoid);
        static int getParaNo(int index);
        static int getIsVoid(int index);
		static int getLen(int index);
		static bool getIsLeaf(int index);
        static void setParaNo(int index, int val);
        static void setLen(int index, int val);
        static void setIsLeaf(int index, bool val);
        static void setSRnum(int index, int val);
		static int getSRnum(int index);
    protected:
    private:
        static struct FEle FTab[50];
        static int i;
};

#endif // FUNCTABLE_H
