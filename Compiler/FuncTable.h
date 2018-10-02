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
    int length;//�ú����ֲ�������������Ϣ��������ջ����ռ�洢��Ԫ����
    bool isVoid;//�Ƿ�Ϊ�޷���ֵ����
    bool isLeaf;//�Ƿ�ΪҶ�Ӻ��������������ڲ����ں�������
    int srNum;//��Ҫʹ�õ�ȫ�ּĴ�������
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
